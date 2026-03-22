#include "UrzadDataWrapper.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

UrzadDataWrapper::UrzadDataWrapper()
    : IDataWrapper(DATA_TYPES::URZEDY_DATA , "UrzedyData.json") {}


QList<QMap<QString, QString> > UrzadDataWrapper::read(const QJsonObject &json) const
{
    QList<QMap<QString, QString>> data;

    if (const QJsonValue v = json["urzad"]; v.isArray()) {
        const QJsonArray users = v.toArray();
        data.reserve(users.size());
        for (const QJsonValue &user : users)
            data.append(readData(user.toObject()));
    }

    return data;
}


QMap<QString, QString> UrzadDataWrapper::readData(const QJsonObject &json) const
{
    QMap<QString, QString> result;
    if (const QJsonValue v = json["Kod_TERYT"]; v.isString())
        result["Kod_TERYT"] = v.toString();
    if (const QJsonValue v = json["nazwa_samorządu"]; v.isString())
        result["nazwa_samorządu"]= v.toString();
    if (const QJsonValue v = json["Województwo"]; v.isString())
        result["Województwo"] = v.toString();
    if (const QJsonValue v = json["Powiat"]; v.isString())
        result["Powiat"] = v.toString();
    if (const QJsonValue v = json["nazwa_urzędu_JST"]; v.isString())
        result["nazwa_urzędu_JST"] = v.toString();
    if (const QJsonValue v = json["miejscowość"]; v.isString())
        result["miejscowość"] = v.toString();
    if (const QJsonValue v = json["Kod pocztowy"]; v.isString())
        result["Kod pocztowy"] = v.toString();
    if (const QJsonValue v = json["poczta"]; v.isString())
        result["poczta"] = v.toString();
    if (const QJsonValue v = json["Ulica"]; v.isString())
        result["Ulica"] = v.toString();
    if (const QJsonValue v = json["Nr domu"]; v.isString())
        result["Nr domu"] = v.toString();
    if (const QJsonValue v = json["telefon kierunkowy"]; v.isString())
        result["telefon kierunkowy"] = v.toString();
    if (const QJsonValue v = json["telefon"]; v.isString())
        result["telefon"] = v.toString();
    if (const QJsonValue v = json["telefon 2"]; v.isString())
        result["telefon 2"] = v.toString();
    if (const QJsonValue v = json["ogólny adres poczty elektronicznej gminy/powiatu/województwa"]; v.isString())
        result["email"] = v.toString();


    return result;
}
