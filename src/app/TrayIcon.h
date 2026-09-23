#pragma once

#include <QMenu>
#include <QObject>
#include <QSystemTrayIcon>

class AppController;
class QAction;

// Menu-bar icon (macOS template image, adapts to light/dark menu bar).
class TrayIcon : public QObject {
    Q_OBJECT

public:
    explicit TrayIcon(AppController* controller, QObject* parent = nullptr);

private:
    static QIcon makeIcon();
    void syncState();

    AppController* m_controller;
    QSystemTrayIcon m_tray;
    QMenu m_menu;
    QAction* m_openAction = nullptr;
    QAction* m_pauseAction = nullptr;
};
