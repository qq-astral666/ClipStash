#include "AppController.h"
#include "ClipImageProvider.h"
#include "ClipStore.h"
#include "TrayIcon.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QLockFile>
#include <QQmlApplicationEngine>
#include <QStandardPaths>

int main(int argc, char* argv[])
{
    // QApplication (not QGuiApplication) because the menu-bar icon uses QMenu.
    QApplication app(argc, argv);
    QApplication::setApplicationName(QStringLiteral("ClipStash"));
    QApplication::setOrganizationName(QStringLiteral("ClipStash"));
    QApplication::setOrganizationDomain(QStringLiteral("clipstash.app"));
    QApplication::setApplicationVersion(QStringLiteral(PROJECT_VERSION_STRING));
    QApplication::setQuitOnLastWindowClosed(false);   // lives in the menu bar

    const QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dataDir);

    // Two instances would record every copy twice.
    QLockFile instanceLock(dataDir + QStringLiteral("/instance.lock"));
    if (!instanceLock.tryLock(100)) {
        qWarning() << "ClipStash is already running";
        return 0;
    }

    ClipStore store(dataDir + QStringLiteral("/history.sqlite"));
    if (!store.open()) {
        qCritical() << "ClipStash: cannot open database in" << dataDir;
        return 1;
    }

    AppController controller(&store);
    TrayIcon tray(&controller);

    QQmlApplicationEngine engine;
    engine.addImageProvider(QStringLiteral("clipstash"), new ClipImageProvider(&store));
    engine.setInitialProperties({ { QStringLiteral("controller"), QVariant::fromValue(&controller) } });
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
        [] { QCoreApplication::exit(1); }, Qt::QueuedConnection);
    engine.loadFromModule("ClipStash", "Main");

    controller.start();
    return app.exec();
}
