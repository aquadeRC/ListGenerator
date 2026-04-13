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
