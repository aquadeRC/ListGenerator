#ifndef IDATAWRAPPER_H
#define IDATAWRAPPER_H

#include <QStringList>
#include <QString>
#include <QJsonObject>
#include "KernelCommon.h"

class IDataWrapper
{
public:
    explicit IDataWrapper()= default;
    IDataWrapper(DATA_TYPES aType, const QString& aFile)
        :m_type{aType}, m_dataFile{aFile}
    {}
    virtual QString getDataFile()const
    { return m_dataFile;}
    virtual QList<QStringList> read(const QJsonObject &json) const=0;
    virtual DATA_TYPES getType()
    {return m_type;}

    virtual void setType(DATA_TYPES aType)
    {m_type = aType;}
    virtual void setFileName(const QString& aFile)
    {m_dataFile = aFile;}
protected:
    virtual QStringList readData(const QJsonObject &json)const =0;

private:
    QString m_dataFile;
    DATA_TYPES m_type;
};

#endif // IDATAWRAPPER_H
