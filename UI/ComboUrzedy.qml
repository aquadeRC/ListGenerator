import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

ComboBox {
    id: control
    required property AbstractItemModel comboModel
    required property string backendProp
    required property string textRolenNme
    required property string valueRolenNme
    property bool higlight: false

    model: comboModel
    font: mainTheme.font

    textRole: control.textRolenNme
    valueRole: control.valueRolenNme
    width: 245
    height: 35

    // Set currentValue to the value stored in the backend.
    currentValue: backendProp
    // When an item is selected, update the backend.
    onActivated: backendProp = currentValue

    delegate: ItemDelegate {
        id: delegate

        required property var model
        required property int index

        width: control.width
        contentItem: Text {
            text: delegate.model[control.textRole]
            color: mainTheme.text_color
            font: control.font
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }
        highlighted: control.highlightedIndex === index
    }

    indicator: Canvas {
        id: canvas
        x: control.width - width - control.rightPadding
        y: control.topPadding + (control.availableHeight - height) / 2
        width: 12
        height: 8
        contextType: "2d"

        Connections {
            target: control
            function onPressedChanged() {
                canvas.requestPaint();
            }
        }

        onPaint: {
            context.reset();
            context.moveTo(0, 0);
            context.lineTo(width, 0);
            context.lineTo(width / 2, height);
            context.closePath();
            context.fillStyle = control.pressed ? Qt.darker(mainTheme.field_border_color) : mainTheme.field_border_color;
            context.fill();
        }
    }

    contentItem: Text {
        leftPadding: 10
        rightPadding: control.indicator.width + control.spacing

        text: control.displayText
        font: control.font
        color: mainTheme.text_color
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: control.width
        implicitHeight: control.height
        border {
            color: mainTheme.field_border_color
            width: control.visualFocus ? 2 : 1
        }
        radius: 10
        color: {
            control.higlight === false ? mainTheme.field_bacground_color : mainTheme.mode === "dark_mode" ? Qt.lighter(mainTheme.field_bacground_color) : Qt.darker(mainTheme.field_bacground_color);
        }
    }

    popup: Popup {
        y: control.height - 1
        width: control.width
        //  height: Math.min(contentItem.implicitHeight, control.Window.height - topMargin - bottomMargin)
        implicitHeight: contentItem.implicitHeight
        padding: 1

        contentItem: Rectangle {
            anchors.fill: parent

            implicitWidth: control.width
            implicitHeight: 23 + popUpView.implicitHeight

            border {
                color: mainTheme.field_border_color
                width: control.visualFocus ? 2 : 1
            }
            radius: 10
            color: {
                control.higlight === false ? mainTheme.field_bacground_color : mainTheme.mode === "dark_mode" ? Qt.lighter(mainTheme.field_bacground_color) : Qt.darker(mainTheme.field_bacground_color);
            }

            ColumnLayout {
                anchors.fill: parent

                TextField {
                    id: filterConditionText
                    Layout.fillWidth: true
                    height: 23
                    focus: false
                    leftPadding: 12

                    selectByMouse: true
                    hoverEnabled: true

                    wrapMode: TextEdit.WrapAnywhere
                    placeholderText: qsTr("Filter")
                    background: null

                    onTextEdited: {
                        if (filterConditionText.text.length >= 3) {
                            console.log(filterConditionText.text);
                            control.model.templatePattern = filterConditionText.text;
                        }
                    }
                }

                Rectangle {
                    //anchors.bottom: parent.bottom
                    //  anchors.left: parent.left
                    //   anchors.right: parent.right

                    anchors.bottomMargin: 12
                    height: 1
                    color: filterConditionText.activeFocus ? filterConditionText.selectionColor : mainTheme.background_color
                }

                ListView {
                    id: popUpView
                    clip: true
                    //anchors.left: parent.left
                    // anchors.right: parent.right
                    // anchors.top: filterConditionText.bottom
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    implicitHeight: contentHeight
                    model: control.popup.visible ? control.delegateModel : null
                    currentIndex: control.highlightedIndex

                    ScrollIndicator.vertical: ScrollIndicator {}

                    highlight: comboHighlight
                }
            }
        }

        background: Rectangle {
            border.color: mainTheme.field_border_color
            radius: 2
        }
    }

    Component {
        id: comboHighlight

        Rectangle {
            id: highlightRect

            radius: 10

            border.color: mainTheme.field_higlight_color
            color: mainTheme.field_higlight_color
            opacity: 0.15
            z: 3
        }
    }
}
