#ifndef DATAWRAPPERMANAGER_H
#define DATAWRAPPERMANAGER_H

#include <QObject>
#include <QAbstractListModel>
#include "KernelCommon.h"
#include "DataWrappers/IDataWrapper.h"

struct DataObject{
public:
    DATA_TYPES m_type;
    std::shared_ptr<QAbstractListModel> m_model;
    QList<QMap<QString, QString>> m_data;
};

class DataWraperManager: public QObject
{
    Q_OBJECT
public:  

    explicit DataWraperManager(QObject *parent = nullptr);
    QList<QMap<QString, QString>> getData(DATA_TYPES aType);

private :
    void createDataModels();
    QList<QMap<QString, QString>> loadData(const IDataWrapper& aWrapper);

    QString m_dataDir{"/Kernel/Data/"};
    QMap<DATA_TYPES, QList<QMap<QString, QString>>> m_data;
    QList<std::shared_ptr<IDataWrapper>> m_wrappers;
    QList<DataObject> m_dataModels;
};

#endif // DATAWRAPPERMANAGER_H
