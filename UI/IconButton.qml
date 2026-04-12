import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

RoundedButton {
    id: controll
    required property string iconPath

    icon.color: mainTheme.icon_color
    icon.source: iconPath
}
