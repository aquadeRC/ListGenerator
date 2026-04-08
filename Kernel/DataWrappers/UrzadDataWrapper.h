#ifndef URZADDATAWRAPPER_H
#define URZADDATAWRAPPER_H

#include "IDataWrapper.h"

class UrzadDataWrapper : public IDataWrapper
{
public:
    explicit UrzadDataWrapper();
    QList<QStringList> read(const QJsonObject &json) const override;
protected:
    QStringList readData(const QJsonObject &json) const override;
};

#endif // URZADDATAWRAPPER_H
