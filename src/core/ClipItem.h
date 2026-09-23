#pragma once

#include <QByteArray>
#include <QDateTime>
#include <QString>

// Kind of content captured from the clipboard. Values are persisted in SQLite,
// so never reorder — only append.
enum class ClipType : int {
    Text = 0,
    Link = 1,
    Image = 2,
    Color = 3,
    File = 4,
};

struct ClipItem {
    qint64 id = 0;
    ClipType type = ClipType::Text;
    QString text;          // text, URL, hex color or newline-separated file paths
    QByteArray imagePng;   // only for ClipType::Image (loaded on demand)
    int imageWidth = 0;
    int imageHeight = 0;
    QByteArray hash;       // content fingerprint used for de-duplication
    QString sourceApp;     // human-readable name of the app it was copied from
    QString sourceBundle;  // bundle id of that app (macOS)
    QDateTime createdAt;
    bool pinned = false;
    int useCount = 0;
};

namespace clip {

// Detects links and hex colors in plain text.
ClipType classifyText(const QString& text);

QByteArray textHash(const QString& text);
QByteArray fileHash(const QString& paths);
QByteArray imageHash(const QByteArray& png);

// Lower-cased (Unicode-aware) haystack used for search. SQLite's LIKE only
// folds ASCII, so we pre-fold Cyrillic and everything else in C++.
QString searchHaystack(const ClipItem& item);

} // namespace clip
