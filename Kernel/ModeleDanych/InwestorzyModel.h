#ifndef INWESTORZYMODEL_H
#define INWESTORZYMODEL_H

#include <QAbstractListModel>
#include "AbstractAppModel.h"


class InwestorzyModel : public AbstractAppModel
{
    Q_OBJECT

public:

    enum InwestorRoles {
        ID = Qt::UserRole + 1,
        Ulica,
        Kod,
        Miejscowosc
    };
    explicit InwestorzyModel(QObject *parent = nullptr);

    void initData(const QList<QStringList>&data) override;
    QList<QStringList> getData()  const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QHash<int, QByteArray> roleNames() const override;

     void dumpData() override;

private:
    QList<QStringList> m_inwestorData;

    const QString m_id{"id"};
    const QString m_ulica{"ulica"};
    const QString m_kod{"kod"};
    const QString m_miejscowosc{"miejscowosc"};
};

#endif // INWESTORZYMODEL_H
