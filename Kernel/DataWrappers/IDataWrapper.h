#ifndef IDATAWRAPPER_H
#define IDATAWRAPPER_H

#include <QMap>
#include <QJsonObject>
#include "KernelCommon.h"

class IDataWrapper
{
public:
    explicit IDataWrapper()= default;
    virtual QString getDataFile()const =0;
    virtual QList<QMap<QString, QString>> read(const QJsonObject &json) const=0;
    virtual DATA_TYPES getType() =0;
protected:
    virtual QMap<QString, QString> readData(const QJsonObject &json)const =0;
};

#endif // IDATAWRAPPER_H
