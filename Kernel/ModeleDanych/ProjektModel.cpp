#include "ProjektModel.h"

ProjektModel::ProjektModel(QObject *parent)
    : AbstractAppModel(parent)
{}

int ProjektModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return m_projektData.length();
}

QVariant ProjektModel::data(const QModelIndex &index, int role) const
{    qDebug() << role <<index ;

    if (!checkIndex(index, CheckIndexOption::IndexIsValid))
        return QVariant();

    if (!index.isValid())
        return QVariant();

    int row = index.row();


    switch (role) {
    case ID:
    case Qt::DisplayRole:
        return m_projektData[row].at(0);
    case Prowadzacy:
        return m_projektData[row].at(1);
    case Stadium:
        return m_projektData[row].at(2);
    case Tytuł:
        return m_projektData[row].at(3);
    case Dzialka:
        return m_projektData[row].at(4);
    case Ewidencja:
        return m_projektData[row].at(5);
    case Obreb:
        return m_projektData[row].at(6);
    case Ulica:
        return m_projektData[row].at(7);
    case Inwestor:
        return m_projektData[row].at(8);
    }

    return QVariant();
}

QList<QStringList> ProjektModel::getData() const
{
    return m_projektData;
}

void ProjektModel::initData(const  QList<QStringList> &data)
{
    beginResetModel();
    if(data.isEmpty() == false)
    {
        m_projektData = data;
    }
    endResetModel();
}

bool ProjektModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole || value.toString().isEmpty())
        return false;
    const int row = index.row();

    QStringList newData = value.toString().split("#");

    m_projektData[row][0] = newData[0];
    m_projektData[row][1] = newData[1];
    m_projektData[row][2] = newData[2];
    m_projektData[row][3] = newData[3];
    m_projektData[row][4] = newData[4];
    m_projektData[row][5] = newData[5];
    m_projektData[row][6] = newData[6];
    m_projektData[row][7] = newData[7];
    m_projektData[row][8] = newData[8];

    emit dataChanged(index, index);
    return true;
}

Qt::ItemFlags ProjektModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QHash<int, QByteArray> ProjektModel::roleNames() const
{
    QHash<int, QByteArray> mapping {

        {ProjektRoles::ID, m_id.toUtf8()},
        {ProjektRoles::Prowadzacy, m_prowadzacy.toUtf8()},
        {ProjektRoles::Stadium, m_stadium.toUtf8()},
        {ProjektRoles::Tytuł, m_tytul.toUtf8()},
        {ProjektRoles::Dzialka, m_dzialka.toUtf8()},
        {ProjektRoles::Ewidencja, m_ewidencja.toUtf8()},
        {ProjektRoles::Obreb, m_obreb.toUtf8()},
        {ProjektRoles::Ulica, m_ulica.toUtf8()},
        {ProjektRoles::Inwestor, m_inwestor.toUtf8()}
    };
    return mapping;
}

void ProjektModel::dumpData()
{
    qDebug() << "ProjektModel";
    QListIterator<QStringList> it(m_projektData);
    while(it.hasNext())
    {
        qDebug() << it.next() ;
    }
}