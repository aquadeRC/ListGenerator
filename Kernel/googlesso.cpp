#include "googlesso.h"

#include <QString>
#include <QDir>
#include <QUrl>
#include <QOAuthHttpServerReplyHandler>
#include <QDesktopServices>
#include <QtNetworkAuth>

#include "GDocUpdateShema.h"
#include "Ustawienia.h"



GoogleSSO::GoogleSSO(QObject *parent)
    : QObject(parent)
{
    m_google = new QOAuth2AuthorizationCodeFlow(this);
    m_networkManager = new QNetworkAccessManager(this);
    m_restManager = new QRestAccessManager(m_networkManager, this);
    m_requestFactory = QNetworkRequestFactory(m_sheetsEndPoint);

   // init();
}

GoogleSSO::~GoogleSSO() {
    delete m_google;
}


bool GoogleSSO::init ()
{
    m_credFileRead= readCredentials();
    if(m_credFileRead)
    {
       init_internal();
    }

    return m_credFileRead;
}
void GoogleSSO::init_internal(){
    QSet<QByteArray> scope;
    scope.insert("https://www.googleapis.com/auth/spreadsheets.readonly");
    scope.insert("https://www.googleapis.com/auth/documents");
    scope.insert("https://www.googleapis.com/auth/drive");
    scope.insert("https://www.googleapis.com/auth/drive.metadata.readonly");
    m_google->setRequestedScopeTokens(scope);
    //m_google->setScope(m_scope);

   // connect(m_networkManager,
     //       SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError> &)), this,
       //     SLOT(slotSSLErrorHandler(QNetworkReply*, const QList<QSslError> &)));

    connect(m_networkManager,
            &QNetworkAccessManager::sslErrors, this,
            &GoogleSSO::slotSSLErrorHandler
            );

    connect(m_google,
            &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser,
            &QDesktopServices::openUrl);

    connect(m_google,
            &QOAuth2AuthorizationCodeFlow::statusChanged,
            [=](QAbstractOAuth::Status status) {
                if (status == QAbstractOAuth::Status::Granted) {

                    const QString token = m_google->token();
                    const QString client = m_google->clientIdentifier();
                    const QUrl authho = m_google->authorizationUrl();
                    auto status = m_google->status();
                    auto extraT = m_google->extraTokens();

                    qDebug() << extraT;
                    m_activeToken = token;
                    qlonglong expiredAt = extraT["refresh_token_expires_in"].toLongLong();

                    m_expiredAt = QTime::currentTime();
                    m_expiredAt = m_expiredAt.addMSecs(expiredAt);

                    m_requestFactory.setBearerToken(token.toLatin1());

                    emit gotToken(token);

                    writeTokenFile();

                    setAuthenticated(true);
                } else {
                    setAuthenticated(false);
                }
            });

    const QUrl authUri(m_credentials.m_auth_uri);
    const QUrl tokenUri(m_credentials.m_token_uri);
    const QUrl redirectUri(m_credentials.m_redirect_uri);

    const auto port = static_cast<quint16>(redirectUri.port());

    m_google->setAuthorizationUrl(authUri);
    m_google->setClientIdentifier(m_credentials.m_client_id);
    m_google->setTokenUrl(tokenUri);
    m_google->setClientIdentifierSharedKey(m_credentials.m_client_secret);

    m_google->setModifyParametersFunction(
        [&](QAbstractOAuth::Stage stage, QMultiMap<QString, QVariant> *parameters) {
            if (stage == QAbstractOAuth::Stage::RequestingAccessToken) {
                QByteArray code = parameters->value("code").toByteArray();
                parameters->insert("code", QUrl::fromPercentEncoding(code));
            }
        });

    m_replyHandler = new QOAuthHttpServerReplyHandler(port, this);
    m_google->setReplyHandler(m_replyHandler);
}

void GoogleSSO::setCredentials(const QString& clientId, const QString& clientSecret) {
    m_google->setClientIdentifier(clientId);
    m_google->setClientIdentifierSharedKey(clientSecret);
}

// Invoked externally to initiate
void GoogleSSO::authenticate() {

    if(m_credFileRead)
    {
        bool credsFromTokenFile = checkTokenFile();
        if(credsFromTokenFile == false)
        {
            m_google->grant();
        }
        else
        {
            m_requestFactory.setBearerToken(m_activeToken.toLatin1());

            setAuthenticated(true);
        }
    }

}

