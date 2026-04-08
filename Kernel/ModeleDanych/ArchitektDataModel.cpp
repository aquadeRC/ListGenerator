#include "ArchitektDataModel.h"

//using namespace Modele_Danych;

ArchitektDataModel::ArchitektDataModel(QObject *parent)
    : AbstractAppModel(parent)
{}

void ArchitektDataModel::initData(const  QList<QStringList> &data)
{
    if(data.isEmpty() == false)
    {
        m_architektData = data;
    }
}

 QList<QStringList> ArchitektDataModel::getData() const
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
    case ID:
        return m_architektData[row].at(0);
    case Nazwa:
        return m_architektData[row].at(1);
    case Telefon:
        return m_architektData[row].at(2);
    case Email:
        return m_architektData[row].at(3);
    }

    return QVariant();
}


bool ArchitektDataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole || value.toString().isEmpty())
        return false;
    const int row = index.row();

    QStringList newData = value.toString().split("#");

    m_architektData[row][0] = newData[0];
    m_architektData[row][1] = newData[1];
    m_architektData[row][2] = newData[2];
    m_architektData[row][3] = newData[3];

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

        {ArchitektRoles::ID, "achitektId"},
        {ArchitektRoles::Nazwa, "nazwa"},
        {ArchitektRoles::Telefon, "telefon"},
        {ArchitektRoles::Email, "email"}

    };
    return mapping;
}

 void ArchitektDataModel::dumpData()
{
    qDebug() << "ArchitektDataModel";
    QListIterator<QStringList> it(m_architektData);
    while(it.hasNext())
        {
        qDebug() << it.next() ;
        }
}