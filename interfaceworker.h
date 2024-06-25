#ifndef INTERFACEWORKER_H
#define INTERFACEWORKER_H

#include <QObject>
#include <QMutex>
#include <QQmlApplicationEngine>
#include <QTimer>
#include <wordsFromFileStruct.h>

// Класс interfaceWorker наследуется от QObject, чтобы иметь возможность использовать сигналы и слоты
class interfaceWorker : public QObject
{
    Q_OBJECT  // Макрос, необходимый для использования сигналов и слотов в Qt

public:
    explicit interfaceWorker(QObject *parent = nullptr);  // Конструктор класса, принимающий указатель на родительский объект
    void sendData();  // Публичный метод для отправки данных в графический интерфейс

private:
    QTimer *updateTimer;  // Указатель на таймер для периодического обновления данных
    void setTimer();  // Приватный метод для установки таймера

    int percent;  // Переменная для хранения процента выполнения задачи
    QString modelForHistogram;  // Переменная для хранения модели для гистограммы

    bool stopTimerFlag;  // Флаг для остановки таймера

signals:
    // Сигналы, генерируемые классом для передачи данных в графический интерфейс
    void finished(bool);  // Сигнал завершения работы потока
    void openFileToHandle();  // Сигнал для открытия файла
    void setFileName(QString filename);  // Сигнал для установки имени файла
    void startHandleSignal();  // Сигнал для начала обработки данных
    void sendPercentToQml(double percent);  // Сигнал для отправки процента выполнения в QML
    void sendTopToQml(QString topString);  // Сигнал для отправки топа в QML
    void openFileStateToQML(bool state);  // Сигнал для отправки состояния открытия файла в QML
    void stopHandleForFh();  // Сигнал для остановки обработки данных
    void pauseHandleForFh();  // Сигнал для приостановки обработки данных
    void setStartAndOpenEnableToQml(bool clearModel);  // Сигнал для установки доступности кнопок в QML

public slots:
    // Слоты, обрабатывающие входящие команды от графического интерфейса
    void startInterfaceWorker();  // Слот для запуска интерфейсного воркера
    void openFile(QString filename);  // Слот для открытия файла
    void startHandleFromQml();  // Слот для запуска обработки данных из QML
    void sendDataToQml();  // Слот для отправки данных в QML
    void updateDataFromHandler(int m_percent, QString model);  // Слот для обновления данных от обработчика
    void stopTimer();  // Слот для остановки таймера
    void openFileState(bool state);  // Слот для установки состояния открытия файла
    void stopHandleFromQml();  // Слот для остановки обработки данных из QML
    void pauseHandleFromQml();  // Слот для приостановки обработки данных из QML
    void setStartAndOpenEnableFromIf(bool clearModel);  // Слот для установки доступности кнопок из интерфейсного воркера
};

#endif // INTERFACEWORKER_H
