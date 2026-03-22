#ifndef DATAWRAPPERMANAGER_H
#define DATAWRAPPERMANAGER_H

#include <QObject>
#include <QAbstractListModel>
#include "KernelCommon.h"
#include "ModeleDanych/AbstractAppModel.h"
#include "DataWrappers/IDataWrapper.h"

/*
struct DataObject{
public:
    DATA_TYPES m_type;
    std::shared_ptr<AbstractAppModel> m_model;
   // QList<QMap<QString, QString>> m_data;
};*/

class DataWraperManager: public QObject
{
    Q_OBJECT
public:  

    explicit DataWraperManager(QObject *parent = nullptr);
    //QList<QMap<QString, QString>> getData(DATA_TYPES aType);
    AbstractAppModel* getModel(DATA_TYPES aType);

private :
    void createDataModels();
    QList<QMap<QString, QString>> loadData(const IDataWrapper& aWrapper);

    QString m_dataDir{"/Kernel/Data/"};
    //QMap<DATA_TYPES, QList<QMap<QString, QString>>> m_data;
   // QList<std::shared_ptr<IDataWrapper>> m_wrappers;
    QMap<DATA_TYPES, std::shared_ptr<AbstractAppModel>> m_dataModels;
};

#endif // DATAWRAPPERMANAGER_H
