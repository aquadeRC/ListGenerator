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
            contentHeight: 820

            contentWidth: leftPanel.width
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            clip: true
            spacing: 5

            ColumnLayout {
                id: fieldsColumL
                spacing: 10
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom

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
                        console.log(data);

                        let projektId = data[0];
                        let architektID = data[1];
                        let inwestycja = data[2];
                        //let inwestor = data[3];
                        let dzialka = data[3];
                        let ewidencja = data[4];
                        let obreb = data[5];
                        // let urzad = data[7];

                        let archData = backEnd.getArchitektData(architektID);
                        let archName = archData[0];

                        architektCB.backendProp = archName;
                        inwestycjaF.fieldtext = inwestycja;
                        nrDzialka.fieldtext = dzialka;
                        ewidencjaF.fieldtext = ewidencja;
                        obrebF.fieldtext = obreb;
                        nazwaProjekt.fieldtext = inwestycja;

                    //inwestorCB.backendProp = inwestor;
                    //urzadCB.backendProp = urzad;

                    }
                }
                FieldText {
                    id: nazwaProjekt
                    fieldLabel: "Nazwa"
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
                    fieldLabel: "Nr. Działki"
                    width: 253
                    implicitHeight: 58
                }
                FieldText {
                    id: ewidencjaF
                    fieldLabel: "Ewidencja"
                    width: 253
                    implicitHeight: 58
                }
                FieldText {
                    id: obrebF
                    fieldLabel: "Obręb"
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
                    fieldLabel: "Inwestor"
                    width: 253
                    implicitHeight: 58
                }
                FieldText {
                    id: inwestycjaF
                    fieldLabel: "Inwestycja"
                    width: 253
                    implicitHeight: 58
                }
                FieldText {
                    id: odpowiedzTresc
                    fieldLabel: "Treść odpowiedzi"
                    width: 253
                    implicitHeight: 58
                }
                FieldText {
                    id: zalaczniki
                    fieldLabel: "Załączniki"
                    width: 253
                    implicitHeight: 58
                }

                Rectangle {
                    width: 10
                    color: "#00ffffff"
                    Layout.maximumWidth: 10
                    Layout.fillHeight: true
                }
            }
        }

        RowLayout {
            id: rowLayout
            Layout.maximumHeight: 40
            Layout.preferredHeight: 40
            uniformCellSizes: false
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 5

            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            Rectangle {
                width: 200
                height: 10

                color: "#00ffffff"
                Layout.maximumHeight: 10
                Layout.fillWidth: true
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
}
