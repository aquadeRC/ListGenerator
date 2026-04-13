pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQuick.Dialogs
import QtCore
import LG_GUI
import Kernel

ApplicationWindow {
    id: root
    height: 900
    width: 1080
    color: "#F7F8FA"

    required property Kernel backEnd
    property AbstractItemModel archModel: backEnd.getModelArchitekci()
    property AbstractItemModel urzadModel: backEnd.getModelUrzedy()
    property AbstractItemModel projektyModel: backEnd.getModelProjekt()

    property double headerHight: 79
    property double leftPanelW: 297

    visible: true
    title: qsTr("Generuj pismo")

    AppTheme {
        id: mainTheme
    }

    BusyIndicator {
        visible: !backEnd.isAuthenticated
        running: !backEnd.isAuthenticated
        anchors.centerIn: parent
    }

    QtObject {
        id: backend2
        property string architekt
        property string urzad
        property string inwestycja
        property string nr_sprawy
        property string inwestor
    }

    header: AppToolBar {
        id: toolBar
        height: 79
        width: parent.width
        visible: true
    }

    /*
    footer: Footer {
        id: footer
        anchors.fill: parent
    }
*/
    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            LeftPanel {
                id: leftPanel
                width: root.leftPanelW
                Layout.fillHeight: true
                visible: true
                architekciModel: root.archModel
                urzedyModel: root.urzadModel
                projektyModel: root.projektyModel
            }

            TextArea {
                id: document
                property string initDataStr

                width: root.width - root.leftPanelW
                Layout.fillHeight: true
                visible: true
                textFormat: TextEdit.RichText
            }
        }
    }

    Settings {
        id: settings
        property string state: mainTheme.mode
    }

    Ustawienia {
        id: settingsDialog
    }

    Component.onCompleted: {
        if (settings.state == "light_mode") {
            mainTheme.setLightMode();
        } else {
            mainTheme.setDarkMode();
        }

        backEnd.authenticate();
    }

    Component.onDestruction: {
        settings.state = mainTheme.mode;
    }

    Connections {
        target: backEnd
        function onIsAuthenticatedChanged() {
            archModel = backEnd.getModelArchitekci();
            urzadModel = backEnd.getModelUrzedy();
            projektyModel = backEnd.getModelProjekt();

            let doc = JSON.stringify(backEnd.getDoc(), null, '\t');

            document.text = "<pre style='color:red'>" + doc + "</pre>";
        }
    }
}
