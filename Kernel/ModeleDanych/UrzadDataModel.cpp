#include "UrzadDataModel.h"


using namespace Modele_Danych;

UrzadDataModel::UrzadDataModel(QObject *parent)
    : AbstractAppModel(parent)
{}


void UrzadDataModel::initData(const  QList<QStringList> &data)
{
    beginResetModel();
    if(data.isEmpty() == false)
    {
        m_urzadData = data;
    }
    endResetModel();
}


QList<QStringList> UrzadDataModel::getData() const
{
    return m_urzadData;
}

QStringList UrzadDataModel::getDataFor(const QString& anUrzadNazwa) const
{
    QStringList result;
    QListIterator it(m_urzadData);
    while(it.hasNext())
    {
        QStringList data = it.next();
        if(data[4] == anUrzadNazwa)
        {
            result = data;
        }

    }
    return result;;
}

int UrzadDataModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return m_urzadData.length();
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
            return m_urzadData[row].at(0);
        case Nazwa_Samorzadu:
            return m_urzadData[row].at(1);
        case Wojewodztwo:
            return m_urzadData[row].at(2);
        case Powiat:
            return m_urzadData[row].at(3);
        case Nazwa:
            return m_urzadData[row].at(4);
        case Miejscowosc:
            return m_urzadData[row].at(5);
        case Kod_Pocztowy:
            return m_urzadData[row].at(6);
        case Poczta:
            return m_urzadData[row].at(7);
        case Ulica:
            return m_urzadData[row].at(8);
        case Nr_Domu:
            return m_urzadData[row].at(9);
        case Kierunkowy:
            return m_urzadData[row].at(10);
        case Telefon:
            return m_urzadData[row].at(11);
        case Telefon2:
            return m_urzadData[row].at(12);
        case Email:
            return m_urzadData[row].at(13);
        }

        return QVariant();
}


bool UrzadDataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole || value.toString().isEmpty())
        return false;
    const int row = index.row();

    QStringList newData = value.toString().split("#");

    m_urzadData[row][0] = newData[0];
    m_urzadData[row][1] = newData[1];
    m_urzadData[row][2] = newData[2];
    m_urzadData[row][3] = newData[3];
    m_urzadData[row][4] = newData[4];
    m_urzadData[row][5] = newData[5];
    m_urzadData[row][6] = newData[6];
    m_urzadData[row][7] = newData[7];
    m_urzadData[row][8] = newData[8];
    m_urzadData[row][9] = newData[9];
    m_urzadData[row][10] = newData[10];
    m_urzadData[row][11] = newData[11];
    m_urzadData[row][12] = newData[12];
    m_urzadData[row][13] = newData[13];

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

void UrzadDataModel::dumpData()
{
    qDebug() << "UrzadDataModel";
    QListIterator<QStringList> it(m_urzadData);
    while(it.hasNext())
    {
        qDebug() << it.next() ;
    }
}