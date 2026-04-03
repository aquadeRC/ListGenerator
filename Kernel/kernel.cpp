#include "kernel.h"
#include "KernelCommon.h"

#include<QNetworkReply>


Kernel::Kernel(QObject *parent)
    : QObject{parent},
    m_DataWrapperManager {DataWraperManager(this)},
    m_googleWrapper{GoogleSSO(this)}
{
    authenticate();
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


 void Kernel::authenticate()
{
     m_googleWrapper.authenticate();
}


void Kernel::getDataFromGoogle()
{
    getProjects();
}

void Kernel::getProjects()
{
    std::optional<QJsonArray> result = m_googleWrapper.getSheetValues("Projekty");

    QList<QStringList> projects;

    if(result.has_value())
    {
        qDebug() << "rows in sheet:";
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
    qDebug() << projects;
    }
}