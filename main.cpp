#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "src/parser.h"
#include <QFile>
#include <QDebug>
#include <QVector>
#include <QList>
#include <QMap>
#include <QQmlContext>

int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []()
        { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("gem5_visualizer", "Main");
    return app.exec();
}
