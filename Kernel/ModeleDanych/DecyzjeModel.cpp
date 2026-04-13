#include "DecyzjeModel.h"


DecyzjeModel::DecyzjeModel(QObject *parent)
    : QObject(parent)
{}

QMap<QString, QList<QStringList>> DecyzjeModel::getData() const
{
    return m_decyzjaData;
}

QList<QStringList> DecyzjeModel::getDataForProjekt(const QString& anID) const
{
    return m_decyzjaData[anID];
}

QStringList DecyzjeModel::getDataForProjektAndEwidencja(const QString& aProjekt,
                                          const QString& aEwidencja) const
{
    QStringList result;
    QListIterator<QStringList> it( m_decyzjaData[aProjekt]);
    while(it.hasNext())
    {
        auto data = it.next();
        if(data.contains(aEwidencja))
        {
            result = data;
        }
    }
    return result;
}

void DecyzjeModel::initData(const QMap<QString, QList<QStringList>> &data)
{
    if(data.isEmpty() == false)
    {
        m_decyzjaData = data;
    }
}

void DecyzjeModel::dumpData()
{
    qDebug() << "DecyzjaModel";
    QMapIterator<QString, QList<QStringList>> it(m_decyzjaData);
    while(it.hasNext())
    {
        auto data = it.next();
        qDebug() << data.key() << ":" ;
        QListIterator<QStringList> dataIt = data.value();
        while(dataIt.hasNext())
        {
            qDebug() << dataIt.next();
        }
    }
}