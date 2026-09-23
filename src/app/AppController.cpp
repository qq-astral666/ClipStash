#include "AppController.h"

#include "ClipStore.h"
#include "ClipboardWatcher.h"
#include "Platform.h"

#include <QClipboard>
#include <QCoreApplication>
#include <QCursor>
#include <QDebug>
#include <QDesktopServices>
#include <QGuiApplication>
#include <QImage>
#include <QMimeData>
#include <QScreen>
#include <QSettings>
#include <QTimer>
#include <QUrl>
#include <QWindow>

namespace {

// Time for the previous app to regain key focus before we send Cmd+V.
constexpr int kPasteDelayMs = 120;
constexpr int kAppIconPx = 64;

namespace keys {
const QString hotkey = QStringLiteral("hotkeyPreset");
const QString maxItems = QStringLiteral("maxItems");
const QString pasteOnSelect = QStringLiteral("pasteOnSelect");
const QString theme = QStringLiteral("themeMode");
const QString ignoreConcealed = QStringLiteral("ignoreConcealed");
const QString vibrancy = QStringLiteral("vibrancy");
const QString firstRunDone = QStringLiteral("firstRunDone");
} // namespace keys

} // namespace

AppController::AppController(ClipStore* store, QObject* parent)
    : QObject(parent)
    , m_store(store)
    , m_model(new ClipModel(store, this))
    , m_watcher(new ClipboardWatcher(this))
{
    loadSettings();
    m_watcher->setIgnoreConcealed(m_ignoreConcealed);
    m_accessibilityTrusted = platform::isAccessibilityTrusted(false);

    connect(m_watcher, &ClipboardWatcher::captured, this, &AppController::onCaptured);
    connect(m_store, &ClipStore::changed, this, &AppController::totalCountChanged);
}

AppController::~AppController() = default;

void AppController::start()
{
    registerHotkey();
    m_watcher->start();

    QSettings settings;
    if (!settings.value(keys::firstRunDone, false).toBool()) {
        settings.setValue(keys::firstRunDone, true);
        emit systemNotification(QStringLiteral("ClipStash запущен"),
                                QStringLiteral("Открывай историю буфера по %1").arg(hotkeyText()));
        QTimer::singleShot(400, this, &AppController::showWindow);
    }
}

bool AppController::isMac() const
{
#ifdef Q_OS_MACOS
    return true;
#else
    return false;
#endif
}

QStringList AppController::hotkeyOptions() const
{
    return platform::hotkeyPresets();
}

QString AppController::hotkeyText() const
{
    const QStringList presets = platform::hotkeyPresets();
    return presets.value(m_hotkeyPreset, presets.value(0));
}

void AppController::setWindowVisible(bool visible)
{
    if (visible == m_windowVisible)
        return;
    m_windowVisible = visible;
    emit windowVisibleChanged();
}

int AppController::totalCount() const
{
    return m_store->count();
}

// ---------------------------------------------------------------- window ---

void AppController::showWindow()
{
    qInfo() << "ClipStash: show window";
    platform::rememberFrontmostApp();
    refreshAccessibility();
    m_model->reload();            // refresh relative timestamps / sections
    platform::activateSelf();
    emit showRequested();
}

void AppController::hideWindow()
{
    emit hideRequested();
    platform::restorePreviousApp();
}

void AppController::toggleWindow()
{
    if (m_windowVisible)
        hideWindow();
    else
        showWindow();
}

void AppController::openSettings()
{
    if (!m_windowVisible)
        showWindow();
    emit settingsRequested();
}

QRect AppController::targetScreenGeometry() const
{
    QScreen* screen = QGuiApplication::screenAt(QCursor::pos());
    if (!screen)
        screen = QGuiApplication::primaryScreen();
    return screen ? screen->availableGeometry() : QRect(0, 0, 1440, 900);
}

void AppController::prepareWindow(QWindow* window, const QRectF& card, int radius, bool dark)
{
    platform::prepareWindow(window, card.toRect(), radius, dark, m_vibrancy);
}

void AppController::bringToFront(QWindow* window)
{
    platform::bringToFront(window);
}

// --------------------------------------------------------------- actions ---

void AppController::activate(qint64 id, bool paste)
{
    const std::optional<ClipItem> item = m_store->get(id);
    if (!item)
        return;

    writeToClipboard(*item);
    m_watcher->syncAfterOwnWrite();
    m_store->markUsed(id);

    emit hideRequested();
    platform::restorePreviousApp();

    if (!paste || !m_pasteOnSelect || !isMac())
        return;

    if (!platform::isAccessibilityTrusted(false)) {
        emit systemNotification(QStringLiteral("Скопировано в буфер"),
                                QStringLiteral("Разреши ClipStash «Универсальный доступ», чтобы вставлять автоматически."));
        platform::isAccessibilityTrusted(true);   // shows the system prompt once
        return;
    }

    QTimer::singleShot(kPasteDelayMs, this, [] { platform::sendPasteKeystroke(); });
}

void AppController::togglePin(qint64 id)
{
    const std::optional<ClipItem> item = m_store->get(id);
    if (!item)
        return;
    m_store->setPinned(id, !item->pinned);
    emit toast(item->pinned ? QStringLiteral("Откреплено") : QStringLiteral("Закреплено"));
}

void AppController::remove(qint64 id)
{
    if (m_store->remove(id))
        emit toast(QStringLiteral("Удалено"));
}

