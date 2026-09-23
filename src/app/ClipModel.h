#pragma once

#include "ClipItem.h"

#include <QAbstractListModel>
#include <QtQml/qqmlregistration.h>

class ClipStore;

// List model for the QML history view. Re-queries SQLite on every change of
// the search text / filter / store; with a LIMIT of a few hundred rows this is
// well under a millisecond and keeps the model trivially correct.
class ClipModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("ClipModel is owned by AppController")

    Q_PROPERTY(QString searchText READ searchText WRITE setSearchText NOTIFY searchTextChanged)
    Q_PROPERTY(int filter READ filter WRITE setFilter NOTIFY filterChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    enum Role {
        ClipIdRole = Qt::UserRole + 1,
        ClipTypeRole,
        ClipTextRole,
        PreviewRole,
        SourceAppRole,
        SourceBundleRole,
        CreatedAtRole,
        PinnedRole,
        UseCountRole,
        ImageWidthRole,
        ImageHeightRole,
        CharCountRole,
        LineCountRole,
        SectionNameRole,
    };
    Q_ENUM(Role)

    explicit ClipModel(ClipStore* store, QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = {}) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    QString searchText() const { return m_search; }
    void setSearchText(const QString& text);

    int filter() const { return m_filter; }
    void setFilter(int filter);

    int count() const { return static_cast<int>(m_items.size()); }

    Q_INVOKABLE void reload();
    Q_INVOKABLE int indexOfId(qint64 id) const;

signals:
    void searchTextChanged();
    void filterChanged();
    void countChanged();

private:
    ClipStore* m_store;
    QList<ClipItem> m_items;
    QString m_search;
    int m_filter = 0;
    static constexpr int kLimit = 300;
};
