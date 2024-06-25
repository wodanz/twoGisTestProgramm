#include "interfaceworker.h"
#include <QDebug>
#include <QThread>
#include <QQmlContext>

// Конструктор класса interfaceWorker
interfaceWorker::interfaceWorker(QObject *parent)
    : QObject{parent}
{
    setTimer();  // Вызываем приватный метод для установки таймера
}

// Приватный метод для установки таймера
void interfaceWorker::setTimer()
{
    updateTimer = new QTimer();  // Создаем новый таймер
    connect(updateTimer, &QTimer::timeout, this, &interfaceWorker::sendDataToQml);  // Подключаем сигнал таймера к слоту для отправки данных в QML
    updateTimer->setInterval(100);  // Устанавливаем интервал таймера в 100 мс
    updateTimer->setSingleShot(false);  // Настраиваем таймер на многократный запуск
    updateTimer->setTimerType(Qt::PreciseTimer);  // Устанавливаем точный тип таймера
}

// Публичный слот для запуска интерфейсного воркера
void interfaceWorker::startInterfaceWorker()
{
    // Реализация метода
}

// Публичный слот для открытия файла
void interfaceWorker::openFile(QString filename)
{
    emit(setFileName(filename));  // Отправляем сигнал с именем файла
    emit(openFileToHandle());  // Отправляем сигнал для открытия файла
}

// Публичный слот для запуска обработки данных из QML
void interfaceWorker::startHandleFromQml()
{
    emit(startHandleSignal());  // Отправляем сигнал для начала обработки данных
    updateTimer->start();  // Запускаем таймер для периодической отправки данных в QML
    stopTimerFlag = false;  // Сбрасываем флаг остановки таймера
}

// Публичный слот для отправки данных в QML
void interfaceWorker::sendDataToQml()
{
    emit(sendPercentToQml(percent / 100.0));  // Отправляем сигнал с процентом выполнения
    emit(sendTopToQml(modelForHistogram));  // Отправляем сигнал с моделью для гистограммы

    if (stopTimerFlag) {
        stopTimerFlag = false;
        updateTimer->stop();  // Останавливаем таймер, если установлен флаг остановки
    }
}

// Публичный слот для обновления данных от обработчика
void interfaceWorker::updateDataFromHandler(int m_percent, QString model)
{
    percent = m_percent;  // Обновляем процент выполнения
    modelForHistogram = model;  // Обновляем модель для гистограммы
}

// Публичный слот для остановки таймера
void interfaceWorker::stopTimer()
{
    stopTimerFlag = true;  // Устанавливаем флаг остановки таймера
}

// Публичный слот для установки состояния открытия файла
void interfaceWorker::openFileState(bool state)
{
    emit(openFileStateToQML(state));  // Отправляем сигнал с состоянием открытия файла
}

// Публичный слот для остановки обработки данных из QML
void interfaceWorker::stopHandleFromQml()
{
    emit(stopHandleForFh());  // Отправляем сигнал для остановки обработки данных
}

// Публичный слот для приостановки обработки данных из QML
void interfaceWorker::pauseHandleFromQml()
{
    emit(pauseHandleForFh());  // Отправляем сигнал для приостановки обработки данных
}

// Публичный слот для установки доступности кнопок из интерфейсного воркера
void interfaceWorker::setStartAndOpenEnableFromIf(bool clearModel)
{
    emit(setStartAndOpenEnableToQml(clearModel));  // Отправляем сигнал для установки доступности кнопок в QML
}
