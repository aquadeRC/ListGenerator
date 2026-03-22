#ifndef URZADDATAWRAPPER_H
#define URZADDATAWRAPPER_H

#include "IDataWrapper.h"

class UrzadDataWrapper : public IDataWrapper
{
public:
    explicit UrzadDataWrapper();
    QList<QMap<QString, QString>> read(const QJsonObject &json) const override;
protected:
    QMap<QString, QString> readData(const QJsonObject &json) const override;
};

#endif // URZADDATAWRAPPER_H