std::optional<QJsonArray> GoogleSSO::getSheetValues(const QString & aRange)
{
    QString url = QString("/spreadsheets/%1/values/%2")
                      .arg(Ustawienia::getProjektyId(), aRange);

    std::optional<QJsonObject> resultObject = getReplay(m_sheetsEndPoint, url);
    if(resultObject.has_value())
    {
       QJsonObject result =  resultObject.value();
        if (result.contains("values") == false)
        {
           slotSetErrorMessage("GoogleSSO::getSheetValues result did not contain the values string.");
            return std::nullopt;
        }

        QJsonValue token = result.value("values");
        QJsonArray values = token.toArray();
        return values;
    }

    return std::nullopt;
}

std::optional<QJsonObject> GoogleSSO::getDocument(const QString & anId)
{
    QString url = QString("/documents/%1").arg(anId);

    std::optional<QJsonObject> resultObject = getReplay(m_docEndPoint, url);
    if(resultObject.has_value())
    {
        QJsonObject result =  resultObject.value();
        return result;
    }

    return std::nullopt;
}

std::optional<QString> GoogleSSO::copyDocument(const QString & anId, const QString &newName)
{
    QString url = QString("/files/%1/copy").arg(anId);
    std::string rawData =R"({"name": "%1"})";
    QString dataString = QString::fromUtf8(rawData).arg(newName);

    QJsonDocument doc =  QJsonDocument::fromJson(dataString.toUtf8());
    QByteArray requestData = doc.toJson(QJsonDocument::Compact);

    std::optional<QJsonObject> updateResult = postReplay(m_fileEndPoint, url, requestData);
    if(updateResult.has_value())
    {
        QJsonObject result =  updateResult.value();
        if (result.contains("id") == false)
        {
            slotSetErrorMessage("GoogleSSO::copyDocument result did not contain the id string.");
            return std::nullopt;
        }

        QString newFileId = result.value("id").toString();
        return newFileId;
    }
    return std::nullopt;
}


void GoogleSSO::updateDocument(const QString & anId, const QString & newName)
{
   //copy document
    std::optional<QString> copyResult = copyDocument(anId, newName);
   if(copyResult.has_value())
    {
        QString newFileId = copyResult.value();

       // get tmp document
       std::optional<QJsonObject> resp = getDocument(newFileId);
       if(resp.has_value())
       {
           //change data
           QString url = QString("/documents/%1:batchUpdate").arg(newFileId);

           GDocUpdateShema shema;
           QString dataString = QString::fromUtf8(shema.rawData)
                                    .arg("aa","aa","aa","aa","aa","aa","aa","aa","aa","aa","aa","aa","aa","aa","aa","aa","aa","aa","aa","aa","aa","aa");

           QJsonDocument doc =  QJsonDocument::fromJson(dataString.toUtf8());
           QByteArray requestData = doc.toJson(QJsonDocument::Compact);

           std::optional<QJsonObject> updateResult = postReplay(m_docEndPoint, url, requestData);
           if(updateResult.has_value())
           {
               QJsonObject result =  updateResult.value();
               if (result.contains("replaceAllText") == false)
               {
                   slotSetErrorMessage("GoogleSSO::updateDocument result did not contain the replaceAllText string.");
                   return;
               }

               QJsonValue token = result.value("replaceAllText");
               QJsonArray values = token.toArray();
               return;
           }
       }
    }
    return;
}

 std::optional<QJsonObject> GoogleSSO::postReplay(const QString & endPoint, const QString& aUrl,
                                                 const QByteArray &aData)
{
     if(m_restManager.isNull())
     {
         slotSetErrorMessage(QString("GoogleSSO::postReplay m_restManager is NULL!"));

        return std::nullopt;
     }

    m_requestFactory.setBaseUrl(QUrl(endPoint));
    QNetworkReply *reply = m_restManager->post(m_requestFactory.createRequest(aUrl), aData);
    if(reply)
    {
        connect(reply,
                SIGNAL(errorOccurred(QNetworkReply::NetworkError)), this,
                SLOT(slotNetworkError(QNetworkReply::NetworkError)));

        if (!reply->isFinished())
        {
            QEventLoop loop;
            QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));

            if (m_timeout > 0)
                QTimer::singleShot(m_timeout, &loop, SLOT(quit()));

            loop.exec(QEventLoop::ExcludeUserInputEvents);
        }


        // If we are not done then we timed out.
        if (!reply->isFinished())
        {
            reply->abort();
            reply->deleteLater();

            if (m_timeout > 0)
            {
                slotSetErrorMessage(QString("POST reply timed out after %1 milliseconds.").arg(m_timeout));
            }
            return std::nullopt;
        }

        m_neworkReplyError = reply->error();
        if (m_neworkReplyError != QNetworkReply::NoError)
        {
            slotSetErrorMessage(reply->errorString());
            reply->deleteLater();
            return std::nullopt;
        }

        QRestReply restReply(reply);
        if (restReply.isSuccess())
        {
            QJsonParseError jsonParseError;

            quint64 bytesAvailable = reply->bytesAvailable();
            QByteArray replyData = reply->read(bytesAvailable);
            QJsonDocument replyJson = QJsonDocument::fromJson(replyData, &jsonParseError);

            if (jsonParseError.error != QJsonParseError::NoError)
            {
                slotSetErrorMessage(jsonParseError.errorString());
                reply->deleteLater();
                return std::nullopt;
            }

            if (!replyJson.isObject())
            {
                slotSetErrorMessage("POST result was not a JSON object as expected.");
                reply->deleteLater();
                return std::nullopt;
            }

            return replyJson.object();
        }
        else
        {
            return std::nullopt;
        }
    }
    else
    {
        slotSetErrorMessage("QRestAccessManager::post return NULL!");
        return std::nullopt;;
    }

    return std::nullopt;

}


