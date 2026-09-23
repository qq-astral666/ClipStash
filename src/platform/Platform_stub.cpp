#include "Platform.h"

#include <QStringList>

// Non-macOS fallback: the app still builds and runs (history, search, UI),
// but without a global hotkey or auto-paste.
namespace platform {

qint64 pasteboardChangeCount() { return -1; }
bool pasteboardIsConcealed() { return false; }
AppInfo frontmostApp() { return {}; }
QByteArray appIconPng(const QString&, int) { return {}; }
void rememberFrontmostApp() {}
void restorePreviousApp() {}
void activateSelf() {}
bool isAccessibilityTrusted(bool) { return false; }
void openAccessibilitySettings() {}
bool sendPasteKeystroke() { return false; }
bool registerGlobalHotkey(int, std::function<void()>) { return false; }

QStringList hotkeyPresets()
{
    return { QStringLiteral("Ctrl+Shift+V"), QStringLiteral("Alt+Space"), QStringLiteral("Ctrl+Alt+V") };
}

void prepareWindow(QWindow*, const QRect&, int, bool, bool) {}
void bringToFront(QWindow*) {}
bool launchAtLoginEnabled() { return false; }
bool setLaunchAtLogin(bool, QString* error)
{
    if (error)
        *error = QStringLiteral("Not supported on this platform");
    return false;
}

} // namespace platform
