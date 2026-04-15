import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

Item {
    id: comboFieldN

    required property variant comboModel
    required property string backendProp
    required property string fieldText
    property bool higlight: false

    signal comboFieldChanged(aName: string, index: int)

    width: 263
    height: 58

    ColumnLayout {
        spacing: 9
        anchors.fill: comboFieldN
        anchors.margins: 6

        Text {
            id: field_Text
            color: mainTheme.text_color
            text: comboFieldN.fieldText
            font.pixelSize: 12
            verticalAlignment: Text.AlignVCenter
        }

        ComboN {
            id: fieldComboN
            comboModel: comboFieldN.comboModel
            backendProp: comboFieldN.backendProp
            higlight: comboFieldN.higlight

            onActivated: {
                comboFieldChanged(fieldComboN.backendProp, fieldComboN.currentIndex);
            }
        }
    }
}
