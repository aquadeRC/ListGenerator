import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

Item {
    id: comboFieldN

    required property variant comboModel
    //required property string backendProp
    required property string fieldText
    property bool higlight: false

    property alias curentFieldValue: fieldComboN.currentValue
    property alias curentFieldIndex: fieldComboN.currentIndex

    signal comboFieldChanged(aName: string, index: int)
    signal updateClicked

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
        RowLayout {
            ComboN {
                id: fieldComboN
                comboModel: comboFieldN.comboModel
                //backendProp: comboFieldN.backendProp
                higlight: comboFieldN.higlight

                onActivated: {
                    comboFieldChanged(fieldComboN.currentValue, fieldComboN.currentIndex);
                }
            }

            ToolButton {
                id: updateSprawaModel
                icon.source: Qt.resolvedUrl("icons/update.svg")
                ToolTip.visible: enabled && hovered
                ToolTip.delay: 2000
                ToolTip.text: "Odswież"
                display: AbstractButton.IconOnly
                Layout.alignment: Qt.AlignBottom

                background: Rectangle {
                    color: updateSprawaModel.down ? mainTheme.button_color_down : mainTheme.button_color
                    implicitHeight: 30
                    opacity: enabled ? 1 : 0.3
                    border.color: updateSprawaModel.down ? mainTheme.button_color_down : mainTheme.button_color
                    border.width: 1
                    radius: 10
                }
                onClicked: {
                    comboFieldN.updateClicked();
                }
            }
        }
    }
}
