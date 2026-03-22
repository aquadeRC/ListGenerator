#ifndef KERNEL_H
#define KERNEL_H

#include <QObject>
#include <QtQml>
#include <QQmlEngine>
#include <QStringList>
#include "DataWrapperManager.h"

using namespace std;

class Kernel : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Kernel must be instantiated in C++")
public:
    explicit Kernel(QObject *parent = nullptr);
    ~Kernel() = default;

    Q_INVOKABLE QList<QMap<QString, QString>> getModelUrzedy();
    Q_INVOKABLE QStringList getModelInwestycje();
    Q_INVOKABLE QList<QMap<QString, QString>> getModelArchitekci();
    Q_INVOKABLE QStringList getModelSpraw();

signals:

private:
    void init();
    DataWraperManager m_DataWrapperManager;
};

#endif // KERNEL_H
