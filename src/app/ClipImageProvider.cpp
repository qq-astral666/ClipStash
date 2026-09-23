#include "ClipImageProvider.h"

#include "ClipStore.h"

#include <QMutexLocker>

namespace {
constexpr int kCacheBudgetKiB = 64 * 1024;   // 64 MiB of decoded pixels
}

ClipImageProvider::ClipImageProvider(const ClipStore* store)
    : QQuickImageProvider(QQuickImageProvider::Image)
    , m_store(store)
    , m_cache(kCacheBudgetKiB)
{
}

QImage ClipImageProvider::requestImage(const QString& id, QSize* size, const QSize& requestedSize)
{
    const QString key = QStringLiteral("%1@%2x%3").arg(id).arg(requestedSize.width()).arg(requestedSize.height());
    {
        QMutexLocker lock(&m_mutex);
        if (const QImage* cached = m_cache.object(key)) {
            if (size)
                *size = cached->size();
            return *cached;
        }
    }

    QByteArray bytes;
    if (id.startsWith(QLatin1String("clip/")))
        bytes = m_store->imageData(id.mid(5).toLongLong());
    else if (id.startsWith(QLatin1String("app/")))
        bytes = m_store->appIcon(id.mid(4));

    QImage image;
    if (bytes.isEmpty() || !image.loadFromData(bytes)) {
        if (size)
            *size = {};
        return {};
    }

    const int rw = requestedSize.width();
    const int rh = requestedSize.height();
    if (rw > 0 && rh > 0) {
        if (image.width() > rw || image.height() > rh)
            image = image.scaled(rw, rh, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    } else if (rw > 0 && image.width() > rw) {
        image = image.scaledToWidth(rw, Qt::SmoothTransformation);
    } else if (rh > 0 && image.height() > rh) {
        image = image.scaledToHeight(rh, Qt::SmoothTransformation);
    }

    if (size)
        *size = image.size();

    {
        QMutexLocker lock(&m_mutex);
        const auto cost = static_cast<qsizetype>(qMax<qint64>(1, image.sizeInBytes() / 1024));
        m_cache.insert(key, new QImage(image), cost);
    }
    return image;
}
