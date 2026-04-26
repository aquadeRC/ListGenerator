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
    property FilterModel urzadModel: backEnd.getModelUrzedy()
    property AbstractItemModel projektyModel: backEnd.getModelProjekt()
    property AbstractItemModel pracowniaModel: backEnd.getModelPracownia()

    property double headerHight: 79
    property double leftPanelW: 297

    property url curentDoc: "file:///pusty.pdf"
    property bool generujOdpowiedz: true

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

    /* QtObject {
        id: backend2
        property string architekt
        property string urzad
        property string inwestycja
        property string nr_sprawy
        property string inwestor
        property string pracownia
    }*/

    header: AppToolBar {
        id: toolBar
        height: 79
        width: parent.width
        visible: true
    }

    footer: Footer {
        id: footer
    }
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
                pracowniaModel: root.pracowniaModel
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
        function onIsAuthenticatedChanged(status: bool) {
            archModel = backEnd.getModelArchitekci();
            urzadModel = backEnd.getModelUrzedy();
            projektyModel = backEnd.getModelProjekt();
            pracowniaModel = backEnd.getModelPracownia();

            root.curentDoc = backEnd.getDocPdfPath();
            innerDoc.source = root.curentDoc;

            if (status === true) {
                footer.state = "connected";
                footer.message = "Połaczenie autoryzowane";
            } else if (status === false) {
                footer.state = "error";
                footer.message = "Brak autoryzacji";
            } else {
                footer.state = "idle";
            }
        }
    }

    Connections {
        target: backEnd
        function onIsError(data: string) {
            message_Dialog.text = "Error";
            message_Dialog.informativeText = data;
            footer.message = data;

            message_Dialog.open();
        }
    }

    Connections {
        target: backEnd
        function onSignalDocumentCreated(data: string) {
            message_Dialog.text = "Dokument wygenerowany.";
            let message = `Wygenerowano pismo :${data}`;
            message_Dialog.informativeText = message;
            footer.message = message;

            message_Dialog.open();
        }
    }
}
