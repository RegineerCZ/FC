#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "common.h"
#include "renderprovider.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    common c;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("com", &c);
    engine.addImageProvider(QLatin1String("renderProvider"), new renderProvider);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
