import QtQuick

import QtQuick.Controls.Basic

Switch {
    id: control

    property color kolorTekstu: mainTheme.button_text_color  //"#737c8e"
    property string switchImage: control.checked ? "icons/ksiezyc.svg" : "icons/sloneczko.svg"

    text: qsTr("Switch")

    indicator: Rectangle {
        id: indicatorRect
        implicitWidth: 48
        implicitHeight: 26
        x: control.leftPadding
        y: parent.height / 2 - height / 2
        radius: 13
        color: control.checked ? mainTheme.button_color : "#ffffff"
        border.color: control.checked ? mainTheme.button_color : "#cccccc"

        Image {
            id: image
            x: control.checked ? 5 : parent.width - width - 5
            width: 12
            height: 12
            anchors.verticalCenter: indicatorRect.verticalCenter
            source: control.switchImage
            fillMode: Image.PreserveAspectFit
        }

        Rectangle {
            x: control.checked ? parent.width - width : 0
            width: 26
            height: 26
            radius: 13
            color: control.down ? "#cccccc" : mainTheme.switch_indicator_color
            border.color: control.checked ? mainTheme.button_color : "#999999"
        }
    }

    contentItem: Text {
        color: control.kolorTekstu
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        verticalAlignment: Text.AlignVCenter
        leftPadding: control.indicator.width + control.spacing
    }

    Component.onCompleted: {
        if (settings.state == "light_mode") {
            control.checked = false;
        } else {
            control.checked = true;
        }
    }
}
