#ifndef KERNEL_H
#define KERNEL_H

#include <QObject>
#include <QtQml>
#include <QQmlEngine>
#include <QStringList>
#include "DataWrapperManager.h"
#include "googlesso.h"

using namespace std;

class Kernel : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Kernel must be instantiated in C++")
    Q_PROPERTY(bool isAuthenticated READ isAuthenticated WRITE setAuthenticated NOTIFY isAuthenticatedChanged)
public:
    explicit Kernel(QObject *parent = nullptr);
    ~Kernel() = default;


    void setAuthenticated(bool isAuthenticated) {
        if (m_isAuthenticated != isAuthenticated) {
            m_isAuthenticated = isAuthenticated;
            emit isAuthenticatedChanged();
        }
    }

    Q_INVOKABLE bool isAuthenticated()
    {
        return m_isAuthenticated;
    }

    Q_INVOKABLE AbstractAppModel* getModelUrzedy();
    Q_INVOKABLE AbstractAppModel* getModelInwestycje();
    Q_INVOKABLE AbstractAppModel* getModelArchitekci();
    Q_INVOKABLE AbstractAppModel* getModelInwestor();
    Q_INVOKABLE AbstractAppModel* getModelProjekt();

    Q_INVOKABLE void authenticate();
    Q_INVOKABLE QStringList getProjectData(int anIndex);
    Q_INVOKABLE QStringList getArchitektData(const QString & anID);
    Q_INVOKABLE QJsonObject getDoc();
    Q_INVOKABLE void generateDocument(const QString & anID);

public slots:
    void slotCurrentProjektChanged(const QString & aProjektName, int index);


signals:
    void isAuthenticatedChanged();

protected slots:
    void slotSetAuthenticated();

private:
    DataWraperManager m_DataWrapperManager;
    GoogleSSO m_googleWrapper;
    bool m_isAuthenticated {false};

    void getDataFromGoogle();
    void getProjects();
    void getInwestorzy();
    void getArchitekci();

    const QString m_tmpDoc{"1IzeibTeOY8CW_G3AR5KfAsOsVKlbvM461ZvgLKqJ6b4"};

};

#endif // KERNEL_H
