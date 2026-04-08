#ifndef ARCHITEKTDATAMODEL_H
#define ARCHITEKTDATAMODEL_H

#include "AbstractAppModel.h"
#include <QAbstractListModel>
#include <QModelIndex>
#include <QVariant>
#include <QtQml>

//namespace Modele_Danych {
class ArchitektDataModel : public AbstractAppModel
{
    Q_OBJECT
public:
    enum ArchitektRoles {
        ID = Qt::UserRole + 1,
        Nazwa,
        Telefon,
        Email
    };

    explicit ArchitektDataModel(QObject *parent = nullptr);

    void initData(const QList<QStringList>&data) override;
    QList<QStringList> getData() const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;  
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QHash<int, QByteArray> roleNames() const override;
    void dumpData() override;

private:
     QList<QStringList> m_architektData;
};
//}
#endif // ARCHITEKTDATAMODEL_H
