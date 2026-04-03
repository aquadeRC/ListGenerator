pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls.Windows
import QtQuick.Layouts
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

    property double headerHight: 79
    property double leftPanelW: 297

    visible: true
    title: qsTr("Generuj pismo")

    AppTheme {
        id: mainTheme
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

    // footer: Footer {
    //    id: footer
    //}

    LeftPanel {
        width: root.leftPanelW
        height: root.height - root.headerHight
        visible: true

        anchors.horizontalCenter: root.horizontalCenter
        anchors.top: toolBar.bottom
        anchors.left: root.left
        architekciModel: root.archModel
        urzedyModel: root.urzadModel
    }

    Settings {
        id: settings
        property string state: mainTheme.mode
    }

    Component.onCompleted: {
        if (settings.state == "light_mode") {
            mainTheme.setLightMode();
        } else {
            mainTheme.setDarkMode();
        }
    }

    Component.onDestruction: {
        settings.state = mainTheme.mode;
    }
}
