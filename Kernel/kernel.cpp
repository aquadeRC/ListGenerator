#include "kernel.h"
#include "KernelCommon.h"


Kernel::Kernel(QObject *parent)
    : QObject{parent},
    m_DataWrapperManager {DataWraperManager(this)}
{
    init();
}

QStringList Kernel::getModelInwestycje()
{
    return QStringList(); //getData
}

QList<QMap<QString, QString>> Kernel::getModelUrzedy()
{
return m_pimp->m_urzedy.get();
}

QList<QMap<QString, QString>> Kernel::getModelArchitekci()
{
    return m_pimp->m_architekci.get();
}


QStringList Kernel::getModelSpraw()
{
return QStringList();
}

void Kernel::init()
{
 auto archData = m_pimp->m_architektWraper->getData();

    qDebug() << archData.length();
m_pimp->m_architekci->initData(archData);
}
