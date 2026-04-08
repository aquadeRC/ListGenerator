#include "InwestorzyModel.h"

InwestorzyModel::InwestorzyModel(QObject *parent)
    : AbstractAppModel(parent)
{}


int InwestorzyModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return m_inwestorData.length();
}

QVariant InwestorzyModel::data(const QModelIndex &index, int role) const
{
    if (!checkIndex(index, CheckIndexOption::IndexIsValid))
        return QVariant();

    if (!index.isValid())
        return QVariant();

    int row = index.row();

    switch (role) {
    case ID:
        return m_inwestorData[row].at(0);
    case Ulica:
        return m_inwestorData[row].at(1);
    case Kod:
        return m_inwestorData[row].at(2);
    case Miejscowosc:
        return m_inwestorData[row].at(3);
    }

    return QVariant();
}

QList<QStringList> InwestorzyModel::getData() const
{
    return m_inwestorData;
}

void InwestorzyModel::initData(const  QList<QStringList> &data)
{
    beginResetModel();
    if(data.isEmpty() == false)
    {
        m_inwestorData = data;
    }
    endResetModel();
}

bool InwestorzyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole || value.toString().isEmpty())
        return false;
    const int row = index.row();

    QStringList newData = value.toString().split("#");

    m_inwestorData[row][0] = newData[0];
    m_inwestorData[row][1] = newData[1];
    m_inwestorData[row][2] = newData[2];
    m_inwestorData[row][3] = newData[3];

    emit dataChanged(index, index);
    return true;
}

Qt::ItemFlags InwestorzyModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> InwestorzyModel::roleNames() const
{
    QHash<int, QByteArray> mapping {

        {InwestorRoles::ID, m_id.toUtf8()},
            {InwestorRoles::Ulica, m_ulica.toUtf8()},
            {InwestorRoles::Kod, m_kod.toUtf8()},
            {InwestorRoles::Miejscowosc, m_miejscowosc.toUtf8()},

    };
    return mapping;
}

void InwestorzyModel::dumpData()
{
    qDebug() << "InwestorzyModel";
    QListIterator<QStringList> it(m_inwestorData);
    while(it.hasNext())
    {
        qDebug() << it.next() ;
    }
}