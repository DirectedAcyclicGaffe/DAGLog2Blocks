#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "SurfaceAndStyleSetup.h"
#include "QMLRegisterHelper.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    app.setOrganizationName("DAG");
    app.setOrganizationDomain("DAG.com");
    app.setApplicationName("DAG");

    registerQML(&app);
    setupSurfaceAndStyle();

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/QML/Malamute.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,&app, [url](QObject *obj, const QUrl &objUrl)
    {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    },
    Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
