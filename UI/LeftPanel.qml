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
        id: topLayout
        anchors.fill: parent

        ScrollView {
            id: scroll
            height: topLayout.height - 90
            width: topLayout.width
            contentHeight: topLayout.height//fieldsColumL.height  // Same
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            clip: true

            ColumnLayout {
                id: fieldsColumL
                spacing: 19
                anchors.top: parent.top

                ComboField {
                    id: projektF
                    fieldText: "Projekt"
                    implicitHeight: 58
                    width: 253
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
                FieldText {
                    id: nazwaProjekt
                    fieldText: "Nazwa"
                    width: 253
                    implicitHeight: 58
                }
                ComboField {
                    id: architektCB
                    fieldText: "Architekt"
                    width: 253
                    implicitHeight: 58
                    comboModel: leftPanel.architekciModel
                    backendProp: backend2.architekt
                    textRolenNme: "nazwa"
                    valueRolenNme: "achitektId"
                }
                FieldText {
                    id: nrDzialka
                    fieldText: "Nr. Działki"
                    width: 253
                    implicitHeight: 58
                }
                FieldText {
                    id: ewidencja
                    fieldText: "Ewidencja"
                    width: 253
                    implicitHeight: 58
                }
                FieldText {
                    id: obreb
                    fieldText: "Obręb"
                    width: 253
                    implicitHeight: 58
                }
                ComboField {
                    id: sprawaF
                    fieldText: "Numer sprawy"
                    width: 253
                    implicitHeight: 58
                    comboModel: UrzadModel {}
                    backendProp: backend2.nr_sprawy
                    textRolenNme: "nazwa"
                    valueRolenNme: "nazwa"
                }
                ComboField {
                    id: urzadCB
                    fieldText: "Urząd"
                    width: 253
                    implicitHeight: 58
                    comboModel: leftPanel.urzedyModel
                    backendProp: backend2.urzad
                    textRolenNme: "nazwa"
                    valueRolenNme: "nazwa"
                }
                FieldText {
                    id: inwestorCB
                    fieldText: "Inwestor"
                    width: 253
                    implicitHeight: 58
                }
                FieldText {
                    id: inwestycjaF
                    fieldText: "Inwestycja"
                    width: 253
                    implicitHeight: 58
                }
                FieldText {
                    id: odpowiedzTresc
                    fieldText: "Treść odpowiedzi"
                    width: 253
                    implicitHeight: 58
                }
                FieldText {
                    id: zalaczniki
                    fieldText: "Załączniki"
                    width: 253
                    implicitHeight: 58
                }
            }
        }

        IconButton {
            id: generujBT
            width: 40
            Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
            buttonName: qsTr("Generuj")
            iconPath: "icons/ustawienia.svg"
            onClicked: {
                backEnd.generateDocument("ala ma kota");
            }
        }
    }
}
