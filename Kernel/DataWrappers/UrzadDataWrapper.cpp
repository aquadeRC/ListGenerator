#include "UrzadDataWrapper.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

UrzadDataWrapper::UrzadDataWrapper()
    : IDataWrapper(DATA_TYPES::URZEDY_DATA , "UrzedyData.json") {}


QList<QStringList > UrzadDataWrapper::read(const QJsonObject &json) const
{
    QList<QStringList> data;

    if (const QJsonValue v = json["urzad"]; v.isArray()) {
        const QJsonArray users = v.toArray();
        data.reserve(users.size());
        for (const QJsonValue &user : users)
            data.append(readData(user.toObject()));
    }
    return data;
}

QStringList UrzadDataWrapper::readData(const QJsonObject &json) const
{
    QStringList result(14);
    if (const QJsonValue v = json["Kod_TERYT"]; v.isDouble())
        result[0] = QString::number(v.toInt());
    if (const QJsonValue v = json["nazwa_samorządu"]; v.isString())
        result[1]= v.toString();
    if (const QJsonValue v = json["Województwo"]; v.isString())
        result[2] = v.toString();
    if (const QJsonValue v = json["Powiat"]; v.isString())
        result[3] = v.toString();
    if (const QJsonValue v = json["nazwa_urzędu_JST"]; v.isString())
        result[4] = v.toString();
    if (const QJsonValue v = json["miejscowość"]; v.isString())
        result[5] = v.toString();
    if (const QJsonValue v = json["Kod pocztowy"]; v.isString())
        result[6] = v.toString();
    if (const QJsonValue v = json["poczta"]; v.isString())
        result[7] = v.toString();
    if (const QJsonValue v = json["Ulica"]; v.isString())
        result[8] = v.toString();

    if (const QJsonValue v = json["Nr domu"]; v.isString())
        result[9] = v.toString();
    else if (const QJsonValue v = json["Nr domu"]; v.isDouble())
        result[9] = QString::number(v.toInt());

    if (const QJsonValue v = json["telefon kierunkowy"]; v.isDouble())
        result[10] = QString::number(v.toInt());

    if (const QJsonValue v = json["telefon"]; v.isString())
        result[11] = v.toString();
    else if (const QJsonValue v = json["telefon"]; v.isDouble())
        result[11] = QString::number(v.toInt());

    if (const QJsonValue v = json["telefon 2"]; v.isString())
        result[12] = v.toString();
    else if (const QJsonValue v = json["telefon 2"]; v.isDouble())
        result[12] = QString::number(v.toInt());


    if (const QJsonValue v = json["email"]; v.isString())
        result[13] = v.toString();

    return result;
}
