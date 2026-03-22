#ifndef ARCHITEKTDATAWRAPPER_H
#define ARCHITEKTDATAWRAPPER_H

#include "IDataWrapper.h"

class ArchitektDataWrapper : public IDataWrapper
{
public:
    explicit ArchitektDataWrapper();
    QString getDataFile() const override;
    QList<QMap<QString, QString>> read(const QJsonObject &json) const override;
    DATA_TYPES getType() override;
protected:
    QMap<QString, QString> readData(const QJsonObject &json) const override;

private:
    QString m_dataFile;
    DATA_TYPES m_type{DATA_TYPES::ARCHITEKT_DATA};
};

#endif // ARCHITEKTDATAWRAPPER_H
