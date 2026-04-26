import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

RoundButton {
    id: controll
    //required property string buttonName

    Layout.preferredWidth: 112
    Layout.preferredHeight: 30

   // text: buttonName
    antialiasing: true
    font.bold: true
    flat: false
    radius: 10

    background: Rectangle {
        color: controll.down ? mainTheme.button_color_down : mainTheme.button_color
        implicitWidth: 112
        implicitHeight: 30
        opacity: enabled ? 1 : 0.3
        border.color: controll.down ? mainTheme.button_color_down : mainTheme.button_color
        border.width: 1
        radius: 10
    }
}
