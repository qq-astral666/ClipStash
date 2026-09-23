#include "ClipboardWatcher.h"

#include "Platform.h"

#include <QBuffer>
#include <QClipboard>
#include <QGuiApplication>
#include <QImage>
#include <QMimeData>
#include <QUrl>

namespace {

constexpr int kPollIntervalMs = 350;
constexpr qsizetype kMaxTextChars = 2'000'000;   // ~2 MB of text, larger is skipped
constexpr int kMaxImageSide = 4096;              // huge screenshots are downscaled

QByteArray encodePng(const QImage& image)
{
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    return bytes;
}

} // namespace

ClipboardWatcher::ClipboardWatcher(QObject* parent)
    : QObject(parent)
{
    m_timer.setInterval(kPollIntervalMs);
    connect(&m_timer, &QTimer::timeout, this, &ClipboardWatcher::poll);
}

void ClipboardWatcher::start()
{
    m_lastChangeCount = platform::pasteboardChangeCount();
    m_usePolling = m_lastChangeCount >= 0;
    if (m_usePolling) {
        m_timer.start();
    } else {
        connect(QGuiApplication::clipboard(), &QClipboard::dataChanged,
                this, &ClipboardWatcher::onClipboardSignal, Qt::UniqueConnection);
    }
}

void ClipboardWatcher::syncAfterOwnWrite()
{
    if (m_usePolling)
        m_lastChangeCount = platform::pasteboardChangeCount();
    else
        m_ignoreNextSignal = true;
}

void ClipboardWatcher::poll()
{
    const qint64 current = platform::pasteboardChangeCount();
    if (current == m_lastChangeCount)
        return;
    m_lastChangeCount = current;
    capture();
}

void ClipboardWatcher::onClipboardSignal()
{
    if (m_ignoreNextSignal) {
        m_ignoreNextSignal = false;
        return;
    }
    capture();
}

void ClipboardWatcher::capture()
{
    if (m_paused)
        return;
    if (m_ignoreConcealed && platform::pasteboardIsConcealed())
        return;

    const QMimeData* mime = QGuiApplication::clipboard()->mimeData(QClipboard::Clipboard);
    if (!mime)
        return;

    ClipItem item;
    const platform::AppInfo app = platform::frontmostApp();
    item.sourceApp = app.name;
    item.sourceBundle = app.bundleId;
    item.createdAt = QDateTime::currentDateTime();

    // Priority: files -> text -> image. Office apps put a rendered picture
    // next to the text of copied cells; the user almost always wants the text.
    if (mime->hasUrls()) {
        QStringList paths;
        const QList<QUrl> urls = mime->urls();
        for (const QUrl& url : urls) {
            if (!url.isLocalFile()) {
                paths.clear();
                break;
            }
            paths << url.toLocalFile();
        }
        if (!paths.isEmpty()) {
            item.type = ClipType::File;
            item.text = paths.join(QLatin1Char('\n'));
            item.hash = clip::fileHash(item.text);
            emit captured(item);
            return;
        }
    }

    if (mime->hasText()) {
        const QString text = mime->text();
        if (!text.trimmed().isEmpty() && text.size() <= kMaxTextChars) {
            item.type = clip::classifyText(text);
            item.text = text;
            item.hash = clip::textHash(text);
            emit captured(item);
            return;
        }
    }

    if (mime->hasImage()) {
        QImage image = qvariant_cast<QImage>(mime->imageData());
        if (image.isNull())
            return;
        if (image.width() > kMaxImageSide || image.height() > kMaxImageSide)
            image = image.scaled(kMaxImageSide, kMaxImageSide, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        item.type = ClipType::Image;
        item.imagePng = encodePng(image);
        item.imageWidth = image.width();
        item.imageHeight = image.height();
        item.hash = clip::imageHash(item.imagePng);
        emit captured(item);
    }
}
