import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

ToolBar {
    id: footer_toolBar
    height: footerRow.implicitHeight
    state: "idle"
    property string message: ""

    RowLayout {
        id: footerRow
        anchors.fill: parent
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        ToolButton {
            id: statusIndicatror
            icon.source: Qt.resolvedUrl("icons/yellow-light.png")
            icon.color: "transparent"
            ToolTip.visible: enabled && hovered
            ToolTip.delay: 2000
            ToolTip.text: "Status: rozłączony"
            display: AbstractButton.IconOnly
        }
        TextField {
            id: messageField
            placeholderText: "logs"
            Layout.minimumWidth: 150
            Layout.fillWidth: true
            readOnly: true
            text: footer_toolBar.message
            background: Rectangle {
                anchors.fill: parent
                width: parent.width
                height: parent.height
                color: mainTheme.background_color
            }
        }
    }
    states: [
        State {
            name: "idle"
            PropertyChanges {
                target: statusIndicatror
                icon.source: Qt.resolvedUrl("icons/yellow-light.png")
            }
        },
        State {
            name: "connected"
            PropertyChanges {
                target: statusIndicatror
                icon.source: Qt.resolvedUrl("icons/green-light.png")
            }
        },
        State {
            name: "error"
            PropertyChanges {
                target: statusIndicatror
                icon.source: Qt.resolvedUrl("icons/red-light.png")
            }
        }
    ]
    transitions: [
        Transition {
            PropertyAnimation {
                duration: 210
                properties: statusIndicatror.icon.source
            }
        }
    ]
}
