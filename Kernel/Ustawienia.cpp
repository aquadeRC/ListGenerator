#include "Ustawienia.h"

#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>

Ustawienia::Ustawienia(QObject *parent)
    : QObject{parent}
{
}

QString Ustawienia::getProjektyId()
{
    return m_projekty_file_id;
}

QString Ustawienia::getDecyjzeId()
{
    return m_decyje_file_id;
}

QString Ustawienia::getTemplateId()
{
    return m_template_file_id;
}

QString Ustawienia::getCredentialFile()
{
    return m_credentials_file;
}

QString Ustawienia::getDataDir()
{
    return m_data_dir;
}

QStringList Ustawienia::getSettingsData()
{
    QStringList  data ;

    data.append(m_projekty_file_id);
    data.append(m_decyje_file_id);
    data.append(m_template_file_id);
    data.append(m_credentials_file);
    data.append(m_data_dir);

    return data;
}

void Ustawienia::setProjektyId(const QString &aValue)
{
    m_projekty_file_id = aValue;
}

void Ustawienia::setDecyjzeId(const QString &aValue)
{
    m_decyje_file_id = aValue;
}

void Ustawienia::setTemplateId(const QString &aValue)
{
    m_template_file_id = aValue;
}

void Ustawienia::setCredentialFile(const QString &aValue)
{
    m_credentials_file = aValue;
}


void Ustawienia::setSettingsData(const QStringList& aData)
{
     m_projekty_file_id = aData[0];
     m_decyje_file_id = aData[1];
     m_template_file_id = aData[2];
     m_credentials_file = aData[3];
}



void Ustawienia::load()
{
    QDir dir;

    QFile file(dir.absolutePath() + m_data_dir + m_settings_file );
    if (!file.open(QIODevice::ReadOnly))
    {
        QFileInfo info(file);
        QString message = QString("Nie mogę otworzyć pliku %1").arg(info.absoluteFilePath() );
        qWarning(message.toStdString().c_str());

        emit signalError("Bład otwarcia pliku Settings.json");

        return;
    }

    QByteArray loadData = file.readAll();
    file.close();

    QJsonParseError perror;
    QJsonDocument loadDoc(QJsonDocument::fromJson(loadData,&perror));
    if(perror.error != QJsonParseError::NoError)
    {
        QString message = QString("Bład odczytac plik Settings.json %1").arg(perror.errorString());
        qWarning(message.toStdString().c_str());

        emit signalError(message);
    }

    const QJsonObject data = loadDoc.object();
    if (const QJsonValue v = data[m_projekty_file_id_field]; v.isString())
        m_projekty_file_id = v.toString();
    if (const QJsonValue v = data[m_decyje_file_id_field]; v.isString())
        m_decyje_file_id = v.toString();
    if (const QJsonValue v = data[m_template_file_id_field]; v.isString())
        m_template_file_id = v.toString();
    if (const QJsonValue v = data[m_credentials_file_field]; v.isString())
        m_credentials_file = v.toString();
}

void Ustawienia::save()
{
    QDir dir;

    QFile file(dir.absolutePath() + m_data_dir + m_settings_file );
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QFileInfo info(file);
        QString message = QString("Nie mogę otworzyć pliku %1 do zapisu").arg(info.absoluteFilePath() );
        qWarning(message.toStdString().c_str());

        emit signalError(message);

        return;
    }

    QJsonDocument doc;
    QJsonObject data;
    data.insert(m_projekty_file_id_field, m_projekty_file_id);
    data.insert(m_decyje_file_id_field, m_decyje_file_id);
    data.insert(m_template_file_id_field, m_template_file_id);
    data.insert(m_credentials_file_field, m_credentials_file);
    doc.setObject(data);

    file.write(doc.toJson());
    file.close();
}
