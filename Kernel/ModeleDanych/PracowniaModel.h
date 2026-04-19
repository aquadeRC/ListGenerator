#ifndef PRACOWNIAMODEL_H
#define PRACOWNIAMODEL_H

#include <QAbstractListModel>
#include "AbstractAppModel.h"

class PracowniaModel: public AbstractAppModel
{
    Q_OBJECT

public:
    enum ProjektRoles {
        ID = Qt::UserRole + 1,
        Adres,
        Kod,
        Miejscowosc,
        Telefon,
        Nip,
        Krs
    };
    explicit PracowniaModel(QObject *parent = nullptr);

    void initData(const QList<QStringList>&data) override;
    QList<QStringList> getData()  const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QHash<int, QByteArray> roleNames() const override;

    void dumpData() override;


private:
    QList<QStringList> m_projektData;
    const QString m_id{"id"};
    const QString m_adres{"adres"};
    const QString m_kod{"kod"};
    const QString m_tmiejscowoscl{"miejscowosc"};
    const QString m_telefon{"telefon"};
    const QString m_nip{"nip"};
    const QString m_krs{"krs"};
};

#endif // PRACOWNIAMODEL_H
