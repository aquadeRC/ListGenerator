
#include "kernel.h"
#include "KernelCommon.h"

#include <QNetworkReply>
#include <QGuiApplication>


Kernel::Kernel(QObject *parent)
    : QObject{parent},
    m_DataWrapperManager {DataWraperManager(this)},
    m_googleWrapper{GoogleSSO(this)},
    m_settins{new Ustawienia(this)},
    m_wnioskiModel{ new QStringListModel()}
{
     m_updateTokensMap["dzialka"] = "Inwestycja_dzialka";
     m_updateTokensMap["obreb"] = "Inwestycja_Obreb";
     m_updateTokensMap["ewidencja"] = "Inwestycja_Ewidencja";
     m_updateTokensMap["inwestcja_nazwa"] = "Inwestycja_Nazwa";
     m_updateTokensMap["inwestor"] = "Inwestor";
     m_updateTokensMap["tesc"] = "Tresc";
     m_updateTokensMap["zalaczniki"] = "Zalaczniki";
     m_updateTokensMap["sprawa"] = "Inwestycja_Sprawa";

     m_settins->load();
     m_DataWrapperManager.createDataModels();
     m_googleWrapper.init();


    QObject::connect(&m_googleWrapper, &GoogleSSO::isAuthenticatedChanged,
            this, &Kernel::slotSetAuthenticated);

    QObject::connect(qApp, &QGuiApplication::aboutToQuit, this,
                     [&](){m_settins->save();});
}

AbstractAppModel* Kernel::getModelInwestycje()
{
   return m_DataWrapperManager.getModel(DATA_TYPES::INWESTYCJE_DATA);
}

AbstractAppModel* Kernel::getModelUrzedy()
{
    return m_DataWrapperManager.getModel(DATA_TYPES::URZEDY_DATA);
}

AbstractAppModel* Kernel::getModelArchitekci()
{
    return m_DataWrapperManager.getModel(DATA_TYPES::ARCHITEKT_DATA);
}

AbstractAppModel* Kernel::getModelInwestor()
{
     return m_DataWrapperManager.getModel(DATA_TYPES::INWESTOR_DATA);
}

AbstractAppModel* Kernel::getModelProjekt()
{
    return m_DataWrapperManager.getModel(DATA_TYPES::PROJEKTY_DATA);
}

QStringList Kernel::getSettings()
{
    return Ustawienia::getSettingsData();
}

void  Kernel::setSettings(const QStringList &aData)
{
    Ustawienia::setSettingsData(aData);

    m_settins->load();
    m_googleWrapper.init();

    authenticate();
}

 void Kernel::authenticate()
{
     m_googleWrapper.authenticate();
}

void Kernel::getDataFromGoogle()
{
    getProjects();
    getInwestorzy();
    getArchitekci();
    getWnioski();
    //m_DataWrapperManager.dumpData();
}

void Kernel::getProjects()
{
    std::optional<QJsonArray> result =
        m_googleWrapper.getSheetValues(Ustawienia::getProjektyId(), "Projekty");
    QList<QStringList> projects;

    if(result.has_value())
    {
        for(const auto &arr: result.value())
        {
            const QJsonArray row = arr.toArray();
            QStringList projekt;
            for(const auto& rowValue:row)
            {
                QString val = rowValue.toString();
                projekt.append(val);
            }
            projects.append(projekt);
        }
        projects.removeAt(0);
        projects.removeAt(0);
    }
     m_DataWrapperManager.addSheetModel(DATA_TYPES::PROJEKTY_DATA, projects);
}

void Kernel::getInwestorzy()
{
    std::optional<QJsonArray> result
        = m_googleWrapper.getSheetValues(Ustawienia::getProjektyId(), "Inwestorzy");
    QList<QStringList> inwestorzy;

    if(result.has_value())
    {
        for(const auto &arr: result.value())
        {
            const QJsonArray row = arr.toArray();
            QStringList inwestor;
            for(const auto& rowValue:row)
            {
                QString val = rowValue.toString();
                inwestor.append(val);
            }
            inwestorzy.append(inwestor);
        }
      inwestorzy.removeAt(0);
    }
    m_DataWrapperManager.addSheetModel(DATA_TYPES::INWESTOR_DATA, inwestorzy);
}

void Kernel::getArchitekci()
{
    std::optional<QJsonArray> result
        = m_googleWrapper.getSheetValues(Ustawienia::getProjektyId(), "Architekci");
    QList<QStringList> architekci;

    if(result.has_value())
    {
        for(const auto &arr: result.value())
        {
            const QJsonArray row = arr.toArray();
            QStringList architekt;
            for(const auto& rowValue:row)
            {
                QString val = rowValue.toString();
                architekt.append(val);
            }
            architekci.append(architekt);
        }
      architekci.removeAt(0);
    }
    m_DataWrapperManager.addSheetModel(DATA_TYPES::ARCHITEKT_DATA, architekci);
}

