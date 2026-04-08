#include "ArchitektDataWrapper.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

ArchitektDataWrapper::ArchitektDataWrapper()
    : IDataWrapper(DATA_TYPES::ARCHITEKT_DATA, "ArchitektData.json")
{}

 QList<QStringList> ArchitektDataWrapper::read(const QJsonObject &json)const
{
     QList<QStringList> data;

     if (const QJsonValue v = json["architek"]; v.isArray()) {
         const QJsonArray users = v.toArray();
         data.reserve(users.size());
         for (const QJsonValue &user : users)
             data.append(readData(user.toObject()));
     }
     return data;
}

QStringList ArchitektDataWrapper::readData(const QJsonObject &json)const
{
    QStringList result(3);
    if (const QJsonValue v = json["nazwa"]; v.isString())
        result[0] = v.toString();
    if (const QJsonValue v = json["telefon"]; v.isString())
        result[1]= v.toString();
    if (const QJsonValue v = json["email"]; v.isString())
        result[2] = v.toString();

    return result;
}
