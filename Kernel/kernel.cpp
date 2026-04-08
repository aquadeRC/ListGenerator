#include "kernel.h"
#include "KernelCommon.h"

#include<QNetworkReply>


Kernel::Kernel(QObject *parent)
    : QObject{parent},
    m_DataWrapperManager {DataWraperManager(this)},
    m_googleWrapper{GoogleSSO(this)}
{

    QObject::connect(&m_googleWrapper, &GoogleSSO::isAuthenticatedChanged,
            this, &Kernel::slotSetAuthenticated);

  //  bool result = m_googleWrapper.init();
}

AbstractAppModel* Kernel::getModelInwestycje()
{
   return m_DataWrapperManager.getModel(DATA_TYPES::INWESTYCJE_DATA);
}

AbstractAppModel* Kernel::getModelUrzedy()
{
    return m_DataWrapperManager.getModel(DATA_TYPES::URZEDY_DATA);
}

AbstractAppModel* Kernel::getModelArchitekci()
{
    return m_DataWrapperManager.getModel(DATA_TYPES::ARCHITEKT_DATA);
}

AbstractAppModel* Kernel::getModelInwestor()
{
     return m_DataWrapperManager.getModel(DATA_TYPES::INWESTOR_DATA);
}

AbstractAppModel* Kernel::getModelProjekt()
{
    return m_DataWrapperManager.getModel(DATA_TYPES::PROJEKTY_DATA);
}


 void Kernel::authenticate()
{
     m_googleWrapper.authenticate();
}


void Kernel::getDataFromGoogle()
{
    qDebug() << "at getDataFromGoogle";
    getProjects();
    getInwestorzy();
    getArchitekci();
    getUrzedy();

    m_DataWrapperManager.dumpData();
}

void Kernel::getProjects()
{
    std::optional<QJsonArray> result = m_googleWrapper.getSheetValues("Projekty");

    QList<QStringList> projects;

    if(result.has_value())
    {
        for(const auto &arr: result.value())
        {
            const QJsonArray row = arr.toArray();
            QStringList projekt;
            for(const auto& rowValue:row)
            {
                QString val = rowValue.toString();
                projekt.append(val);
            }
            projects.append(projekt);
        }

        projects.removeAt(0);
        projects.removeAt(0);
    }


     m_DataWrapperManager.addSheetModel(DATA_TYPES::PROJEKTY_DATA, projects);
}

void Kernel::getInwestorzy()
{
    std::optional<QJsonArray> result = m_googleWrapper.getSheetValues("Inwestorzy");

    QList<QStringList> inwestorzy;

    if(result.has_value())
    {
        for(const auto &arr: result.value())
        {
            const QJsonArray row = arr.toArray();
            QStringList inwestor;
            for(const auto& rowValue:row)
            {
                QString val = rowValue.toString();
                inwestor.append(val);
            }

            inwestorzy.append(inwestor);
        }
      inwestorzy.removeAt(0);
    }


    m_DataWrapperManager.addSheetModel(DATA_TYPES::INWESTOR_DATA, inwestorzy);
}

void Kernel::getArchitekci()
{
    std::optional<QJsonArray> result = m_googleWrapper.getSheetValues("Architekci");

    QList<QStringList> architekci;

    if(result.has_value())
    {
        for(const auto &arr: result.value())
        {
            const QJsonArray row = arr.toArray();
            QStringList architekt;
            for(const auto& rowValue:row)
            {
                QString val = rowValue.toString();
                architekt.append(val);
            }
            architekci.append(architekt);
        }
      architekci.removeAt(0);
    }


    m_DataWrapperManager.addSheetModel(DATA_TYPES::ARCHITEKT_DATA, architekci);
}

void Kernel::getUrzedy()
{

}

void Kernel::slotSetAuthenticated()
{
    bool au= m_googleWrapper.isAuthenticated();

    if (m_isAuthenticated != au) {
        m_isAuthenticated = au;

        if(m_isAuthenticated)
            qDebug() << "m_isAuthenticated";
            getDataFromGoogle();

        emit isAuthenticatedChanged();
    }
}