std::optional<QJsonObject> GoogleSSO::getReplay(const QString & endPoint, const QString& aUrl)
{
    if(m_restManager.isNull())
        return std::nullopt;

    m_requestFactory.setBaseUrl(QUrl(endPoint));
    QNetworkReply *reply = m_restManager->get(m_requestFactory.createRequest(aUrl));
    if(reply)
    {
        connect(reply,
                SIGNAL(errorOccurred(QNetworkReply::NetworkError)), this,
                SLOT(slotNetworkError(QNetworkReply::NetworkError)));

        if (!reply->isFinished())
        {
            QEventLoop loop;
            QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));

            if (m_timeout > 0)
                QTimer::singleShot(m_timeout, &loop, SLOT(quit()));

            loop.exec(QEventLoop::ExcludeUserInputEvents);
        }

        if (!reply->isFinished())
        {
            reply->abort();
            reply->deleteLater();

            if (m_timeout > 0)
            {
                slotSetErrorMessage(QString("GET reply timed out after %1 milliseconds.").arg(m_timeout));
            }
            return std::nullopt;
        }

        m_neworkReplyError = reply->error();
        if (m_neworkReplyError != QNetworkReply::NoError)
        {
            slotSetErrorMessage(reply->errorString());
            reply->deleteLater();
            return std::nullopt;
        }

        QRestReply restReply(reply);
        if (restReply.isSuccess())
        {
            QJsonParseError jsonParseError;

            quint64 bytesAvailable = reply->bytesAvailable();
            QByteArray replyData = reply->read(bytesAvailable);
            QJsonDocument replyJson = QJsonDocument::fromJson(replyData, &jsonParseError);

            if (jsonParseError.error != QJsonParseError::NoError)
            {
                slotSetErrorMessage(jsonParseError.errorString());
                reply->deleteLater();
                return std::nullopt;
            }

            if (!replyJson.isObject())
            {
                slotSetErrorMessage("POST result was not a JSON object as expected.");
                reply->deleteLater();
                return std::nullopt;
            }

            return replyJson.object();
        }
        else
        {
            return std::nullopt;
        }


        /*
        QObject::connect(reply, &QNetworkReply::finished, this, [reply]() {
            // The reply may be wrapped in the finish handler:
            QRestReply restReply(reply);
            if (restReply.isSuccess())
            {
                QJsonParseError jsonParseError;

                quint64 bytesAvailable = reply->bytesAvailable();
                QByteArray replyData = reply->read(bytesAvailable);
                QJsonDocument replyJson = QJsonDocument::fromJson(replyData, &jsonParseError);

                if (jsonParseError.error != QJsonParseError::NoError)
                {
                    qDebug() << jsonParseError.errorString();
                    reply->deleteLater();
                    return QJsonObject();
                }

                if (!replyJson.isObject())
                {
                    qDebug() <<  QString("POST result was not a JSON object as expected.");
                    reply->deleteLater();
                    return QJsonObject();
                }

                return replyJson.object();
            }
            return QJsonObject();
        });
*/
    }
    else
    {
        slotSetErrorMessage("QRestAccessManager::get return NULL!");
        return std::nullopt;;
    }

    return std::nullopt;
}


