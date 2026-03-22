#include "ArchitektDataModel.h"

//using namespace Modele_Danych;

ArchitektDataModel::ArchitektDataModel(QObject *parent)
    : QAbstractListModel(parent)
{}

void ArchitektDataModel::initData(const  QList<QMap<QString, QString>> &data)
{
    if(data.isEmpty() == false)
    {
        m_architektData = data;
    }
}

 QList<QMap<QString, QString>> ArchitektDataModel::getData() const
{
    return m_architektData;
}

int ArchitektDataModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_architektData.length();
}

QVariant ArchitektDataModel::data(const QModelIndex &index, int role) const
{
    if (!checkIndex(index, CheckIndexOption::IndexIsValid))
        return QVariant();

    if (!index.isValid())
        return QVariant();

    int row = index.row();

    switch (role) {
    case Nazwa:
        return m_architektData[row]["nazwa"];
    case Telefon:
        return m_architektData[row]["telefon"];
    case Email:
        return m_architektData[row]["email"];
    }

    return QVariant();
}


bool ArchitektDataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole || value.toString().isEmpty())
        return false;
    const int row = index.row();

    QStringList newData = value.toString().split("#");

    m_architektData[row]["nazwa"] = newData[0];
    m_architektData[row]["telefon"] = newData[1];
    m_architektData[row]["email"] = newData[2];

    emit dataChanged(index, index);
    return true;
}

Qt::ItemFlags ArchitektDataModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> ArchitektDataModel::roleNames() const
{
    QHash<int, QByteArray> mapping {

        {ArchitektRoles::Nazwa, "nazwa"},
        {ArchitektRoles::Telefon, "telefon"},
        {ArchitektRoles::Email, "email"}

    };
    return mapping;
}