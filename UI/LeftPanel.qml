pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

Rectangle {
    id: leftPanel

    required property AbstractItemModel architekciModel
    required property AbstractItemModel urzedyModel
    required property AbstractItemModel projektyModel

    color: mainTheme.background_color

    ColumnLayout {
        spacing: 19
        anchors.top: leftPanel.top
        anchors.left: leftPanel.left
        anchors.leftMargin: 15
        anchors.horizontalCenter: leftPanel.horizontalCenter

        ComboField {
            id: projektF
            fieldText: "Projekt"
            width: 253
            height: 58
            comboModel: leftPanel.projektyModel
            backendProp: backend2.inwestycja
            textRolenNme: "id"
            valueRolenNme: "id"

            onComboFieldChanged: (projektName, index) => {
                let data = backEnd.getProjectData(index);

                let inwestycja = data[2];
                let inwestor = data[3];
                let architektID = data[1];
                let urzad = data[7];
                let archData = backEnd.getArchitektData(architektID);
                let archName = archData[0];

                architektCB.backendProp = archName;
                inwestycjaF.backendProp = inwestycja;
                inwestorCB.backendProp = inwestor;
                urzadCB.backendProp = urzad;
            }
        }

        ComboField {
            id: inwestycjaF
            fieldText: "Inwestycja"
            width: 253
            height: 58
            comboModel: InwestycjaModel {}
            backendProp: backend2.inwestycja
            textRolenNme: "nazwa"
            valueRolenNme: "nazwa"
        }
        ComboField {
            id: sprawaF
            fieldText: "Numer sprawy"
            width: 253
            height: 58
            comboModel: UrzadModel {}
            backendProp: backend2.nr_sprawy
            textRolenNme: "nazwa"
            valueRolenNme: "nazwa"
        }
        ComboField {
            id: urzadCB
            fieldText: "Urząd"
            width: 253
            height: 58
            comboModel: leftPanel.urzedyModel
            backendProp: backend2.urzad
            textRolenNme: "nazwa"
            valueRolenNme: "nazwa"
        }
        ComboField {
            id: inwestorCB
            fieldText: "Inwestor"
            width: 253
            height: 58
            comboModel: InwestorModel {}
            backendProp: backend2.inwestor
            textRolenNme: "nazwa"
            valueRolenNme: "nazwa"
        }
        ComboField {
            id: architektCB
            fieldText: "Architekt"
            width: 253
            height: 58
            comboModel: leftPanel.architekciModel
            backendProp: backend2.architekt
            textRolenNme: "nazwa"
            valueRolenNme: "achitektId"
        }

        IconButton {
            id: generujBT
            buttonName: qsTr("Generuj")
            iconPath: "icons/ustawienia.svg"

            onClicked: {
                backEnd.generateDocument("ala ma kota");
            }
        }
    }
}
