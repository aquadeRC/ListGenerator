pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQuick.Dialogs

Dialog {
    id: settingsDialog
    title: "Ustawienia"
    width: 500
    height: 400
    margins: 20
    modal: true

    Rectangle {
        id: settingsRect
        anchors.fill: parent
        width: settingsDialog.width
        height: settingsDialog.height

        color: mainTheme.background_color

        anchors.margins: 10

        ColumnLayout {
            anchors.fill: parent
            spacing: 10

            RowLayout {
                x: 41
                y: 0
                width: 557
                height: 35
                spacing: 10
                Label {
                    id: label
                    x: 49
                    y: 49
                    color: mainTheme.text_color
                    text: qsTr("ID Szablonu")
                }
                RoundedTextField {
                    id: fieldIdSzablonu
                    x: 170
                    y: 37
                    width: 428
                    height: 35
                    Layout.fillHeight: false
                    Layout.fillWidth: true
                    placeholderText: qsTr("Text Field")
                }
            }

            RowLayout {
                x: 41
                y: 60
                width: 557
                height: 35
                Label {
                    id: label1
                    x: 49
                    y: 113
                    color: mainTheme.text_color
                    text: qsTr("ID skoroszytu Projekty")
                }
                RoundedTextField {
                    id: fieldIdProjekty
                    x: 170
                    y: 100
                    width: 428
                    height: 35
                    Layout.fillWidth: true
                    placeholderText: qsTr("Text Field")
                }
            }

            RowLayout {
                x: 41
                y: 120
                width: 557
                height: 35
                Label {
                    id: label2
                    x: 49
                    y: 175
                    color: mainTheme.text_color
                    text: qsTr("Id skoroszytu Dodatki")
                }
                RoundedTextField {
                    id: fieldIdDodatki
                    x: 170
                    y: 162
                    width: 428
                    height: 35
                    Layout.fillWidth: true
                    placeholderText: qsTr("Text Field")
                }
            }

            RowLayout {
                x: 41
                y: 150
                width: 557
                height: 35

                FileEdit {
                    id: fieldIdCertyfikat
                    y: 150
                    labelName: "Certyfikat"
                    Layout.fillWidth: true
                    itemText: ""
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
        }
    }

    footer: DialogButtonBox {

        alignment: Qt.AlignRight
        defaultButton: zatwierdzButton
        position: DialogButtonBox.Footer

        RoundButton {
            id: anulujButton
            x: 408
            y: 479
            // width: 107
            // height: 40
            text: "Anuluj"
            display: AbstractButton.TextOnly
            DialogButtonBox.buttonRole: DialogButtonBox.DestructiveRole

            // onClicked: reject()
        }

        RoundButton {
            id: zatwierdzButton
            x: 529
            y: 479
            // width: 107
            // height: 40
            text: "Zatwierdż"
            display: AbstractButton.TextOnly
            highlighted: true
            DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
            // onClicked: accept()
        }
    }

    onAccepted: {}

    Component.onCompleted: {
        let settings = backEnd.getSettings();

        fieldIdSzablonu.text = settings[2];
        fieldIdProjekty.text = settings[0];
        fieldIdDodatki.text = settings[1];
        fieldIdCertyfikat.itemText = settings[3];
    }
}
