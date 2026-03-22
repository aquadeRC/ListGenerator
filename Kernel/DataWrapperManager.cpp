#include "DataWrapperManager.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QUrl>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QList>

#include "DataWrappers/ArchitektDataWrapper.h"
#include "ModeleDanych/ArchitektDataModel.h"


DataWraperManager::DataWraperManager(QObject *parent)
    : QObject{parent}
{
    createDataModels();
    loadData();
}

QList<QMap<QString, QString>>  DataWraperManager::getData(DATA_TYPES aType)
{
    if(m_data.contains(aType))
    {
        return m_data[aType];
    }
    else
    {
        return QList<QMap<QString, QString>> {};
    }
}

void DataWraperManager::createDataModels()
{
   // m_wrappers.append(std::make_shared<ArchitektDataWrapper>());

    ArchitektDataWrapper archWrapper = ArchitektDataWrapper();

    DataObject architekData;
    architekData.m_type = archWrapper.getType();
    architekData.m_data = loadData(archWrapper);
    architekData.m_model = std::make_shared<ArchitektDataModel>(this);
    architekData.m_model->initData(architekData.m_data);


    m_dataModels.append(architekData);
}


QList<QMap<QString, QString>> DataWraperManager::loadData(const IDataWrapper& aWrapper)
    {
    QDir dir;

        QString dataFile = aWrapper.getDataFile();
        QFile file(dir.absolutePath() + m_dataDir + dataFile );
        if (!file.open(QIODevice::ReadOnly))
        {
            QString message = QString("Nie mogę otworzyć pliku %1").arg(dataFile);
            qWarning(message.toStdString().c_str());
            return QList<QMap<QString, QString>>{};
        }

        QByteArray loadData = file.readAll();
        file.close();

        QJsonParseError perror;
        QJsonDocument loadDoc(QJsonDocument::fromJson(loadData,&perror));

        return aWrapper.read(loadDoc.object());
/*
    for(const auto& wrapper: m_wrappers )
    {
        QString dataFile = wrapper->getDataFile();
        QFile file(dir.absolutePath() + m_dataDir + dataFile );
        if (!file.open(QIODevice::ReadOnly))
            {
                QString message = QString("Nie mogę otworzyć pliku %1").arg(dataFile);
                qWarning(message.toStdString().c_str());
                return;
            }

        QByteArray loadData = file.readAll();
        file.close();

        QJsonParseError perror;
        QJsonDocument loadDoc(QJsonDocument::fromJson(loadData,&perror));

        auto data = wrapper->read(loadDoc.object());
        if(data.isEmpty() == false)
        {
            m_data[wrapper->getType()]= data;
        }
    }
*/
}

