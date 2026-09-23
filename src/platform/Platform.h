#pragma once

#include <QByteArray>
#include <QRect>
#include <QString>
#include <QStringList>

#include <functional>

class QWindow;

// Thin OS abstraction. macOS implementation lives in Platform_mac.mm,
// everything else gets harmless no-ops from Platform_stub.cpp.
namespace platform {

struct AppInfo {
    QString name;
    QString bundleId;
};

// NSPasteboard.changeCount, or -1 when the OS offers no cheap change counter
// (then QClipboard::dataChanged is used instead).
qint64 pasteboardChangeCount();

// True when the current clipboard content is marked as a password / transient
// (nspasteboard.org conventions used by 1Password, Bitwarden, Keychain...).
bool pasteboardIsConcealed();

// Frontmost application other than ourselves.
AppInfo frontmostApp();
QByteArray appIconPng(const QString& bundleId, int pixelSize);

// Focus juggling for "paste into the app you came from".
void rememberFrontmostApp();
void restorePreviousApp();
void activateSelf();

// Synthetic Cmd+V. Requires the Accessibility permission.
bool isAccessibilityTrusted(bool promptUser);
void openAccessibilitySettings();
bool sendPasteKeystroke();

// System-wide hotkey. `preset` indexes hotkeyPresets().
bool registerGlobalHotkey(int preset, std::function<void()> callback);
QStringList hotkeyPresets();

// Native polish for the popup: joins all Spaces, floats over full-screen
// apps and optionally puts an NSVisualEffectView (frosted glass) behind the
// QML card described by `card` (in window coordinates).
void prepareWindow(QWindow* window, const QRect& card, int radius, bool dark, bool glass);

// Start at login via SMAppService (macOS 13+). On failure `error` explains why.
bool launchAtLoginEnabled();
bool setLaunchAtLogin(bool enable, QString* error);

// Orders the popup to the front and makes it key even when macOS refuses to
// activate the app (menu-bar agents on macOS 14+ often are refused).
void bringToFront(QWindow* window);

} // namespace platform
