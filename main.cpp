#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "threadcontroller.h"
#include <QDebug>
#include <QThread>
#include <QTextCodec>
#include <QQmlContext>
#include <QQuickItem>


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QCoreApplication::setOrganizationName("none");
    QCoreApplication::setOrganizationDomain("none");

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);


    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    threadController *controller = new threadController();
    QQmlContext *context = engine.rootContext();
    context->setContextProperty("interfaceWorker", controller->ifWorker);

    //controller->makeInterfaceThread();
    controller->makeFileHandlerThread();

    engine.load(url);

    return app.exec();
}
