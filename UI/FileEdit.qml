pragma ComponentBehavior: Bound

import QtCore
import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQuick.Dialogs

Item {
    id: rootFEdit
    required property string labelName
    property alias itemText: textField.text
    width: 35

    signal filePatchChenged(path: string)

    RowLayout {
        anchors.fill: parent
        spacing: 10

        Label {
            id: label
            x: 49
            y: 49
            height: 35
            color: mainTheme.text_color
            text: qsTr(rootFEdit.labelName)
        }
        RoundedTextField {
            id: textField
            x: 170
            y: 37
            Layout.fillWidth: true
            //width: 428
            height: 35
            placeholderText: qsTr("Podaj scieżke do pliku.")

            onTextChanged: filePatchChenged(filePatchChenged.text)
        }

        RoundedButton {
            buttonName: "Wybierz plik..."
            onClicked: fileDialog.open()
            height: 35
        }

        FileDialog {
            id: fileDialog
            currentFolder: StandardPaths.standardLocations(StandardPaths.ApplicationsLocation)[0]
            onAccepted: textField.text = selectedFile
        }
    }
}
