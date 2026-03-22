import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RoundButton {
    id: controll
    required property string iconPath
    required property string buttonName

    Layout.preferredWidth: 112
    Layout.preferredHeight: 30

    text: buttonName
    icon.color: mainTheme.icon_color
    antialiasing: true
    font.bold: true
    flat: false
    icon.source: iconPath
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
