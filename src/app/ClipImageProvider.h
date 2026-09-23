#pragma once

#include <QCache>
#include <QMutex>
#include <QQuickImageProvider>

class ClipStore;

// Serves images to QML:
//   image://clipstash/clip/<id>        — copied image (thumbnail or preview)
//   image://clipstash/app/<bundleId>   — icon of the source application
//
// Runs on QML's image loader threads (Image { asynchronous: true }), which is
// why ClipStore keeps one SQLite connection per thread.
class ClipImageProvider : public QQuickImageProvider {
public:
    explicit ClipImageProvider(const ClipStore* store);

    QImage requestImage(const QString& id, QSize* size, const QSize& requestedSize) override;

private:
    const ClipStore* m_store;
    QMutex m_mutex;
    QCache<QString, QImage> m_cache;   // cost in KiB
};
