#ifndef URZADDATAMODEL_H
#define URZADDATAMODEL_H

#include "AbstractAppModel.h"
#include <QAbstractListModel>
#include <QModelIndex>
#include <QVariant>

namespace Modele_Danych {
class UrzadDataModel : public AbstractAppModel
{
    Q_OBJECT
public:
    enum UrzadRoles {
        ID = Qt::UserRole + 1,
        Nazwa,
        Nazwa_Samorzadu,
        Wojewodztwo,
        Powiat,
        Miejscowosc,
        Kod_Pocztowy,
        Poczta,
        Ulica,
        Nr_Domu,
        Kierunkowy,
        Telefon,
        Telefon2,
        Email
    };
    explicit UrzadDataModel(QObject *parent = nullptr);

    void initData(const QList<QStringList>&data) override;
    QList<QStringList> getData() const override;
    QStringList getDataFor(const QString& anUrzadNazwa) const;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QHash<int, QByteArray> roleNames() const override;

     void dumpData() override;
private:
    QList<QStringList> m_urzadData;

};
}
#endif // URZADDATAMODEL_H
