import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

TextField {
    id: roundedTextField
    width: 428
    height: 35
    Layout.fillWidth: true
    placeholderText: qsTr("Text Field")

    color: mainTheme.text_color

    background: Rectangle {
        implicitWidth: roundedTextField.width
        implicitHeight: roundedTextField.height
        border {
            color: mainTheme.field_border_color
            width: roundedTextField.visualFocus ? 2 : 1
        }
        radius: 10
        color: mainTheme.field_bacground_color
    }
}