void Kernel::getWnioski()
{
    const QString decysjeSheetId = Ustawienia::getDecyjzeId();

    std::optional<QStringList> sheetsIds
        = m_googleWrapper.getSheets(decysjeSheetId);
    QMap<QString, QList<QStringList>> decyzje;

    if(sheetsIds.has_value())
    {
        for(const auto &sheetId: sheetsIds.value())
        {
            std::optional<QJsonArray> result
                = m_googleWrapper.getSheetValues(decysjeSheetId, sheetId);
            QList<QStringList> sheetData;

            if(result.has_value())
            {
                for(const auto &arr: result.value())
                {
                    const QJsonArray row = arr.toArray();
                    QStringList data;
                    for(const auto& rowValue:row)
                    {
                        QString val = rowValue.toString();
                        data.append(val);
                    }
                    sheetData.append(data);
                }
                sheetData.removeAt(0);
                decyzje.insert(sheetId, sheetData);
            }
        }
    }
    m_DataWrapperManager.addDecyzjeSheetModel(decyzje);
}

QJsonObject Kernel::getDoc()
{
    std::optional<QJsonObject> result = m_googleWrapper.getDocument(Ustawienia::getTemplateId());
    if(result.has_value())
    {
        return result.value();
    }
    else
        return QJsonObject();
}

void Kernel::generateDocument(const QString & anID, const QVariantMap  &aData)
{
    auto data = createUpdateData(aData);
    m_googleWrapper.updateDocument(Ustawienia::getTemplateId(), anID, data);
}

void Kernel::slotSetAuthenticated()
{
    bool au = m_googleWrapper.isAuthenticated();
    if (m_isAuthenticated != au) {
        m_isAuthenticated = au;

        if(m_isAuthenticated)
            qDebug() << "m_isAuthenticated";
            getDataFromGoogle();

        emit isAuthenticatedChanged();
    }
}

void Kernel::slotCurrentProjektChanged(const QString & aProjektName, int index)
{
    qDebug() <<  "projekt name changed to " << aProjektName << index;
}

QStringList Kernel::getProjectData(int anIndex)
{
    return m_DataWrapperManager.getProjectData(anIndex);
}

QStringList Kernel::getArchitektData(const QString & anID)
{
    return m_DataWrapperManager.getArchitektData(anID);
}

QStringList Kernel::getWnioskiList(const QString &anProjectId)
{
    return m_DataWrapperManager.getWnioskiList(anProjectId);
}

QStringList Kernel::getWniosekData(const QString&anProjectId, const QString&aEwidencjaId)
{
    return m_DataWrapperManager.getWniosekData(anProjectId ,aEwidencjaId);
}


QString Kernel::createUpdateData(const QVariantMap  &aData)
{

    string rawStartToken = R"({"requests":[)";
    string rawEndToken = R"(]})";
    string rawToken = R"({"replaceAllText":{"replaceText":"%1","containsText":{"text":"{{%2}}","matchCase":false}}})";

    QString updateData = QString::fromUtf8(rawStartToken);

   // QString updateToken("{\"replaceAllText\":{\"replaceText\":\"%1\",\"containsText\":{\"text\":\"{{\"%2\"}}\",\"matchCase\":false}}}");

    QStringList tokens;
    QMapIterator<QString, QVariant> it(aData);
    while(it.hasNext())
    {
        auto mapData = it.next();

        if(m_updateTokensMap.contains(mapData.key()))
        {
            QString tokenName = m_updateTokensMap[mapData.key()];
            QString value = mapData.value().toString();

            QString tokenData = QString::fromUtf8(rawToken).arg(value, tokenName);
            tokens.append(tokenData);
        }
        else
        {
            if(mapData.key() == "architekt")
            {
                auto archData = mapData.value().toList();

                QString imie = QString::fromUtf8(rawToken).arg(archData[1].toString(), "Architekt_Imie");
                QString telefon = QString::fromUtf8(rawToken).arg(archData[2].toString(), "Architekt_telefon");
                QString email = QString::fromUtf8(rawToken).arg(archData[3].toString(), "Architekt_email");

                tokens.append(imie);
                tokens.append(telefon);
                tokens.append(email);
            }

            if(mapData.key() == "urzad_nazwa")
            {
                QString val = mapData.value().toString();
                QStringList urzadData = m_DataWrapperManager.getUrzadData(val);

                QString adresCaly = urzadData[8] + " "+ urzadData[9];

                QString nazwa = QString::fromUtf8(rawToken).arg(urzadData[4],"Urzad_Nazwa");
                QString adres = QString::fromUtf8(rawToken).arg(adresCaly,  "Urzad_Adres");
                QString kod = QString::fromUtf8(rawToken).arg(urzadData[6], "Urzad_Kod");
                QString miasto = QString::fromUtf8(rawToken).arg(urzadData[7], "Urzad_Miasto");

                tokens.append(nazwa);
                tokens.append(adres);
                tokens.append(kod);
                tokens.append(miasto);
            }
        }

    }

    QString tokenData = QString::fromUtf8(rawToken).arg(QDate::currentDate().toString(), "DATA");
    tokens.append(tokenData);

    updateData.append(tokens.join(","));
    updateData.append(QString::fromUtf8(rawEndToken));

return updateData;
}




