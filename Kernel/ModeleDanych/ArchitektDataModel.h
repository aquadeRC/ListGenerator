#ifndef ARCHITEKTDATAMODEL_H
#define ARCHITEKTDATAMODEL_H

#include <QAbstractListModel>
#include <QModelIndex>
#include <QVariant>
#include <QtQml>

//namespace Modele_Danych {
class ArchitektDataModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ArchitektRoles {
        Nazwa= Qt::UserRole + 1,
        Telefon,
        Email
    };

    explicit ArchitektDataModel(QObject *parent = nullptr);

    void initData(const QList<QMap<QString, QString>>&data);
    QList<QMap<QString, QString>> getData() const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;  
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QHash<int, QByteArray> roleNames() const override;

private:
     QList<QMap<QString, QString>> m_architektData;


};
//}
#endif // ARCHITEKTDATAMODEL_H
