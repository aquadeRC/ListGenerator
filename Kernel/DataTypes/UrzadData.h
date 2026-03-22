#ifndef URZADDATA_H
#define URZADDATA_H

#include <QString>
namespace Modele_Danych {
struct UrzadData
{
    QString m_kod;
    QString m_nazwa;
    QString m_nazwa_samorządu;
    QString m_województwo;
    QString m_powiat;
    QString m_miejscowość;
    QString m_kod_pocztowy;
    QString m_poczta;
    QString m_ulica;
    QString m_nr_domu;
    QString m_kierunkowy;
    QString m_telefon;
    QString m_telefon2;
    QString m_email;
};
}

#endif // URZADDATA_H
