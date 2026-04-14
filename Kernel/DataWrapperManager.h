#ifndef DATAWRAPPERMANAGER_H
#define DATAWRAPPERMANAGER_H

#include <QObject>
#include <QAbstractListModel>
#include "KernelCommon.h"
#include "ModeleDanych/AbstractAppModel.h"
#include "DataWrappers/IDataWrapper.h"
#include <memory>


class DataWraperManager: public QObject
{
    Q_OBJECT
public:  

    explicit DataWraperManager(QObject *parent = nullptr);
    AbstractAppModel* getModel(DATA_TYPES aType);
    void createDataModels();
    void addSheetModel(DATA_TYPES aType, QList<QStringList> aData);
    void addDecyzjeSheetModel( QMap<QString, QList<QStringList>> aData);
    void dumpData();

    QStringList getProjectData(int index);
    QStringList getArchitektData(const QString & anIndex);

    QStringList getWnioskiList(const QString&anProjectId);
    QStringList getWniosekData(const QString&anProjectId, const QString&aEwidencjaId);
    QStringList getUrzadData(const QString&anName);

protected:
    struct DataWraperManagerImpl;

private :

    QList<QStringList> loadData(const IDataWrapper& aWrapper);

    std::shared_ptr<DataWraperManagerImpl> m_Impl;
};

#endif // DATAWRAPPERMANAGER_H
