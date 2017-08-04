#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "http.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<http>("HTTP", 1, 0, "Http");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