void AppController::clearHistory()
{
    const int n = m_store->clearUnpinned();
    emit toast(QStringLiteral("Очищено записей: %1").arg(n));
}

void AppController::openUrl(const QString& url)
{
    QDesktopServices::openUrl(QUrl::fromUserInput(url.trimmed()));
    hideWindow();
}

void AppController::requestAccessibility()
{
    if (!platform::isAccessibilityTrusted(true))
        platform::openAccessibilitySettings();
    refreshAccessibility();
}

void AppController::refreshAccessibility()
{
    const bool trusted = platform::isAccessibilityTrusted(false);
    if (trusted == m_accessibilityTrusted)
        return;
    m_accessibilityTrusted = trusted;
    emit accessibilityTrustedChanged();
}

void AppController::quit()
{
    QCoreApplication::quit();
}

// -------------------------------------------------------------- internals ---

void AppController::onCaptured(const ClipItem& item)
{
    qInfo().noquote() << "ClipStash: captured type" << static_cast<int>(item.type) << "from" << item.sourceApp;
    if (!item.sourceBundle.isEmpty() && !m_store->hasAppIcon(item.sourceBundle)) {
        const QByteArray png = platform::appIconPng(item.sourceBundle, kAppIconPx);
        if (!png.isEmpty())
            m_store->saveAppIcon(item.sourceBundle, png);
    }
    m_store->upsert(item);
    m_store->trim(m_maxItems);
}

void AppController::writeToClipboard(const ClipItem& item)
{
    QClipboard* clipboard = QGuiApplication::clipboard();
    switch (item.type) {
    case ClipType::Image: {
        QImage image;
        image.loadFromData(item.imagePng, "PNG");
        clipboard->setImage(image);
        break;
    }
    case ClipType::File: {
        auto* mime = new QMimeData;
        QList<QUrl> urls;
        const QStringList paths = item.text.split(QLatin1Char('\n'), Qt::SkipEmptyParts);
        for (const QString& p : paths)
            urls << QUrl::fromLocalFile(p);
        mime->setUrls(urls);
        mime->setText(item.text);
        clipboard->setMimeData(mime);   // clipboard takes ownership
        break;
    }
    default:
        clipboard->setText(item.text);
        break;
    }
}

void AppController::registerHotkey()
{
    if (!platform::registerGlobalHotkey(m_hotkeyPreset, [this] { toggleWindow(); }) && isMac())
        qWarning() << "ClipStash: failed to register hotkey" << hotkeyText();
}

// --------------------------------------------------------------- settings ---

void AppController::loadSettings()
{
    QSettings s;
    m_hotkeyPreset = qBound(0, s.value(keys::hotkey, 0).toInt(), int(platform::hotkeyPresets().size()) - 1);
    m_maxItems = qBound(50, s.value(keys::maxItems, 500).toInt(), 10000);
    m_pasteOnSelect = s.value(keys::pasteOnSelect, true).toBool();
    m_themeMode = qBound(0, s.value(keys::theme, 0).toInt(), 2);
    m_ignoreConcealed = s.value(keys::ignoreConcealed, true).toBool();
    m_vibrancy = s.value(keys::vibrancy, true).toBool();
}

void AppController::saveSetting(const QString& key, const QVariant& value)
{
    QSettings().setValue(key, value);
    emit settingsChanged();
}

void AppController::setPaused(bool paused)
{
    if (paused == m_paused)
        return;
    m_paused = paused;
    m_watcher->setPaused(paused);
    emit pausedChanged();
    emit toast(paused ? QStringLiteral("Запись на паузе") : QStringLiteral("Запись возобновлена"));
}

void AppController::setHotkeyPreset(int preset)
{
    if (preset == m_hotkeyPreset || preset < 0 || preset >= platform::hotkeyPresets().size())
        return;
    m_hotkeyPreset = preset;
    registerHotkey();
    saveSetting(keys::hotkey, preset);
}

void AppController::setMaxItems(int value)
{
    if (value == m_maxItems)
        return;
    m_maxItems = value;
    m_store->trim(value);
    saveSetting(keys::maxItems, value);
}

void AppController::setPasteOnSelect(bool value)
{
    if (value == m_pasteOnSelect)
        return;
    m_pasteOnSelect = value;
    saveSetting(keys::pasteOnSelect, value);
}

void AppController::setThemeMode(int mode)
{
    if (mode == m_themeMode)
        return;
    m_themeMode = mode;
    saveSetting(keys::theme, mode);
}

void AppController::setIgnoreConcealed(bool value)
{
    if (value == m_ignoreConcealed)
        return;
    m_ignoreConcealed = value;
    m_watcher->setIgnoreConcealed(value);
    saveSetting(keys::ignoreConcealed, value);
}

void AppController::setVibrancy(bool value)
{
    if (value == m_vibrancy)
        return;
    m_vibrancy = value;
    saveSetting(keys::vibrancy, value);
}

bool AppController::launchAtLogin() const
{
    return platform::launchAtLoginEnabled();
}

void AppController::setLaunchAtLogin(bool enable)
{
    QString error;
    const bool ok = platform::setLaunchAtLogin(enable, &error);
    if (!error.isEmpty())
        emit toast(error);
    else if (ok)
        emit toast(enable ? QStringLiteral("ClipStash будет запускаться при входе")
                          : QStringLiteral("Автозапуск выключен"));
    emit launchAtLoginChanged();   // re-read the real state from the system
}
