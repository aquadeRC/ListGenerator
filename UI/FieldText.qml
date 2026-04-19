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

        RowLayout {
            RoundedTextField {
                id: textInput
                width: 245
                color: mainTheme.text_color
                font.pixelSize: 12
                verticalAlignment: Text.AlignVCenter
                Layout.fillWidth: true
            }
            ToolButton {
                id: showLargeBox
                icon.source: Qt.resolvedUrl("icons/ustawienia.svg")
                icon.color: "transparent"
                ToolTip.visible: enabled && hovered
                ToolTip.delay: 2000
                ToolTip.text: "Pokaż wiecej..."
                display: AbstractButton.IconOnly
                onClicked: {
                    tArea.text = textInput.text;
                    editorPopup.open();
                }
            }
        }
    }

    Popup {
        id: editorPopup
        y: textField.y
        x: textInput.x + 5
        width: 481
        height: 400
        implicitHeight: contentItem.implicitHeight
        padding: 1
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
        focus: true
        contentItem: Rectangle {
            id: popupRect
            radius: 5
            border.color: mainTheme.field_border_color
            color: mainTheme.field_bacground_color
            anchors.fill: parent
            //implicitHeight: viewRect.implicitHeight

            ColumnLayout {
                spacing: 9
                anchors.fill: popupRect
                anchors.margins: 6

                ScrollView {
                    id: view
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    TextArea {
                        id: tArea
                        color: mainTheme.text_color
                    }
                }

                RowLayout {
                    Rectangle {
                        height: 10
                        Layout.fillWidth: true
                        color: "#00ffffff"
                    }
                    RoundedButton {
                        id: closPopup
                        buttonName: "Zamknij"
                        onClicked: {
                            textInput.text = tArea.text;
                            editorPopup.close();
                        }
                    }
                }
            }
        }
    }
}
