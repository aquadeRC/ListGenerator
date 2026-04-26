pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

Rectangle {
    id: leftPanel

    required property AbstractItemModel architekciModel
    required property AbstractItemModel urzedyModel
    required property AbstractItemModel projektyModel
    required property AbstractItemModel pracowniaModel

    property variant wnioskiModel
    property variant currentArchitekt
    property string currentPracownia
    property string currentProjekt
    property string currentUrzad

    color: mainTheme.background_color

    ColumnLayout {
        id: topLayout
        anchors.fill: parent

        ScrollView {
            id: scroll
            contentHeight: 1000

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
                    //curentFieldValue: backend2.inwestycja
                    textRolenNme: "id"
                    valueRolenNme: "id"
                    higlight: true
                    Layout.fillWidth: true

                    onComboFieldChanged: (projektName, index) => {
                        leftPanel.currentProjekt = projektName;
                        leftPanel.wnioskiModel = backEnd.getWnioskiList(projektName);
                        let data = backEnd.getProjectData(index);

                        let projektId = data[0];
                        let architektID = data[1];
                        let inwestycja = data[2];
                        let dzialka = data[3];
                        let ewidencja = data[4];
                        let obreb = data[5];
                        let inwestor = data[6];

                        let archData = backEnd.getArchitektData(architektID);

                        leftPanel.currentArchitekt = archData;
                        let archName = archData[0];

                        architektCB.curentFieldValue = archName;
                        //inwestycjaF.fieldtext = inwestycja;
                        nrDzialka.fieldtext = dzialka;
                        ewidencjaF.fieldtext = ewidencja;
                        obrebF.fieldtext = obreb;
                        nazwaProjekt.fieldtext = inwestycja;
                        inwestorF.fieldtext = inwestor;

                        sprawaF.comboModel = leftPanel.wnioskiModel;
                    }
                }
                ComboField {
                    id: pracowniaCB
                    fieldText: "Pracownia"
                    implicitHeight: 58
                    width: 253
                    comboModel: leftPanel.pracowniaModel
                    //curentFieldValue: backend2.pracownia
                    textRolenNme: "id"
                    valueRolenNme: "id"
                    Layout.fillWidth: true

                    onComboFieldChanged: (pracowniatName, index) => {
                        leftPanel.currentPracownia = pracowniatName;
                    }
                }
                FieldText {
                    id: nazwaProjekt
                    fieldLabel: "Nazwa inwestycji"
                    width: 253
                    implicitHeight: 58
                    Layout.fillWidth: true
                }
                FieldText {
                    id: nazwaPisma
                    fieldLabel: "Nazwa pisma"
                    width: 253
                    implicitHeight: 58
                    Layout.fillWidth: true
                }
                ComboField {
                    id: architektCB
                    fieldText: "Architekt"
                    width: 253
                    implicitHeight: 58
                    comboModel: leftPanel.architekciModel
                    //curentFieldValue: backend2.architekt
                    textRolenNme: "nazwa"
                    valueRolenNme: "achitektId"
                    Layout.fillWidth: true
                }
                FieldText {
                    id: nrDzialka
                    fieldLabel: "Nr. Działki"
                    width: 253
                    implicitHeight: 58
                    Layout.fillWidth: true
                }
                FieldText {
                    id: ewidencjaF
                    fieldLabel: "Ewidencja"
                    width: 253
                    implicitHeight: 58
                    Layout.fillWidth: true
                }
                FieldText {
                    id: obrebF
                    fieldLabel: "Obręb"
                    width: 253
                    implicitHeight: 58
                    Layout.fillWidth: true
                }

                CheckBoxField {
                    id: typPisma
                    checked: true
                    text: qsTr("Odpowiewdz na pismo...")

                    Layout.fillWidth: true

                    onControlChecked: check => {
                        root.generujOdpowiedz = check;
                        sprawaF.curentFieldIndex = -1;
                        //inwestycjaF.fieldtext = "";
                        nazwaProjekt.fieldtext = "";
                        odpowiedzTresc.fieldtext = "";
                        zalaczniki.fieldtext = "";

                        if (check === true) {
                            nrSprawyItem.enabled = true;
                            nrSprawyItem.visible = true;
                            nrSprawyItem.height = 58;
                            urzadLoader.sourceComponent = urzadFieldComponent;
                        } else {
                            nrSprawyItem.enabled = false;
                            nrSprawyItem.visible = false;
                            nrSprawyItem.height = 0;
                            urzadLoader.sourceComponent = urzadCBComponent;
                        }
                    }
                }
                Item {
                    id: nrSprawyItem
                    width: 263
                    height: 58

                    ComboField2 {
                        id: sprawaF
                        fieldText: "Numer sprawy"
                        width: 253
                        implicitHeight: 58
                        higlight: true
                        //curentFieldValue: backend2.nr_sprawy
                        comboModel: []

                        Layout.fillHeight: true
                        Layout.alignment: Qt.AlignVCenter

                        onComboFieldChanged: (ewidencjaNr, index) => {
                            sprawaF.curentFieldValue = ewidencjaNr;
                            let data = backEnd.getWniosekData(leftPanel.currentProjekt, ewidencjaNr);
                            let urzadD = data[1];

                            if (root.generujOdpowiedz === true) {
                                urzadLoader.item.fieldtext = urzadD;
                            }

                            //inwestycjaF.fieldtext = data[2];
                            odpowiedzTresc.fieldtext = data[4];
                            zalaczniki.fieldtext = data[5];
                        }
                        onUpdateClicked: () => {
                            backEnd.updateWnioski();

                            leftPanel.wnioskiModel = backEnd.getWnioskiList(leftPanel.currentProjekt);
                            sprawaF.comboModel = leftPanel.wnioskiModel;
                        }
                    }
                }

                Loader {
                    id: urzadLoader
                    width: 253
                    height: 58
                    visible: true
                    Layout.fillWidth: true
                }
                FieldText {
                    id: inwestorF
                    fieldLabel: "Inwestor"
                    width: 253
                    implicitHeight: 58
                    Layout.fillWidth: true
                }
                /*FieldText {
                    id: inwestycjaF
                    fieldLabel: "Inwestycja"
                    width: 253
                    implicitHeight: 58
                    Layout.fillWidth: true
                }*/
                FieldText {
                    id: odpowiedzTresc
                    fieldLabel: "Treść odpowiedzi"
                    width: 253
                    implicitHeight: 58
                    Layout.fillWidth: true
                }
                FieldText {
                    id: zalaczniki
                    fieldLabel: "Załączniki"
                    width: 253
                    implicitHeight: 58
                    Layout.fillWidth: true
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
                id: reset
                text: qsTr("Reset")
                iconPath: "icons/ustawienia.svg"
                Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
                onClicked: {
                    leftPanel.currentPracownia = "";
                    leftPanel.currentProjekt = "";
                    leftPanel.currentUrzad = "";

                    projektF.curentFieldIndex = -1;
                    sprawaF.curentFieldIndex = -1;

                    if (root.generujOdpowiedz === true) {
                        urzadLoader.item.fieldtext = "";
                    } else {
                        urzadLoader.item.curentFieldIndex = -1;
                    }

                    architektCB.curentFieldIndex = -1;
                    pracowniaCB.curentFieldIndex = -1;

                    nazwaProjekt.fieldtext = "";
                    nrDzialka.fieldtext = "";
                    ewidencjaF.fieldtext = "";
                    obrebF.fieldtext = "";
                    inwestorF.fieldtext = "";
                    //inwestycjaF.fieldtext = "";
                    odpowiedzTresc.fieldtext = "";
                    zalaczniki.fieldtext = "";
                    nazwaPisma.fieldtext = "";
                }
            }

            IconButton {
                id: generujBT
                width: 40
                Layout.alignment: Qt.AlignLeft | Qt.AlignBottom
                text: qsTr("Generuj")
                iconPath: "icons/ustawienia.svg"
                onClicked: {
                    let urzadData = root.generujOdpowiedz ? urzadLoader.item.fieldtext : leftPanel.currentUrzad;
                    let sprawa = root.generujOdpowiedz ? sprawaF.curentFieldValue : "tmp";

                    /*
                    console.log(leftPanel.currentUrzad);
                    console.log("urzadData.length > 0", urzadData.length > 0);
                    console.log("inwestycjaF.fieldtext.length > 0", inwestycjaF.fieldtext.length > 0);
                    console.log("nrDzialka.fieldtext.length > 0", nrDzialka.fieldtext.length > 0);
                    console.log("obrebF.fieldtext.length > 0", obrebF.fieldtext.length > 0);
                    console.log("ewidencja.length > 0", ewidencjaF.fieldtext.length > 0);
                    console.log("inwestorF.fieldtext.length > 0", inwestorCB.fieldtext.length > 0);
                    console.log("sprawaF.backendProp.length > 0", sprawa.length > 0);
                    console.log("odpowiedzTresc.fieldtext.length > 0 ", odpowiedzTresc.fieldtext.length > 0);
                    console.log("zalaczniki.fieldtext.length > 0", zalaczniki.fieldtext.length > 0);
                    console.log("leftPanel.currentArchitekt.length > 0 ", leftPanel.currentArchitekt.length > 0);
                    console.log("nazwaPisma.fieldtext.length > 0", nazwaPisma.fieldtext.length > 0);
                    */

                    if (urzadData.length > 0 && nazwaProjekt.fieldtext.length > 0 && nrDzialka.fieldtext.length > 0 && obrebF.fieldtext.length > 0 && ewidencjaF.fieldtext.length > 0 && inwestorF.fieldtext.length > 0 && sprawa.length > 0 && odpowiedzTresc.fieldtext.length > 0 && zalaczniki.fieldtext.length > 0 && leftPanel.currentArchitekt.length > 0 && nazwaPisma.fieldtext.length > 0 && leftPanel.currentPracownia.length > 0) {
                        innerDoc.source = "file:///pusty.pdf";

                        let uData;
                        if (root.generujOdpowiedz === true) {
                            let data = urzadLoader.item.fieldtext;
                            const words = data.split("\n");
                            uData = words[0];
                        } else {
                            uData = leftPanel.currentUrzad;
                        }

                        const options = {
                            weekday: "long",
                            year: "numeric",
                            month: "numeric",
                            day: "numeric",
                            hour1: "false",
                            timeStyle: "short"
                        };
                        let wynikNazwa = `${leftPanel.currentProjekt}_${uData}_${Date.now().toLocaleString("pl-PL", options)}`;

                        let urzadData = "";
                        if (root.generujOdpowiedz === false) {
                            urzadData = leftPanel.currentUrzad;
                        } else {
                            urzadData = urzadLoader.item.fieldtext;
                        }

                        let data = {
                            "urzad_nazwa": urzadData,
                            "inwestcja_nazwa": nazwaProjekt.fieldtext,
                            "dzialka": nrDzialka.fieldtext,
                            "obreb": obrebF.fieldtext,
                            "ewidencja": ewidencjaF.fieldtext,
                            "inwestor": inwestorF.fieldtext,
                            "sprawa": sprawaF.curentFieldValue,
                            "tesc": odpowiedzTresc.fieldtext,
                            "zalaczniki": zalaczniki.fieldtext,
                            "architekt": leftPanel.currentArchitekt,
                            "pismoNazwa": nazwaPisma.fieldtext,
                            "odpowiedzType": root.generujOdpowiedz,
                            "pracownia": leftPanel.currentPracownia
                        };

                        // console.log(data);
                        let fileId = backEnd.generateDocument(wynikNazwa, data);
                        let newPdf = backEnd.getDocPdfPath([fileId, wynikNazwa]);

                        root.curentDoc = newPdf;
                        innerDoc.source = root.curentDoc;
                    } else {
                        message_Dialog.text = "Wypełnij wszystkie pola!";
                        message_Dialog.open();
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        sprawaF.enabled = true;

        urzadLoader.sourceComponent = urzadFieldComponent;
    }

    Component {
        id: urzadCBComponent
        ComboUrzedyField {
            id: urzadCB
            fieldText: "Urząd"
            width: 253
            implicitHeight: 58
            comboModel: leftPanel.urzedyModel
            //curentFieldValue: backend2.urzad
            textRolenNme: "nazwa"
            valueRolenNme: "nazwa"

            Layout.fillWidth: true
            onComboFieldChanged: (urzadId, index) => {
                leftPanel.currentUrzad = urzadId;
            }
        }
    }
    Component {
        id: urzadFieldComponent
        FieldText {
            id: urzadField
            fieldLabel: "Urząd"
            width: 253
            implicitHeight: 58
            Layout.fillWidth: true
        }
    }
}
