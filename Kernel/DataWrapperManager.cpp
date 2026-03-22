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
#include "DataWrappers/UrzadDataWrapper.h"

#include "ModeleDanych/ArchitektDataModel.h"
#include "ModeleDanych/UrzadDataModel.h"


DataWraperManager::DataWraperManager(QObject *parent)
    : QObject{parent}
{
    createDataModels();
}

AbstractAppModel* DataWraperManager::getModel(DATA_TYPES aType)
{
    if(m_dataModels.contains(aType))
    {
        return m_dataModels[aType].get();
    }
    else
    {
        return nullptr;
    }
}

void DataWraperManager::createDataModels()
{
    ArchitektDataWrapper archWrapper = ArchitektDataWrapper();
    DATA_TYPES archType = archWrapper.getType();
    auto archData = loadData(archWrapper);
    m_dataModels[archType]= std::make_shared<ArchitektDataModel>(this);
    m_dataModels[archType]->initData(archData);


    UrzadDataWrapper urzadWrapper = UrzadDataWrapper();
    DATA_TYPES urzadType = urzadWrapper.getType();
    auto urzadData = loadData(urzadWrapper);
    m_dataModels[urzadType]= std::make_shared<Modele_Danych::UrzadDataModel>(this);
    m_dataModels[urzadType]->initData(urzadData);
}


QList<QMap<QString, QString>> DataWraperManager::loadData(const IDataWrapper& aWrapper)
    {
    QDir dir;

    QString dataFile = aWrapper.getDataFile();
    QFile file(dir.absolutePath() + m_dataDir + dataFile );
    if (!file.open(QIODevice::ReadOnly))
        {
           QFileInfo info(file);
            QString message = QString("Nie mogę otworzyć pliku %1").arg(info.absoluteFilePath() );
            qWarning(message.toStdString().c_str());

            return QList<QMap<QString, QString>>{};
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

