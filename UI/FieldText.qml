import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

Item {
    id: textField

    required property string fieldLabel
    property alias fieldtext: textInput.text

    width: 263
    height: 58

    ColumnLayout {
        spacing: 9
        anchors.fill: textField
        anchors.margins: 6

        Label {
            id: field_Text
            color: mainTheme.text_color
            text: textField.fieldLabel
            font.pixelSize: 12
            verticalAlignment: Text.AlignVCenter
        }

        RoundedTextField {
            id: textInput
            width: 245
            color: mainTheme.text_color
            font.pixelSize: 12
            verticalAlignment: Text.AlignVCenter
        }
    }
}
