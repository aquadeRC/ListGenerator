#include "ArchitektDataWrapper.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

ArchitektDataWrapper::ArchitektDataWrapper()
    : IDataWrapper()
{}

QString ArchitektDataWrapper::getDataFile() const
{
    return m_dataFile;
}

DATA_TYPES ArchitektDataWrapper::getType()
{
    return m_type;
}

 QList<QMap<QString, QString>> ArchitektDataWrapper::read(const QJsonObject &json)const
{
     QList<QMap<QString, QString>> data;

     if (const QJsonValue v = json["architek"]; v.isArray()) {
         const QJsonArray users = v.toArray();
         data.reserve(users.size());
         for (const QJsonValue &user : users)
             data.append(readData(user.toObject()));
     }

     return data;
}

QMap<QString, QString> ArchitektDataWrapper::readData(const QJsonObject &json)const
{
    QMap<QString, QString> result;
    if (const QJsonValue v = json["nazwa"]; v.isString())
        result["nazwa"] = v.toString();
    if (const QJsonValue v = json["telefon"]; v.isString())
        result["telefon"]= v.toString();
    if (const QJsonValue v = json["email"]; v.isString())
        result["email"] = v.toString();

    return result;
}
