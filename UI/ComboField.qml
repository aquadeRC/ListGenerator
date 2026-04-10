import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: comboField

    required property AbstractItemModel comboModel
    required property string backendProp
    required property string fieldText

    required property string textRolenNme
    required property string valueRolenNme

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
            backendProp: comboField.backendProp
            textRolenNme: comboField.textRolenNme
            valueRolenNme: comboField.valueRolenNme

            onActivated: {
                comboFieldChanged(fieldCombo.backendProp, fieldCombo.currentIndex);
            }
        }
    }

    Component.onCompleted: {
        // console.log(" Combofield", comboField.textRolenNme, comboField.valueRolenNme);
    }
}
