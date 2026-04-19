
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
     m_updateTokensMap["pismoNazwa"] = "Tytul";

     m_settins->load();
     m_DataWrapperManager.createDataModels();
     m_googleWrapper.init();


    QObject::connect(&m_googleWrapper, &GoogleSSO::isAuthenticatedChanged,
            this, &Kernel::slotSetAuthenticated);

     QObject::connect(&m_googleWrapper, &GoogleSSO::signalError,
                      this, &Kernel::slotSooError);


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

AbstractAppModel* Kernel::getModelPracownia()
{
    return m_DataWrapperManager.getModel(DATA_TYPES::PRACOWNIA_DATA);
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
    getArchitekci();
    getWnioski();
    getPacownie();
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
    addLog(QString("Pobrano dane Projekty"), LogLewels::LOG_INFO);
}

void Kernel::getPacownie()
{
    std::optional<QJsonArray> result =
        m_googleWrapper.getSheetValues(Ustawienia::getProjektyId(), "Pracownia");
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
        qDebug() << projects;
    }
    m_DataWrapperManager.addSheetModel(DATA_TYPES::PRACOWNIA_DATA, projects);
    addLog(QString("Pobrano dane Pracownie"), LogLewels::LOG_INFO);
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
    addLog(QString("Pobrano dane Inwestorzu"), LogLewels::LOG_INFO);
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

    addLog(QString("Pobrano dane Architekt"), LogLewels::LOG_INFO);
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
    addLog(QString("Pobrano dane Wnioski"), LogLewels::LOG_INFO);
}

QUrl Kernel::getDocPdfPath(const QStringList &docIds )
{
    QString docId ;
    QString outName = "/tmp.pdf";
    if(docIds.isEmpty())
    {
        docId = Ustawienia::getTemplateId();
    }
    else
    {
        docId = docIds[0];
        outName = QString("/%1.pdf").arg(docIds[1]);
    }

    std::optional<QByteArray> result = m_googleWrapper.getDocumentAsPdf(docId);
    if(result.has_value())
    {
        QDir dir;
        QFile file(dir.absolutePath()  + outName);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate,
                       QFileDevice::WriteUser| QFileDevice::ReadUser| QFileDevice::ExeUser))
        {
            addLog(QString("Blod zapisu pdf %1").arg(outName), LogLewels::LOG_ERROR);
            return QString();
        }
        QFileInfo info(file);
        QString filePath = info.absoluteFilePath();

        file.write(result.value());
        file.close();


        addLog(QString("Utworzono dokument pdf %1").arg(filePath), LogLewels::LOG_INFO);
        return QUrl::fromLocalFile(filePath);
    }
    else
        return QString();
}

 Q_INVOKABLE QString Kernel::getDocId()
{
     return Ustawienia::getTemplateId();
}

QString Kernel::generateDocument(const QString & anID, const QVariantMap  &aData)
{
    auto data = createUpdateData(aData);
    std::optional<QString> out = m_googleWrapper.updateDocument(Ustawienia::getTemplateId(), anID, data);

    if(out.has_value())
    {
        addLog(QString("Utworzono dokument %1").arg(anID), LogLewels::LOG_INFO);
        emit signalDocumentCreated(anID);
        return out.value();
    }
    else
        return QString();
}

void Kernel::slotSetAuthenticated()
{
    bool au = m_googleWrapper.isAuthenticated();
    if (m_isAuthenticated != au) {
        m_isAuthenticated = au;

        if(m_isAuthenticated)
            addLog("Aplikacja polaczona z Google Api  ", LogLewels::LOG_INFO);

            getDataFromGoogle();

        emit isAuthenticatedChanged(m_isAuthenticated);
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
QStringList Kernel::getPracowniaData(const QString & anID)
{
     return m_DataWrapperManager.getPracowniaData(anID);
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

    QStringList tokens;
    QMapIterator<QString, QVariant> it(aData);
    bool isOdpowiedz = aData["odpowiedzType"].toBool();

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
                QString uData;

                if(isOdpowiedz)
                {
                uData = QString::fromUtf8(rawToken).arg(val , "Urzad_Nazwa");
                }
                else
                {
                    QStringList urzadData = m_DataWrapperManager.getUrzadData(val);
                    QString adresCaly = urzadData[8] + " " + urzadData[9];
                    QString data = QString("%1\n%2\n%3 %4").arg(urzadData[4], adresCaly, urzadData[6], urzadData[7]);
                    uData = QString::fromUtf8(rawToken).arg(data,"Urzad_Nazwa");
                }
                tokens.append(uData);
            }
            if(mapData.key() == "pracownia")
            {
                qDebug() <<mapData.value();
                auto pracowniaId= mapData.value().toString();
                QStringList pracowniaData = m_DataWrapperManager.getPracowniaData(pracowniaId);

                QString id = pracowniaData[0];
                QString adres = pracowniaData[1];
                QString kod = pracowniaData[2];
                QString miejscowosc = pracowniaData[3];
                QString telefon = pracowniaData[4];
                QString nip = pracowniaData[5];

                QString data = QString("%1, %2, &3 %4, tel %5, NIP %6").arg(id, adres, kod, miejscowosc, telefon, nip);

                QString prac = QString::fromUtf8(rawToken).arg(data, "Pracownia");
                tokens.append(prac);
            }
        }
    }

    QString tokenData = QString::fromUtf8(rawToken).arg(QDate::currentDate().toString("dd.MM.yyyy"), "DATA");
    tokens.append(tokenData);

    updateData.append(tokens.join(","));
    updateData.append(QString::fromUtf8(rawEndToken));


return updateData;
}


 void Kernel::slotSooError(const QString &error)
{
     addLog(error, LogLewels::LOG_ERROR);
     emit isError(error);
}


void  Kernel::addLog(const QString & aMessage, LogLewels alevel)
{
    QDir dir;
    QFile logFile  = QFile(dir.absolutePath() +"/"+ m_logName );
    if (!logFile.open(QIODevice::ReadOnly | QIODevice::Append | QIODevice::Text))
    {
        QFileInfo info(logFile);
        QString message = QString("Nie mogę otworzyć pliku logu  %1").arg(info.absoluteFilePath() );
        qWarning(message.toStdString().c_str());

        return ;
    }

    QString loglevel;
    switch(alevel)
    {
        case LogLewels::LOG_ERROR:
        {
            loglevel = "Error";
            break;

        }
        case LogLewels::LOG_INFO:
        {
            loglevel = "Info";
            break;

        }
        case LogLewels::LOG_WARNING:
        {
            loglevel = "Warning";
            break;

        }
    }



    QTextStream out(&logFile);
    QString message = QString("%1 %2 %4 %3\n").arg(QDate::currentDate().toString()).arg(QTime::currentTime().toString()).arg(aMessage).arg(loglevel);

    out << message;
    logFile.close();
}

