#include "UrzadDataModel.h"


using namespace Modele_Danych;

UrzadDataModel::UrzadDataModel(QObject *parent)
    : QAbstractListModel(parent)
{}

int UrzadDataModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return m_userData.length();
}

QVariant UrzadDataModel::data(const QModelIndex &index, int role) const
{
        if (!checkIndex(index, CheckIndexOption::IndexIsValid))
            return QVariant();

        if (!index.isValid())
            return QVariant();

        int row = index.row();

        switch (role) {
        case ID:
            return m_userData[row].m_kod;
        case Nazwa:
            return m_userData[row].m_nazwa;
        case Nazwa_Samorzadu:
            return m_userData[row].m_nazwa_samorządu;
        case Wojewodztwo:
            return m_userData[row].m_województwo;
        case Powiat:
            return m_userData[row].m_powiat;
        case Miejscowosc:
            return m_userData[row].m_miejscowość;
        case Kod_Pocztowy:
            return m_userData[row].m_kod_pocztowy;
        case Poczta:
            return m_userData[row].m_poczta;
        case Ulica:
            return m_userData[row].m_ulica;
        case Nr_Domu:
            return m_userData[row].m_nr_domu;
        case Kierunkowy:
            return m_userData[row].m_kierunkowy;
        case Telefon:
            return m_userData[row].m_telefon;
        case Telefon2:
            return m_userData[row].m_telefon2;
        case Email:
            return m_userData[row].m_email;
        }

        return QVariant();
}


bool UrzadDataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole || value.toString().isEmpty())
        return false;
    const int row = index.row();

    QStringList newData = value.toString().split("#");

    m_userData[row].m_kod = newData[0];
    m_userData[row].m_nazwa = newData[1];
    m_userData[row].m_nazwa_samorządu = newData[2];
    m_userData[row].m_województwo = newData[3];
    m_userData[row].m_powiat = newData[4];
    m_userData[row].m_miejscowość = newData[5];
    m_userData[row].m_kod_pocztowy = newData[6];
    m_userData[row].m_poczta = newData[7];
    m_userData[row].m_ulica = newData[8];
    m_userData[row].m_nr_domu = newData[9];
    m_userData[row].m_kierunkowy = newData[10];
    m_userData[row].m_telefon = newData[11];
    m_userData[row].m_telefon2 = newData[12];
    m_userData[row].m_email = newData[13];

    emit dataChanged(index, index);
    return true;
}

Qt::ItemFlags UrzadDataModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> UrzadDataModel::roleNames() const
{
    QHash<int, QByteArray> mapping {
        {UrzadRoles::ID, "id"},
        {UrzadRoles::Nazwa, "nazwa"},
        {UrzadRoles::Nazwa_Samorzadu, "nazwa_samorzad"},
        {UrzadRoles::Wojewodztwo, "wojewodztwo"},
        {UrzadRoles::Powiat, "powiat"},
        {UrzadRoles::Miejscowosc, "miejscowosc"},
        {UrzadRoles::Kod_Pocztowy, "kod_pocztowy"},
        {UrzadRoles::Poczta, "poczta"},
        {UrzadRoles::Ulica, "ulica"},
        {UrzadRoles::Nr_Domu, "nr_domu"},
        {UrzadRoles::Kierunkowy, "kierunkowy"},
        {UrzadRoles::Telefon, "telefon"},
        {UrzadRoles::Telefon2, "telefon2"},
        {UrzadRoles::Email, "email"}

    };
    return mapping;
}

void UrzadDataModel::readData()
{

}