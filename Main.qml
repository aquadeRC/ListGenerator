pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Pdf
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

    property url curentDoc: "file:///D:/sandbox/ListGenerator/build/Desktop_Qt_6_11_0_MSVC2022_64bit-Debug/mapa.pdf"

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

            PdfPageView {
                id: pdfView
                property alias dUrl: innerDoc.source
                property alias inner_Doc: innerDoc

                width: root.width - root.leftPanelW
                Layout.fillHeight: true
                visible: true

                Component.onCompleted: scaleToWidth(width, height)

                document: PdfDocument {
                    id: innerDoc
                    source: Qt.resolvedUrl(root.curentDoc)
                }
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

    MessageDialog {
        id: message_Dialog
        buttons: MessageDialog.Ok
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

            root.curentDoc = backEnd.getDoc();
            innerDoc.source = root.curentDoc;
        }
    }

    Connections {
        target: backEnd
        function onIsError(data: string) {
            message_Dialog.text = "Error";
            message_Dialog.informativeText = data;
        }
    }
}
