#ifndef KERNEL_H
#define KERNEL_H

#include <QObject>
#include <QtQml>
#include <QQmlEngine>
#include <QStringList>
#include <QSharedPointer>
#include "DataWrapperManager.h"
#include "googlesso.h"
#include "Ustawienia.h"

using namespace std;

class Kernel : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Kernel must be instantiated in C++")
    Q_PROPERTY(bool isAuthenticated READ isAuthenticated WRITE setAuthenticated NOTIFY isAuthenticatedChanged)
public:
    enum LogLewels{
        LOG_INFO =0,
        LOG_ERROR,
        LOG_WARNING
    };
    explicit Kernel(QObject *parent = nullptr);
    ~Kernel() = default;


    void setAuthenticated(bool isAuthenticated) {
        if (m_isAuthenticated != isAuthenticated) {
            m_isAuthenticated = isAuthenticated;


            emit isAuthenticatedChanged(m_isAuthenticated);
        }
    }

    Q_INVOKABLE bool isAuthenticated()
    {
        return m_isAuthenticated;
    }

    Q_INVOKABLE FilterModel* getModelUrzedy();
    Q_INVOKABLE AbstractAppModel* getModelInwestycje();
    Q_INVOKABLE AbstractAppModel* getModelArchitekci();
    Q_INVOKABLE AbstractAppModel* getModelInwestor();
    Q_INVOKABLE AbstractAppModel* getModelProjekt();
    Q_INVOKABLE AbstractAppModel* getModelPracownia();
    Q_INVOKABLE QStringList getWnioskiList(const QString &anProjectId);

    Q_INVOKABLE void authenticate();
    Q_INVOKABLE QString generateDocument(const QString & anID, const QVariantMap  &aData);
    Q_INVOKABLE QStringList getProjectData(int anIndex); 
    Q_INVOKABLE QStringList getArchitektData(const QString & anID);
    Q_INVOKABLE QStringList getPracowniaData(const QString & anID);
    Q_INVOKABLE QStringList getWniosekData(const QString &anProjectId, const QString &aEwidencjaId);

    Q_INVOKABLE QStringList getSettings();
    Q_INVOKABLE void setSettings(const QStringList &aData);

    Q_INVOKABLE QUrl getDocPdfPath(const QStringList &docId = QStringList());
    Q_INVOKABLE QString getDocId();


public slots:
    void slotCurrentProjektChanged(const QString & aProjektName, int index);


signals:
    void isAuthenticatedChanged(bool aStatus);
    void isError(const QString & aData);
    void signalDocumentCreated(const QString& aName);

protected slots:
    void slotSetAuthenticated();
    void slotSooError(const QString &error);



private:
    DataWraperManager m_DataWrapperManager;
    GoogleSSO m_googleWrapper;
    QSharedPointer<Ustawienia> m_settins;
    QSharedPointer<QStringListModel> m_wnioskiModel;

    QMap<QString, QString> m_updateTokensMap;
    QString m_logName {"log.txt"};

    bool m_isAuthenticated {false};

    void getDataFromGoogle();
    void getProjects();
    void getInwestorzy();
    void getArchitekci();
    void getWnioski();
    void getPacownie();
    QString createUpdateData(const QVariantMap  &aData);
    void addLog(const QString & aMessage, LogLewels alevel);
};

#endif // KERNEL_H
