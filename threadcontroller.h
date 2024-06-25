#ifndef THREADCONTROLLER_H
#define THREADCONTROLLER_H

#include <QObject>
#include "interfaceworker.h"
#include "filehandlerworker.h"
#include <QThread>
#include <QQmlApplicationEngine>


/*
 * Класс контроллера потоков, в нём создаются исполнители и потоки их выполнения
 * головной класс для управления потоками.
 */
class threadController : public QObject
{
    Q_OBJECT
public:
    explicit threadController(QObject *parent = nullptr);
    void makeInterfaceThread();          // Метод перемещения исполненителей в поток и их запуск
    void makeFileHandlerThread();
    interfaceWorker *ifWorker;

private:
    fileHandlerWorker *fhWorker;

    QThread *threadInterfaceUpdate;
    QThread *threadFileHandler;
    QThread *threadOpenFileHandler;

signals:
public slots:
    void openFile();
};

#endif // THREADCONTROLLER_H
