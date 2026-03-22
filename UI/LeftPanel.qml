import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: leftPanel

    required property AbstractItemModel architekciModel
    required property AbstractItemModel urzedyModel

    color: mainTheme.background_color

    ColumnLayout {
        spacing: 19
        anchors.top: leftPanel.top
        anchors.left: leftPanel.left
        anchors.leftMargin: 15
        anchors.horizontalCenter: leftPanel.horizontalCenter

        ComboField {
            id: inwestycjaF
            fieldText: "Inwestycja"
            width: 253
            height: 58
            comboModel: InwestycjaModel {}
            backendProp: backend2.inwestycja
        }
        ComboField {
            id: sprawaF
            fieldText: "Numer sprawy"
            width: 253
            height: 58
            comboModel: UrzadModel {}
            backendProp: backend2.nr_sprawy
        }
        ComboField {
            id: urzadCB
            fieldText: "Urząd"
            width: 253
            height: 58
            comboModel: leftPanel.urzedyModel
            backendProp: backend2.urzad
        }
        ComboField {
            id: inwestorCB
            fieldText: "Inwestor"
            width: 253
            height: 58
            comboModel: InwestorModel {}
            backendProp: backend2.inwestor
        }
        ComboField {
            id: architektCB
            fieldText: "Architekt"
            width: 253
            height: 58
            comboModel: leftPanel.architekciModel
            backendProp: backend2.architekt
        }

        IconButton {
            id: generujBT
            buttonName: qsTr("Generuj")
            iconPath: "icons/ustawienia.svg"
        }
    }
}
