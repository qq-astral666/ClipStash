#pragma once

#include "ClipItem.h"
#include "ClipModel.h"

#include <QObject>
#include <QRect>
#include <QRectF>
#include <QStringList>
#include <QVariant>
#include <QWindow>
#include <QtQml/qqmlregistration.h>

class ClipStore;
class ClipboardWatcher;

// Glue between the store, the clipboard, the OS and the QML UI.
// Exposed to QML as the `controller` initial property of Main.qml.
class AppController : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("AppController is created in main.cpp")

    Q_PROPERTY(ClipModel* model READ model CONSTANT)
    Q_PROPERTY(bool isMac READ isMac CONSTANT)
    Q_PROPERTY(QStringList hotkeyOptions READ hotkeyOptions CONSTANT)
    Q_PROPERTY(bool windowVisible READ windowVisible WRITE setWindowVisible NOTIFY windowVisibleChanged)
    Q_PROPERTY(int totalCount READ totalCount NOTIFY totalCountChanged)
    Q_PROPERTY(bool accessibilityTrusted READ accessibilityTrusted NOTIFY accessibilityTrustedChanged)

    // Settings (persisted in QSettings)
    Q_PROPERTY(bool paused READ paused WRITE setPaused NOTIFY pausedChanged)
    Q_PROPERTY(int hotkeyPreset READ hotkeyPreset WRITE setHotkeyPreset NOTIFY settingsChanged)
    Q_PROPERTY(QString hotkeyText READ hotkeyText NOTIFY settingsChanged)
    Q_PROPERTY(int maxItems READ maxItems WRITE setMaxItems NOTIFY settingsChanged)
    Q_PROPERTY(bool pasteOnSelect READ pasteOnSelect WRITE setPasteOnSelect NOTIFY settingsChanged)
    Q_PROPERTY(int themeMode READ themeMode WRITE setThemeMode NOTIFY settingsChanged)
    Q_PROPERTY(bool ignoreConcealed READ ignoreConcealed WRITE setIgnoreConcealed NOTIFY settingsChanged)
    Q_PROPERTY(bool vibrancy READ vibrancy WRITE setVibrancy NOTIFY settingsChanged)
    Q_PROPERTY(bool launchAtLogin READ launchAtLogin WRITE setLaunchAtLogin NOTIFY launchAtLoginChanged)

public:
    explicit AppController(ClipStore* store, QObject* parent = nullptr);
    ~AppController() override;

    // Registers the hotkey and starts watching the clipboard.
    void start();

    ClipModel* model() const { return m_model; }
    bool isMac() const;
    QStringList hotkeyOptions() const;
    bool windowVisible() const { return m_windowVisible; }
    void setWindowVisible(bool visible);
    int totalCount() const;
    bool accessibilityTrusted() const { return m_accessibilityTrusted; }

    bool paused() const { return m_paused; }
    void setPaused(bool paused);
    int hotkeyPreset() const { return m_hotkeyPreset; }
    void setHotkeyPreset(int preset);
    QString hotkeyText() const;
    int maxItems() const { return m_maxItems; }
    void setMaxItems(int value);
    bool pasteOnSelect() const { return m_pasteOnSelect; }
    void setPasteOnSelect(bool value);
    int themeMode() const { return m_themeMode; }
    void setThemeMode(int mode);
    bool ignoreConcealed() const { return m_ignoreConcealed; }
    void setIgnoreConcealed(bool value);
    bool vibrancy() const { return m_vibrancy; }
    void setVibrancy(bool value);
    bool launchAtLogin() const;
    void setLaunchAtLogin(bool enable);

    Q_INVOKABLE void showWindow();
    Q_INVOKABLE void hideWindow();
    Q_INVOKABLE void toggleWindow();
    Q_INVOKABLE void openSettings();

    // Puts the clip on the clipboard and (optionally) pastes it into the app
    // that was frontmost before the popup opened.
    Q_INVOKABLE void activate(qint64 id, bool paste);
    Q_INVOKABLE void togglePin(qint64 id);
    Q_INVOKABLE void remove(qint64 id);
    Q_INVOKABLE void clearHistory();
    Q_INVOKABLE void openUrl(const QString& url);

    Q_INVOKABLE QRect targetScreenGeometry() const;
    Q_INVOKABLE void prepareWindow(QWindow* window, const QRectF& card, int radius, bool dark);
    Q_INVOKABLE void bringToFront(QWindow* window);

    Q_INVOKABLE void requestAccessibility();
    Q_INVOKABLE void refreshAccessibility();
    Q_INVOKABLE void quit();

signals:
    void showRequested();
    void hideRequested();
    void settingsRequested();
    void toast(const QString& message);
    void systemNotification(const QString& title, const QString& message);

    void windowVisibleChanged();
    void totalCountChanged();
    void accessibilityTrustedChanged();
    void pausedChanged();
    void settingsChanged();
    void launchAtLoginChanged();

private:
    void onCaptured(const ClipItem& item);
    void writeToClipboard(const ClipItem& item);
    void loadSettings();
    void saveSetting(const QString& key, const QVariant& value);
    void registerHotkey();

    ClipStore* m_store;
    ClipModel* m_model;
    ClipboardWatcher* m_watcher;

    bool m_windowVisible = false;
    bool m_accessibilityTrusted = false;

    bool m_paused = false;
    int m_hotkeyPreset = 0;
    int m_maxItems = 500;
    bool m_pasteOnSelect = true;
    int m_themeMode = 0;   // 0 = system, 1 = dark, 2 = light
    bool m_ignoreConcealed = true;
    bool m_vibrancy = true;
};
