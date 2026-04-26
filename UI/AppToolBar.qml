pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

Rectangle {
    id: toolBar
    y: 0
    x: 0
    visible: true
    width: parent.width
    height: 79

    color: mainTheme.background_color
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.top: parent.top
    anchors.left: parent.left

    RowLayout {
        anchors.fill: toolBar
        anchors.rightMargin: 8
        layoutDirection: Qt.LeftToRight
        spacing: 4

        Rectangle {
            id: mazwaBox
            x: 0
            y: 0
            width: 298
            height: toolBar.height
            Layout.fillHeight: true
            color: "#737c8e"

            Text {
                id: text1
                x: 23
                color: "#ffffff"
                text: qsTr("Wygeneruj pismo")
                anchors.verticalCenter: parent.verticalCenter
                font {
                    pixelSize: 18
                    family: mainTheme.font
                    bold: true
                }
            }
        }

        Rectangle {
            id: dummyBox
            color: "#00000000"
            border.color: Qt.color("transparent")
            Layout.fillWidth: true
        }

        IconButton {
            id: ustawieniaButton
            text: qsTr("Ustawienia")
            iconPath: "icons/ustawienia.svg"
            onClicked: {
                settingsDialog.open();
            }
        }

        /*
        IconButton {
            id: exportPdfButton
            buttonName: qsTr("Eksport PDF")
            iconPath: "icons/ustawienia.svg"
        }
        */

        ThemeSwitch {
            id: motywSW
            Layout.preferredWidth: 112
            Layout.preferredHeight: 30

            text: qsTr("Motyw")
            onCheckedChanged: {
                motywSW.checked ? mainTheme.setDarkMode() : mainTheme.setLightMode();
            }
        }
    }
}
