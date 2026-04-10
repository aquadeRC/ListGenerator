#ifndef PROJEKTMODEL_H
#define PROJEKTMODEL_H

#include <QAbstractListModel>
#include "AbstractAppModel.h"


class ProjektModel : public AbstractAppModel
{
    Q_OBJECT

public:
    enum ProjektRoles {
        ID = Qt::UserRole + 1,
        Prowadzacy,
        Stadium,
        Tytuł,
        Dzialka,
        Ewidencja,
        Obreb,
        Ulica,
        Inwestor,
        Urzad
    };
    explicit ProjektModel(QObject *parent = nullptr);

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
    const QString m_prowadzacy{"prowadzacy"};
    const QString m_stadium{"stadium"};
    const QString m_tytul{"tytul"};
    const QString m_dzialka{"dzialka"};
    const QString m_ewidencja{"ewidencja"};
    const QString m_obreb{"obreb"};
    const QString m_ulica{"ulica"};
    const QString m_inwestor{"inwestor"};
    const QString m_urzad{"urzad"};
};

#endif // PROJEKTMODEL_H
