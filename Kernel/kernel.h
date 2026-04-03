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
public:
    explicit Kernel(QObject *parent = nullptr);
    ~Kernel() = default;

    Q_INVOKABLE AbstractAppModel* getModelUrzedy();
    Q_INVOKABLE AbstractAppModel* getModelInwestycje();
    Q_INVOKABLE AbstractAppModel* getModelArchitekci();
    Q_INVOKABLE AbstractAppModel* getModelInwestor();
    Q_INVOKABLE void authenticate();

    Q_INVOKABLE void getProjects();

signals:


private:
    DataWraperManager m_DataWrapperManager;
    GoogleSSO m_googleWrapper;
};

#endif // KERNEL_H
