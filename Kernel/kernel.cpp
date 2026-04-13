
#include "kernel.h"
#include "KernelCommon.h"

#include <QNetworkReply>
#include <QGuiApplication>


Kernel::Kernel(QObject *parent)
    : QObject{parent},
    m_DataWrapperManager {DataWraperManager(this)},
    m_googleWrapper{GoogleSSO(this)},
    m_settins{new Ustawienia(this)}
{
     m_settins->load();
     m_DataWrapperManager.createDataModels();
     m_googleWrapper.init();


    QObject::connect(&m_googleWrapper, &GoogleSSO::isAuthenticatedChanged,
            this, &Kernel::slotSetAuthenticated);

    QObject::connect(qApp, &QGuiApplication::aboutToQuit, this,
                     [&](){m_settins->save();});
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

QStringList Kernel::getSettings()
{
    return Ustawienia::getSettingsData();
}

void  Kernel::setSettings(const QStringList &aData)
{
    Ustawienia::setSettingsData(aData);

    m_settins->load();
    m_googleWrapper.init();

    authenticate();
}



 void Kernel::authenticate()
{
     m_googleWrapper.authenticate();
}

void Kernel::getDataFromGoogle()
{
    getProjects();
    getInwestorzy();
    getArchitekci();
    //m_DataWrapperManager.dumpData();
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

QJsonObject Kernel::getDoc()
{
    std::optional<QJsonObject> result = m_googleWrapper.getDocument(Ustawienia::getTemplateId());
    if(result.has_value())
    {
        return result.value();
    }
    else
        return QJsonObject();
}

void Kernel::generateDocument(const QString & anID)
{
    m_googleWrapper.updateDocument(Ustawienia::getTemplateId(), anID);
}

void Kernel::slotSetAuthenticated()
{
    bool au = m_googleWrapper.isAuthenticated();
    if (m_isAuthenticated != au) {
        m_isAuthenticated = au;

        if(m_isAuthenticated)
            qDebug() << "m_isAuthenticated";
            getDataFromGoogle();

        emit isAuthenticatedChanged();
    }
}

void Kernel::slotCurrentProjektChanged(const QString & aProjektName, int index)
{
    qDebug() <<  "projekt name changed to " << aProjektName << index;
}

QStringList Kernel::getProjectData(int anIndex)
{
    return m_DataWrapperManager.getProjectData(anIndex);
}

QStringList Kernel::getArchitektData(const QString & anID)
{
    return m_DataWrapperManager.getArchitektData(anID);
}