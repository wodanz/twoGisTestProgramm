#include "filehandlerworker.h"
#include <QDebug>
#include <QThread>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QTimer>

fileHandlerWorker::fileHandlerWorker(QObject *parent)
    : QObject{parent}
{
    // Инициализация переменных
    prevPercent = 0;
    stringFromFile ="";
    fileName = "";
    fileOpenFlag = false;
    stopHandleFlag = false;
    pauseHandleFlag = false;
    globalIndex = 0;
    handleTimer = nullptr;
    currentByteInFile = 0;
}

void fileHandlerWorker::openFile()
{
    // Открытие файла
    QFile file(fileName);

    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        // Чтение содержимого файла
        stringFromFile = stream.readAll();

        file.close();

        fileOpenFlag = true;
        resetValues();

    } else {
        fileOpenFlag = false;
    }
    emit(openFileStateToIf(fileOpenFlag));

    // Вывод списка самых распространенных слов
    for(int i=0; i<top15.size(); i++) {
    }
}

void fileHandlerWorker::setSelectedFileName(QString filename)
{
    // Установка имени файла
    fileName = filename;
}

void fileHandlerWorker::startHandle()
{
    // Подготовка к обработке файла
    stopHandleFlag = false;
    pauseHandleFlag = false;

    allBytsInFile = stringFromFile.length();

    if(!handleTimer) {
        // Создание таймера для обработки файла
        handleTimer = new QTimer(this);
        handleTimer->setInterval(7);
        connect(handleTimer, &QTimer::timeout, this, &fileHandlerWorker::processNextStack);
    }
    handleTimer->start();
}

void fileHandlerWorker::stopHandleFromIf()
{
    // Установка флага для остановки обработки файла
    stopHandleFlag = true;
}

void fileHandlerWorker::pauseHandleFromIf()
{
    // Установка флага для приостановки обработки файла
    pauseHandleFlag = true;
}

void fileHandlerWorker::addToArray(QString word)
{
    // Добавление слова в массив arrayOfWords и обновление массива top15
    if(arrayOfWords.size() != 0) {
        for(int i=0; i<arrayOfWords.size(); i++) {
            if(arrayOfWords[i].word == word) {
                arrayOfWords[i].numberOfSimilar ++;
                updateTop(arrayOfWords[i]);
                return;
            }
        }
        wordsFromFile tempWord;
        tempWord.word = word;
        tempWord.numberOfSimilar = 1;
        arrayOfWords.append(tempWord);
        updateTop(tempWord);
    } else {
        wordsFromFile tempWord;
        tempWord.word = word;
        tempWord.numberOfSimilar = 1;
        arrayOfWords.append(tempWord);
        updateTop(tempWord);
    }
}

void fileHandlerWorker::updateTop(wordsFromFile word)
{
    // Обновление массива top15 с учетом нового слова
    bool findMin = false;
    int position = 0;
    if(top15.size() != 0) {
        for(int i=0; i<top15.size(); i++) {
            if(top15[i].word == word.word) {
                top15[i].numberOfSimilar = word.numberOfSimilar;
                sortTop();
                return;
            } else if(!findMin && (top15[i].numberOfSimilar < word.numberOfSimilar)) {
                position = i;
                findMin = true;
            }
        }
        if(findMin) {
            top15.insert(position, word);
        } else
            top15.append(word);
    } else {
        top15.append(word);
    }

    sortTop();
    if(top15.size() >15) {
        top15.resize(15);
    }
}

void fileHandlerWorker::sortTop()
{
    // Сортировка массива top15 по частоте
    int size = top15.size()-1;
    for(int i=0; i<size; i++) {
        for(int j=0; j<size-i; j++) {
            if(top15[j].numberOfSimilar < top15[j+1].numberOfSimilar) {
                qSwap(top15[j], top15[j+1]);
            }
        }
    }
}

int fileHandlerWorker::completionPercentage(long long currentByte)
{
    // Вычисление процента обработки файла
    return (currentByte * 100)/allBytsInFile;
}

void fileHandlerWorker::convertTopToString(QVector<wordsFromFile> &top)
{
    // Преобразование массива top15 в строку
    modelString = "";
    for(int i=0; i<top.size(); i++) {
        modelString += top[i].word + " " + QString::number(top[i].numberOfSimilar) + " ";
    }
}

void fileHandlerWorker::resetValues()
{
    // Сбрасывание значений переменных
    globalIndex = 0;
    top15.clear();
    currentByteInFile  = 0;
    modelString = "";
    arrayOfWords.clear();
}

void fileHandlerWorker::processNextStack()
{
    // Обработка следующего набора данных из файла
    QString currentWord = "";
    const int stackSize = 10000;

    for(int i=globalIndex; i < globalIndex + stackSize && i < stringFromFile.length(); i++) {
        currentByteInFile ++;
        currentPercent = completionPercentage(currentByteInFile);
        if(!stopHandleFlag && !pauseHandleFlag) {
            if(prevPercent != currentPercent) {
                prevPercent = currentPercent;
            }
            QChar ch = stringFromFile[i];
            if(ch.isLetter()) {
                currentWord += ch;
            } else {
                if(!currentWord.isEmpty()) {
                    wordsFromString.append(currentWord);
                    addToArray(currentWord);
                    currentWord.clear();
                    convertTopToString(top15);
                    emit(updateDataForinterface(currentPercent, modelString));
                }
            }
        } else if(stopHandleFlag) {
            emit(endProcess());
            emit(setStartAndOpenEnableToIf(true));
            resetValues();
            return;
        } else if(pauseHandleFlag) {
            emit(endProcess());
            emit(setStartAndOpenEnableToIf(false));
            return;
        }
    }
    if(!currentWord.isEmpty()) {
        wordsFromString.append(currentWord);
        addToArray(currentWord);
    }

    if (globalIndex >= stringFromFile.length()) {
        handleTimer->stop();
        convertTopToString(top15);
        emit(updateDataForinterface(100, modelString));
        emit(endProcess());
    } else {
        globalIndex += stackSize;
    }
}

void fileHandlerWorker::startfileHandle()
{
    // Запуск обработки файла
    QCoreApplication::processEvents();
}
