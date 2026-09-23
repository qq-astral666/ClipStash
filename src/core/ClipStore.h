#pragma once

#include "ClipItem.h"

#include <QList>
#include <QObject>
#include <QSqlDatabase>

#include <optional>

// SQLite-backed clipboard history.
//
// Thread-safety: read-only methods may be called from any thread (the QML
// image provider loads thumbnails off the GUI thread). Every thread gets its
// own QSqlDatabase connection, as Qt requires. Mutating methods are expected
// to be called from the owner thread because they emit changed().
class ClipStore : public QObject {
    Q_OBJECT

public:
    enum class Filter : int { All = 0, Text, Links, Images, Files, Pinned };

    explicit ClipStore(QString databasePath, QObject* parent = nullptr);
    ~ClipStore() override;

    bool open();
    QString lastError() const { return m_lastError; }

    // Inserts a new clip or, if identical content already exists, moves it to
    // the top of the history. Returns the row id (0 on failure).
    qint64 upsert(const ClipItem& item);

    // Lightweight rows for the list (no image blobs).
    QList<ClipItem> query(const QString& search, Filter filter, int limit) const;

    std::optional<ClipItem> get(qint64 id) const;
    QByteArray imageData(qint64 id) const;

    bool setPinned(qint64 id, bool pinned);
    bool remove(qint64 id);
    bool markUsed(qint64 id);
    int clearUnpinned();
    int trim(int maxUnpinned);
    int count() const;

    bool hasAppIcon(const QString& bundleId) const;
    void saveAppIcon(const QString& bundleId, const QByteArray& png);
    QByteArray appIcon(const QString& bundleId) const;

signals:
    void changed();

private:
    QSqlDatabase db() const;
    QString connectionName() const;
    bool migrate();

    mutable QString m_lastError;
    const QString m_path;
    const QString m_connectionPrefix;
};
