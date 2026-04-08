#ifndef ARCHITEKTDATAWRAPPER_H
#define ARCHITEKTDATAWRAPPER_H

#include "IDataWrapper.h"

class ArchitektDataWrapper : public IDataWrapper
{
public:
    explicit ArchitektDataWrapper();
    QList<QStringList> read(const QJsonObject &json) const override;
protected:
    QStringList readData(const QJsonObject &json) const override;
};

#endif // ARCHITEKTDATAWRAPPER_H
