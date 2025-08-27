#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "TCPHandler.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qmlRegisterType<TCPHandler>("communication", 1, 0, "TCPHandler");
    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("FCS", "Main");

    return app.exec();
}
