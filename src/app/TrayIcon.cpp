#include "TrayIcon.h"

#include "AppController.h"

#include <QAction>
#include <QPainter>
#include <QPainterPath>
#include <QPixmap>

TrayIcon::TrayIcon(AppController* controller, QObject* parent)
    : QObject(parent)
    , m_controller(controller)
{
    m_openAction = m_menu.addAction(QString(), m_controller, &AppController::showWindow);
    m_menu.addSeparator();

    m_pauseAction = m_menu.addAction(QStringLiteral("Пауза записи"));
    m_pauseAction->setCheckable(true);
    connect(m_pauseAction, &QAction::toggled, m_controller, &AppController::setPaused);

    m_menu.addAction(QStringLiteral("Очистить историю"), m_controller, &AppController::clearHistory);
    m_menu.addSeparator();
    m_menu.addAction(QStringLiteral("Настройки…"), m_controller, &AppController::openSettings);
    m_menu.addAction(QStringLiteral("Выйти из ClipStash"), m_controller, &AppController::quit);

    connect(m_controller, &AppController::pausedChanged, this, &TrayIcon::syncState);
    connect(m_controller, &AppController::settingsChanged, this, &TrayIcon::syncState);
    connect(m_controller, &AppController::systemNotification, this,
            [this](const QString& title, const QString& message) {
                m_tray.showMessage(title, message, QSystemTrayIcon::Information, 4000);
            });

    m_tray.setIcon(makeIcon());
    m_tray.setToolTip(QStringLiteral("ClipStash"));
    m_tray.setContextMenu(&m_menu);
    syncState();
    m_tray.show();
}

void TrayIcon::syncState()
{
    m_openAction->setText(QStringLiteral("Открыть историю    %1").arg(m_controller->hotkeyText()));
    const QSignalBlocker blocker(m_pauseAction);
    m_pauseAction->setChecked(m_controller->paused());
}

QIcon TrayIcon::makeIcon()
{
    // Drawn in code: a clipboard glyph at 18pt, rendered @2x. Marked as a
    // mask so macOS tints it for light/dark menu bars.
    constexpr qreal dpr = 2.0;
    QPixmap pm(QSize(18, 18) * dpr);
    pm.setDevicePixelRatio(dpr);
    pm.fill(Qt::transparent);

    QPainter p(&pm);
    p.setRenderHint(QPainter::Antialiasing);
    QPen pen(Qt::black, 1.5);
    pen.setJoinStyle(Qt::RoundJoin);
    p.setPen(pen);
    p.setBrush(Qt::NoBrush);
    p.drawRoundedRect(QRectF(3.25, 3.5, 11.5, 13.0), 2.5, 2.5);

    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(QRectF(6.0, 1.75, 6.0, 3.5), 1.2, 1.2);

    // Two "text lines"
    p.drawRoundedRect(QRectF(6.0, 8.5, 6.0, 1.4), 0.7, 0.7);
    p.drawRoundedRect(QRectF(6.0, 11.5, 4.0, 1.4), 0.7, 0.7);
    p.end();

    QIcon icon(pm);
    icon.setIsMask(true);
    return icon;
}
