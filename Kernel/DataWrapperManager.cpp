#include "DataWrapperManager.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QUrl>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QList>

#include "DataWrappers/UrzadDataWrapper.h"

#include "ModeleDanych/ArchitektDataModel.h"
#include "ModeleDanych/UrzadDataModel.h"
#include "ModeleDanych/InwestorzyModel.h"
#include "ModeleDanych/ProjektModel.h"
#include "Ustawienia.h"


struct DataWraperManager::DataWraperManagerImpl
{
    DataWraperManagerImpl()
    {
    }

    QMap<DATA_TYPES, std::shared_ptr<AbstractAppModel>> m_dataModels;
};

DataWraperManager::DataWraperManager(QObject *parent)
    : QObject{parent}
{
    m_Impl.reset(new DataWraperManagerImpl());
    //createDataModels();
}

AbstractAppModel* DataWraperManager::getModel(DATA_TYPES aType)
{
    if(m_Impl->m_dataModels.contains(aType))
    {
        return m_Impl->m_dataModels[aType].get();
    }
    else
    {
        return nullptr;
    }
}

void DataWraperManager::createDataModels()
{
    UrzadDataWrapper urzadWrapper = UrzadDataWrapper();
    DATA_TYPES urzadType = urzadWrapper.getType();
    auto urzadData = loadData(urzadWrapper);

    m_Impl->m_dataModels[urzadType]= std::make_shared<Modele_Danych::UrzadDataModel>(this);
    m_Impl->m_dataModels[urzadType]->initData(urzadData);
}

void DataWraperManager::addSheetModel(DATA_TYPES aType, QList<QStringList> aData)
{
    switch(aType)
    {
    case DATA_TYPES::ARCHITEKT_DATA:
        {
        m_Impl->m_dataModels[aType]= std::make_shared<ArchitektDataModel>(this);
        m_Impl->m_dataModels[aType]->initData(aData);

        break;
        }
    case DATA_TYPES::PROJEKTY_DATA:
        {
        m_Impl->m_dataModels[aType]= std::make_shared<ProjektModel>(this);
        m_Impl->m_dataModels[aType]->initData(aData);

        break;
        }
    case DATA_TYPES::INWESTOR_DATA:
        {
        m_Impl->m_dataModels[aType]= std::make_shared<InwestorzyModel>(this);
        m_Impl->m_dataModels[aType]->initData(aData);

        break;
        }
    default:
        {
        }
    }
}

QList<QStringList> DataWraperManager::loadData(const IDataWrapper& aWrapper)
    {
    QDir dir;

    QString dataFile = aWrapper.getDataFile();
    QFile file(dir.absolutePath() + Ustawienia::getDataDir() + dataFile );
    if (!file.open(QIODevice::ReadOnly))
        {
           QFileInfo info(file);
            QString message = QString("Nie mogę otworzyć pliku %1").arg(info.absoluteFilePath() );
            qWarning(message.toStdString().c_str());

            return QList<QStringList>{};
        }

    QByteArray loadData = file.readAll();
    file.close();

    QJsonParseError perror;
    QJsonDocument loadDoc(QJsonDocument::fromJson(loadData,&perror));
    if(perror.error != QJsonParseError::NoError)
    {
        qWarning(perror.errorString().toStdString().c_str());
    }

    return aWrapper.read(loadDoc.object());
}


void DataWraperManager::dumpData()
{
    QMapIterator<DATA_TYPES, std::shared_ptr<AbstractAppModel>> modelsIt(m_Impl->m_dataModels);
    while (modelsIt.hasNext())
    {
        modelsIt.next();
        auto model = modelsIt.value();
        model->dumpData();
    }
}


QStringList DataWraperManager::getProjectData(int anIndex)
{
    auto model = m_Impl->m_dataModels[DATA_TYPES::PROJEKTY_DATA];
    auto index = model->index(anIndex);

    QString  projektId =   model->data(index, ProjektModel::ID).toString();
    QString  prowadzacy =   model->data(index, ProjektModel::Prowadzacy).toString();
    QString  tytul =   model->data(index, ProjektModel::Tytuł).toString();
    QString  inwestor =   model->data(index, ProjektModel::Inwestor).toString();
    QString  dzialka =   model->data(index, ProjektModel::Dzialka).toString();
    QString  ewidencja =   model->data(index, ProjektModel::Ewidencja).toString();
    QString  obreb =   model->data(index, ProjektModel::Obreb).toString();
    QString  urzad = model->data(index, ProjektModel::Urzad).toString();

    QStringList data;
    data.append(projektId);
    data.append(prowadzacy);
    data.append(tytul);
    data.append(inwestor);
    data.append(dzialka);
    data.append(ewidencja);
    data.append(obreb);
    data.append(urzad);

    return data;
}

QStringList DataWraperManager::getArchitektData(const QString & anID)
{
    auto model = m_Impl->m_dataModels[DATA_TYPES::ARCHITEKT_DATA];
    auto dataModel = model->getData();

    int row = 0;
    QListIterator<QStringList> it(dataModel);
    while(it.hasNext())
    {
        if(it.next().constFirst() == anID)
            break;
        else
            ++row;
    }

    auto index = model->index(row);

    QString  projektId =   model->data(index, ArchitektDataModel::ID).toString();
    QString  nazwa =   model->data(index, ArchitektDataModel::Nazwa).toString();
    QString  telefon =   model->data(index, ArchitektDataModel::Telefon).toString();
    QString  email =   model->data(index, ArchitektDataModel::Email).toString();

    QStringList data;
    data.append(projektId);
    data.append(nazwa);
    data.append(telefon);
    data.append(email);

    return data;
}

