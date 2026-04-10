pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

Dialog {
    id: settingsDialog
    title: "Ustawienia"
    width: 500
    height: 400

    standardButtons: StandardButton.Ok | StandardButton.Cancel

    Rectangle {
        id: settingsRect
        anchors.fill: parent
        width: settingsDialog.width
        height: settingsDialog.height

        color: mainTheme.background_color

        ColumnLayout {
            anchors.fill: parent
            spacing: 10
            RowLayout {
                x: 41
                y: 0
                width: 557
                height: 40
                spacing: 10
                Label {
                    id: label
                    x: 49
                    y: 49
                    text: qsTr("ID Szablonu")
                }
                TextField {
                    id: textField
                    x: 170
                    y: 37
                    width: 428
                    height: 40
                    Layout.fillHeight: false
                    Layout.fillWidth: true
                    placeholderText: qsTr("Text Field")
                }
            }

            RowLayout {
                x: 41
                y: 60
                width: 557
                height: 40
                Label {
                    id: label1
                    x: 49
                    y: 113
                    text: qsTr("ID skoroszytu Projekty")
                }
                TextField {
                    id: textField1
                    x: 170
                    y: 100
                    width: 428
                    height: 40
                    Layout.fillWidth: true
                    placeholderText: qsTr("Text Field")
                }
            }

            RowLayout {
                x: 41
                y: 120
                width: 557
                height: 40
                Label {
                    id: label2
                    x: 49
                    y: 175
                    text: qsTr("Id skoroszytu Dodatki")
                }
                TextField {
                    id: textField2
                    x: 170
                    y: 162
                    width: 428
                    height: 40
                    Layout.fillWidth: true
                    placeholderText: qsTr("Text Field")
                }
            }

            RowLayout {
                x: 41
                y: 179
                width: 557
                height: 40

                Label {
                    id: label3
                    x: 49
                    y: 238
                    text: qsTr("Certyfikat")
                }
                TextField {
                    id: textField3
                    x: 170
                    y: 225
                    width: 428
                    height: 40
                    Layout.fillWidth: true
                    placeholderText: qsTr("Text Field")
                }
            }

            Rectangle {
                id: rectangle2
                x: 59
                y: 479
                color: "#00ffffff"
                Layout.fillHeight: true
                Layout.fillWidth: false
            }

            RowLayout {
                x: 0
                y: 479
                width: 636
                height: 40
                uniformCellSizes: false
                layoutDirection: Qt.LeftToRight
                spacing: 10

                Rectangle {
                    id: rectangle
                    x: 59
                    y: 479
                    width: 200
                    height: 5
                    color: "#00ffffff"
                    Layout.fillWidth: true
                }

                RoundButton {
                    id: roundButton1
                    x: 408
                    y: 479
                    width: 107
                    height: 40
                    text: "Anuluj"
                    display: AbstractButton.TextOnly

                    onClicked: reject()
                }

                RoundButton {
                    id: roundButton
                    x: 529
                    y: 479
                    width: 107
                    height: 40
                    text: "Zatwierdż"
                    display: AbstractButton.TextOnly
                    highlighted: true

                    onClicked: accept()
                }
            }
        }
    }

    onAccepted: {}

    Component.onCompleted: {}
}
