#ifndef DATAWRAPPERMANAGER_H
#define DATAWRAPPERMANAGER_H

#include <QObject>
#include <QAbstractListModel>
#include "KernelCommon.h"
#include "ModeleDanych/AbstractAppModel.h"
#include "DataWrappers/IDataWrapper.h"
#include <memory>
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
    AbstractAppModel* getModel(DATA_TYPES aType);

    void addSheetModel(DATA_TYPES aType, QList<QStringList> aData);

    void dumpData();

protected:
    struct DataWraperManagerImpl;

private :
    void createDataModels();
    QList<QStringList> loadData(const IDataWrapper& aWrapper);

    std::shared_ptr<DataWraperManagerImpl> m_Impl;
};

#endif // DATAWRAPPERMANAGER_H