void GoogleSSO::slotNetworkError(QNetworkReply::NetworkError anError)
{
    m_neworkReplyError = anError;

    QNetworkReply *reply = dynamic_cast<QNetworkReply*>(sender());
    if (reply){

        QString errorMesage = reply->errorString();

        QJsonParseError jsonParseError;
        quint64 bytesAvailable = reply->bytesAvailable();
        QByteArray replyData = reply->read(bytesAvailable);
        QJsonDocument replyJson = QJsonDocument::fromJson(replyData, &jsonParseError);

        if(replyJson.isObject())
        {
            QJsonObject resultObj = replyJson.object();
            QJsonObject errorObj = resultObj["error"].toObject();

            int code = errorObj["code"].toInt();
            QString message = errorObj["message"].toString();

            errorMesage = QString("Network error, code: %1 :%2").arg(code).arg(message);
        }

        slotSetErrorMessage(errorMesage);
    }
    emit signalError(m_errorMessage);
}

void GoogleSSO::slotSSLErrorHandler(QNetworkReply* aReplay, const QList<QSslError> & anErrorList)
{
    bool ignoreErrors = true;

    for(QSslError err : anErrorList)
    {
        switch (err.error())
        {
        case QSslError::SelfSignedCertificate:
            break;
        case QSslError::SelfSignedCertificateInChain:
            break;
        default:
            ignoreErrors = false;
            break;
        }
    }
    aReplay->ignoreSslErrors(anErrorList);
}



void GoogleSSO::slotSetErrorMessage(const QString & anError)
{
    m_errorMessage = anError;
    qDebug()<< anError;

    emit signalError(m_errorMessage);
}

bool GoogleSSO::readCredentials()
{
    QDir dir;
    QFile file(dir.absolutePath() + Ustawienia::getDataDir() + Ustawienia::getCredentialFile());
    if (!file.open(QIODevice::ReadOnly))
    {
        QFileInfo info(file);
        QString message = QString("Nie mogę otworzyć pliku %1").arg(info.absoluteFilePath() );
        emit signalError(message);

        qWarning(message.toStdString().c_str());

        return false;
    }

    QByteArray loadData = file.readAll();
    file.close();

    QJsonParseError perror;
    QJsonDocument loadDoc(QJsonDocument::fromJson(loadData, &perror));
    if(perror.error != QJsonParseError::NoError)
    {
        qWarning(perror.errorString().toStdString().c_str());
        emit signalError(perror.errorString());

        return false;
    }

    QJsonObject credObj = loadDoc.object();
    if (const QJsonValue v = credObj["installed"]; v.isObject()) {
        const QJsonObject credData = v.toObject();

        if (const QJsonValue v = credData["client_id"]; v.isString())
            m_credentials.m_client_id = v.toString();
        if (const QJsonValue v = credData["project_id"]; v.isString())
            m_credentials.m_project_id= v.toString();
        if (const QJsonValue v = credData["auth_uri"]; v.isString())
            m_credentials.m_auth_uri = v.toString();
        if (const QJsonValue v = credData["token_uri"]; v.isString())
            m_credentials.m_token_uri = v.toString();
        if (const QJsonValue v = credData["auth_provider_x509_cert_url"]; v.isString())
            m_credentials.m_token_provider = v.toString();
        if (const QJsonValue v = credData["client_secret"]; v.isString())
            m_credentials.m_client_secret = v.toString();
        if (const QJsonValue v = credData["redirect_uris"]; v.isString())
            m_credentials.m_redirect_uri = v.toString();

        return true;
    }
    return false;
}

bool GoogleSSO::checkTokenFile()
{
    QDir dir;
    QFile file(dir.absolutePath() + Ustawienia::getDataDir() + m_tokenFile );
    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QByteArray loadData = file.readAll();
    file.close();

    QJsonParseError perror;
    QJsonDocument loadDoc(QJsonDocument::fromJson(loadData, &perror));
    if(perror.error != QJsonParseError::NoError)
    {
        qWarning(perror.errorString().toStdString().c_str());
        emit signalError(perror.errorString());

        return false;
    }

    QJsonObject credObj = loadDoc.object();
    QTime time;
    QString tmpToken;

    if (const QJsonValue v = credObj["refresh_token"]; v.isString())
    {
        const QString ts = v.toString();
        time = QTime::fromString(ts, "HH:mm:ss");
    }
    if (const QJsonValue v = credObj["token"]; v.isString())
        tmpToken = v.toString();

    if(QTime::currentTime() > time)
    {
        return false;
    }
    m_activeToken = tmpToken;

    return true;
}

 void GoogleSSO::writeTokenFile()
{
     QDir dir;
     QFile file(dir.absolutePath() + Ustawienia::getDataDir() + m_tokenFile );
     if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
     {
         return;
     }

     QJsonObject root;
     root.insert("token", m_activeToken);
     root.insert("refresh_token", m_expiredAt.toString());

     QJsonDocument doc;
     doc.setObject(root);

     file.write(doc.toJson());
     file.close();
}