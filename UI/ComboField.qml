import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

Item {
    id: comboField

    required property AbstractItemModel comboModel
    //required property string backendProp
    required property string fieldText
    required property string valueRolenNme
    required property string textRolenNme
    property bool higlight: false
    property alias curentFieldValue: fieldCombo.currentValue
    property alias curentFieldIndex: fieldCombo.currentIndex

    signal comboFieldChanged(aName: string, index: int)

    width: 263
    height: 58

    ColumnLayout {
        spacing: 9
        anchors.fill: comboField
        anchors.margins: 6

        Text {
            id: field_Text
            color: mainTheme.text_color
            text: comboField.fieldText
            font.pixelSize: 12
            verticalAlignment: Text.AlignVCenter
        }

        Combo {
            id: fieldCombo
            comboModel: comboField.comboModel
            //backendProp: comboField.backendProp
            textRolenNme: comboField.textRolenNme
            valueRolenNme: comboField.valueRolenNme
            higlight: comboField.higlight

            onActivated: {
                comboFieldChanged(fieldCombo.currentValue, fieldCombo.currentIndex);
            }
        }
    }
}
