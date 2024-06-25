import QtQuick 2.13
import QtQuick.Dialogs 1.3
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.3
import QtQml.Models 2.13

// Главное окно приложения
ApplicationWindow {
    id: window
    width: 1000
    height: 600
    visible: true
    color: "#fefffb"
    title: qsTr("Hello World")

    // Флаг для очистки гистограммы
    property bool clearFlag: false

    // Диалог выбора файла
    FileDialog {
        id: openFileDialog
        selectExisting: true
        nameFilters: ["Text (*.txt)"]
        onAccepted: {
            // Отправляем путь к файлу в интерфейсный класс
            interfaceWorker.openFile(fileUrl.toString().replace("file:///",""))
        }
    }

    // Подключение сигналов от интерфейсного класса
    Connections {
        target: interfaceWorker
        onSendPercentToQml: progressBar.value = percent;
        onSendTopToQml: {
            // Обновляем данные гистограммы
            updateHistogramData(topString);
        }
        onOpenFileStateToQML: {
            // Изменяем состояние кнопки "Открыть" в зависимости от состояния файла
            startButton.enabled = state;
        }
        onSetStartAndOpenEnableToQml: {
            // Изменяем состояние кнопок "Открыть" и "Старт" и устанавливаем флаг очистки гистограммы
            startButton.enabled = true;
            openButton.enabled = true;
            clearFlag = clearModel;
        }
    }

    // Главная колонка макета
    ColumnLayout {
        id: mainLayout
        anchors.fill: parent
        anchors.topMargin: 10
        spacing: 20

        // Прямоугольник для отображения гистограммы
        Rectangle {
            id: histogrammRect
            Layout.fillWidth: true
            Layout.preferredHeight: 300
            Layout.alignment: Qt.AlignTop
            color: "#fefffb"

            // Колонка для размещения столбцов гистограммы
            Column {
                id: histogramColumn
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10

                // Повторяющийся элемент для создания столбцов гистограммы
                Repeater {
                    id: histogramRepeater
                    model: mainLayout.histogramModel
                    Item {
                        property real scaleFactor: (histogrammRect.height - 100) / (15* 15)
                        width: histogrammRect.width * (value / firstValue)-100
                        height: (histogrammRect.height / 15) * scaleFactor

                        // Прямоугольник для заливки столбца цветом
                        Rectangle {
                            anchors.left: parent.left
                            width: parent.width
                            height: parent.height
                            color: "#76b56d"
                            radius: 5
                        }

                        // Текст для отображения значения столбца
                        Text {
                            anchors.right: parent.right
                            anchors.rightMargin: -60
                            anchors.verticalCenter: parent.verticalCenter
                            text: label + " " + (value > 0 ? value.toFixed(0) + " повт." : "")
                            color: "#333333"
                            font.family: "Times New Roman"
                            font.pixelSize: 14
                            width: 50
                        }
                    }
                }
            }
        }

        // Индикатор прогресса
        ProgressBar {
            id: progressBar
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            Layout.leftMargin: 10
            Layout.rightMargin: 10
            height: 60

            // Фон индикатора прогресса
            background: Rectangle {
                implicitWidth: parent.width
                implicitHeight: parent.height
                color: "#fefffb"
                radius: 5
            }

            // Содержимое индикатора прогресса
            contentItem: Item {
                implicitWidth: parent.width
                implicitHeight: parent.height

                // Заполненная часть индикатора прогресса
                Rectangle {
                    id: progressBarFill
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    height: parent.height * 0.99
                    color: "#76b56d"
                    radius: 5
                    width: progressBar.value * parent.width
                    Behavior on width {
                        NumberAnimation {
                            duration: 500
                            easing.type: Easing.Linear
                        }
                    }
                }

                // Текст для отображения значения индикатора прогресса
                Text {
                    anchors.centerIn: parent
                    text: Math.round(progressBar.value * 100) + "%"
                    font.family: "Times New Roman"
                    font.pixelSize: 14
                    color: "#333333"
                }
            }

            value: 0.0
        }

        // Строка кнопок
        RowLayout {
            Layout.fillWidth: true
            Layout.preferredHeight: 40
            Layout.leftMargin: 10
            Layout.rightMargin: 10
            spacing: 20

            // Кнопка "Открыть"
            Button {
                id: openButton
                text: qsTr("Открыть")
                font.family: "Times New Roman"
                font.pixelSize: 14
                Layout.fillWidth: true
                background: Rectangle {
                    color: openButton.pressed ? "#afaa90" : "#ddd6b2"
                    radius: 5
                }
                onClicked: openFileDialog.open()
            }

            // Кнопка "Старт"
            Button {
                id: startButton
                text: qsTr("Старт")
                font.family: "Times New Roman"
                font.pixelSize: 14
                Layout.fillWidth: true
                enabled: false
                background: Rectangle {
                    color: startButton.pressed ? "#afaa90" : "#ddd6b2"
                    radius: 5
                }
                onClicked: {
                    // Если установлен флаг очистки гистограммы, то очищаем гистограмму
                    if (clearFlag) clearHistogram()
                    // Отправляем сигнал на старт интерфейсному классу
                    interfaceWorker.startHandleFromQml()
                    // Изменяем состояние кнопок
                    openButton.enabled = false
                    startButton.enabled = false
                    pauseButton.enabled = true
                    cancelButton.enabled = true
                }
            }


        // Кнопка "Пауза"
                Button {
                    id: pauseButton
                    text: qsTr("Пауза")
                    font.family: "Times New Roman"
                    font.pixelSize: 14
                    Layout.fillWidth: true
                    enabled: false
                    background: Rectangle {
                        color: pauseButton.pressed ? "#afaa90" : "#ddd6b2"
                        radius: 5
                    }
                    onClicked: interfaceWorker.pauseHandleFromQml()
                }

                // Кнопка "Отмена"
                Button {
                    id: cancelButton
                    text: qsTr("Отмена")
                    font.family: "Times New Roman"
                    font.pixelSize: 14
                    Layout.fillWidth: true
                    enabled: false
                    background: Rectangle {
                        color: cancelButton.pressed ? "#afaa90" : "#ddd6b2"
                        radius: 5
                    }
                    onClicked: {
                        // Отправляем сигнал на стоп в интерфейсный класс
                        interfaceWorker.stopHandleFromQml()
                        // Изменяем состояние кнопок
                        openButton.enabled = true
                        startButton.enabled = false
                        pauseButton.enabled = false
                        cancelButton.enabled = false
                        // Обнуляем значение индикатора прогресса
                        progressBar.value = 0.0
                        // Очищаем гистограмму
                        mainLayout.histogramModel.clear()
                    }
                }
            }

            // Модель данных гистограммы
            property var histogramModel: ListModel {
                id: histogramModel
            }
        }

        // Функция для обновления данных гистограммы
        function updateHistogramData(dataString) {
            // Очищаем модель данных гистограммы
            mainLayout.histogramModel.clear()
            var parts = dataString.split(" ")
            var firstValue = parseInt(parts[1]);
            for (var i = 0; i < parts.length; i += 2) {
                // Добавляем новые данные в модель
                mainLayout.histogramModel.append({
                    "label": parts[i],
                    "value": parseInt(parts[i + 1]),
                    "firstValue": firstValue
                })
            }
            for (var i = 0; i < mainLayout.histogramModel.count; i++) {
                var item = histogramRepeater.itemAt(i);
                if (item) {
                    // Изменяем ширину столбцов гистограммы в зависимости от их значений
                    item.width = histogrammRect.width * (mainLayout.histogramModel.get(i).value / firstValue)-100;
                }
            }
        }

        // Функция для очистки гистограммы
        function clearHistogram() {
            // Очищаем модель данных гистограммы
            mainLayout.histogramModel.clear();
        }
}
