import QtQuick
import QtQuick.Controls.Basic

CheckBox {
    id: control

    signal controlChecked(check: bool)
    indicator: Rectangle {
        implicitWidth: 26
        implicitHeight: 26
        x: control.leftPadding
        y: parent.height / 2 - height / 2
        radius: 3
        border.color: control.down ? mainTheme.text_color : Qt.lighter(mainTheme.text_color)

        Rectangle {
            width: 14
            height: 14
            x: 6
            y: 6
            radius: 2
            color: control.down ? mainTheme.text_color : Qt.lighter(mainTheme.text_color)
            visible: control.checked
        }
    }

    contentItem: Text {
        text: control.text
        font: control.font
        opacity: enabled ? 1.0 : 0.3
        color: mainTheme.text_color
        verticalAlignment: Text.AlignVCenter
        leftPadding: control.indicator.width + control.spacing
    }

    nextCheckState: function () {
        if (checkState === Qt.Checked) {
            controlChecked(false);
            return Qt.Unchecked;
        } else {
            controlChecked(true);
            return Qt.Checked;
        }
    }
}
