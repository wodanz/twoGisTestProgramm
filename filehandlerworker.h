#ifndef FILEHANDLERWORKER_H
#define FILEHANDLERWORKER_H

#include <QObject>
#include <QMutex>
#include "wordsFromFileStruct.h"
#include <QVector>
#include <QTimer>

class fileHandlerWorker : public QObject
{
    Q_OBJECT
public:
    explicit fileHandlerWorker(QObject *parent = nullptr);

private:
    // Члены класса
    QString fileName; // Имя файла
    long long allBytsInFile; // Общий размер файла в байтах
    long long currentByteInFile; // Текущая позиция в файле
    int currentPercent; // Текущий процент обработки файла
    int prevPercent; // Предыдущий процент обработки файла
    QVector<wordsFromFile> arrayOfWords; // Массив слов, извлеченных из файла
    QVector<wordsFromFile> top15; // 15 самых распространенных слов
    QTimer *handleTimer; // Таймер для обработки файла

    bool fileOpenFlag; // Флаг, указывающий, что файл открыт

    QString stringFromFile; // Строка, содержащая текст из файла
    QString modelString; // Строка, содержащая данные для модели
    QStringList wordsFromString; // Список слов, извлеченных из строки

    // Вспомогательные функции
    void addToArray(QString word); // Добавляет слово в массив arrayOfWords
    void updateTop(wordsFromFile word); // Обновляет массив top15 с учетом нового слова
    void sortTop(); // Сортирует массив top15 по частоте
    int completionPercentage(long long currentByte); // Вычисляет процент обработки файла
    void convertTopToString(QVector<wordsFromFile> &top); // Преобразует массив top15 в строку
    void resetValues(); // Сбрасывает значения переменных

    bool stopHandleFlag; // Флаг, указывающий, что обработка файла должна быть остановлена
    bool pauseHandleFlag; // Флаг, указывающий, что обработка файла должна быть приостановлена
    int globalIndex; // Индекс текущей обрабатываемой строки

private slots:
    void processNextStack(); // Слот для обработки следующего набора данных из файла

signals:
    void finished(bool); // Сигнал завершения работы потока
    void updateDataForinterface(int sbPercent, QString model); // Сигнал для обновления данных в интерфейсе
    void endProcess(); // Сигнал об окончании процесса обработки файла
    void openFileStateToIf(bool state); // Сигнал для интерфейса, указывающий, что файл открыт
    void setStartAndOpenEnableToIf(bool clearModel); // Сигнал для интерфейса, указывающий, что кнопки "Старт" и "Открыть" должны быть включены

public slots:
    void startfileHandle(); // Слот для запуска обработки файла
    void openFile(); // Слот для открытия файла
    void setSelectedFileName(QString filename); // Слот для установки имени файла
    void startHandle(); // Слот для начала обработки файла
    void stopHandleFromIf(); // Слот для остановки обработки файла
    void pauseHandleFromIf(); // Слот для приостановки обработки файла
};

#endif // FILEHANDLERWORKER_H
