#include "threadcontroller.h"
#include <QDebug>

// Конструктор класса threadController
threadController::threadController(QObject *parent)
    : QObject{parent}
{
    // Создаем экземпляры классов interfaceWorker и fileHandlerWorker
    ifWorker = new interfaceWorker();
    fhWorker = new fileHandlerWorker();

    // Создаем новые потоки для интерфейсного воркера, обработчика файла и открытия файла
    threadInterfaceUpdate = new QThread();
    threadFileHandler = new QThread();
    threadOpenFileHandler = new QThread();

    // Через сигналы и слоты создаем управление состояниями потоков
    // При условии внешнего управления питанием все потоки будут завершаться выключением питания,
    // поэтому их уничтожение программно не потребуется

    // Подключаем сигнал запуска потока к слоту запуска интерфейсного воркера
    connect(threadInterfaceUpdate, &QThread::started, ifWorker, &interfaceWorker::startInterfaceWorker);
    // Подключаем сигнал запуска потока к слоту запуска обработчика файла
    connect(threadFileHandler, &QThread::started, fhWorker, &fileHandlerWorker::startfileHandle);
    // Подключаем сигнал запуска потока к слоту открытия файла обработчиком
    connect(threadOpenFileHandler, &QThread::started, fhWorker, &fileHandlerWorker::openFile);

    // Обрабатываем сигналы завершения работы интерфейсного воркера и обработчика файла
    connect(ifWorker, &interfaceWorker::finished, this, [](bool state){
        if (state)
            qDebug() << "Thread interface ended work";
        else
            qDebug() << "Thread interface failed to work";
    });
    connect(fhWorker, &fileHandlerWorker::finished, this, [](bool state){
        if (state)
            qDebug() << "Thread handler ended work";
        else
            qDebug() << "Thread handler failed to work";
    });

    // Подключаем сигналы завершения работы к слотам остановки потоков
    connect(ifWorker, &interfaceWorker::finished, threadInterfaceUpdate, &QThread::quit);
    connect(fhWorker, &fileHandlerWorker::finished, threadFileHandler, &QThread::quit);
    connect(fhWorker, &fileHandlerWorker::finished, threadOpenFileHandler, &QThread::quit);

    // Подключаем сигналы завершения работы к слотам удаления объектов
    connect(ifWorker, &interfaceWorker::finished, ifWorker, &QObject::deleteLater);
    connect(fhWorker, &fileHandlerWorker::finished, fhWorker, &QObject::deleteLater);
    connect(threadInterfaceUpdate, &QThread::finished, threadInterfaceUpdate, &QObject::deleteLater);
    connect(threadFileHandler, &QThread::finished, threadFileHandler, &QObject::deleteLater);
    connect(threadOpenFileHandler, &QThread::finished, threadOpenFileHandler, &QObject::deleteLater);

    // Подключаем сигналы и слоты для взаимодействия между интерфейсным воркером и обработчиком файла
    connect(ifWorker, &interfaceWorker::setFileName, fhWorker, &fileHandlerWorker::setSelectedFileName);
    connect(ifWorker, &interfaceWorker::openFileToHandle, fhWorker, &fileHandlerWorker::openFile, Qt::QueuedConnection);
    connect(ifWorker, &interfaceWorker::startHandleSignal, fhWorker, &fileHandlerWorker::startHandle, Qt::QueuedConnection);
    connect(fhWorker, &fileHandlerWorker::updateDataForinterface, ifWorker, &interfaceWorker::updateDataFromHandler, Qt::QueuedConnection);
    connect(fhWorker, &fileHandlerWorker::endProcess, ifWorker, &interfaceWorker::stopTimer, Qt::QueuedConnection);
    connect(fhWorker, &fileHandlerWorker::openFileStateToIf, ifWorker, &interfaceWorker::openFileState, Qt::QueuedConnection);
    connect(ifWorker, &interfaceWorker::stopHandleForFh, fhWorker, &fileHandlerWorker::stopHandleFromIf, Qt::QueuedConnection);
    connect(ifWorker, &interfaceWorker::pauseHandleForFh, fhWorker, &fileHandlerWorker::pauseHandleFromIf, Qt::QueuedConnection);
    connect(fhWorker, &fileHandlerWorker::setStartAndOpenEnableToIf, ifWorker, &interfaceWorker::setStartAndOpenEnableFromIf, Qt::QueuedConnection);
}

// Публичный метод для создания потока интерфейсного воркера
void threadController::makeInterfaceThread()
{
    ifWorker->moveToThread(threadInterfaceUpdate);
    threadInterfaceUpdate->start();
}

// Публичный метод для создания потока обработчика файла
void threadController::makeFileHandlerThread()
{
    fhWorker->moveToThread(threadFileHandler);
    threadFileHandler->start();
}

// Публичный метод для создания потока открытия файла
void threadController::openFile()
{
    fhWorker->moveToThread(threadOpenFileHandler);
    threadOpenFileHandler->start();
}
