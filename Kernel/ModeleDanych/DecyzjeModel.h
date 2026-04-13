#ifndef DECYZJEMODEL_H
#define DECYZJEMODEL_H

#include <QAbstractItemModel>

class DecyzjeModel : public QObject
{
    Q_OBJECT

public:
    enum DecyzjaRoles {
        ID = Qt::UserRole + 1,
        Urzad,
        Nazwa,
        Inwestor,
        Tresc,
        Zalaczniki,
        Kontakt
    };
    explicit DecyzjeModel(QObject *parent = nullptr);

    void initData(const QMap<QString, QList<QStringList>>&data);
    QMap<QString, QList<QStringList>> getData() const;
    QList<QStringList> getDataForProjekt(const QString& anID) const;
    QStringList getDataForProjektAndEwidencja(const QString& aProjekt,
                                              const QString& aEwidencja) const;
    void dumpData();

private:
    QMap<QString, QList<QStringList>> m_decyzjaData;

    const QString m_id{"id"};
    const QString m_urzad{"urzad"};
    const QString m_nazwa{"nazwa"};
    const QString m_inwestor{"inwestor"};
    const QString m_tesc{"tresc"};
    const QString m_zalaczniki{"załaczniki"};
    const QString m_kontakt{"kontakt"};
};

#endif // DECYZJEMODEL_H
