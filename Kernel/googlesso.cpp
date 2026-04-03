#include "googlesso.h"

#include <QString>
#include <QDir>
#include <QUrl>
#include <QOAuthHttpServerReplyHandler>
#include <QDesktopServices>
#include <QtNetworkAuth>


#define SAMPLE_SPREADSHEET_ID "1g1S-g0OqpXYvv0SxqhUCo-JUIF3hshegyjcByQbeI9I"

GoogleSSO::GoogleSSO(QObject *parent)
    : QObject(parent),
      m_isAuthenticated(false) {

    m_google = new QOAuth2AuthorizationCodeFlow(this);
    m_networkManager = new QNetworkAccessManager(this);
    m_restManager = new QRestAccessManager(m_networkManager, this);
    m_sheetFactory = QNetworkRequestFactory(m_sheetsEndPoint);

    init();
}

GoogleSSO::~GoogleSSO() {
    delete m_google;
}

void GoogleSSO::init(){
    m_google->setScope(m_scope);

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
                    setAuthenticated(true);
                } else {
                    setAuthenticated(false);
                }
            });

    connect(m_google,
            &QOAuth2AuthorizationCodeFlow::granted, this,
            [this]{
                m_sheetFactory.setBearerToken(m_google->token().toLatin1());
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

    connect(m_google,
            &QOAuth2AuthorizationCodeFlow::granted,
            [&](){
                const QString token = m_google->token();
                m_activeToken = token;
                qDebug() << "token:" << m_activeToken;
                emit gotToken(token);

                // Alternatively, just use the token for your purposes
                //        auto reply = this->google->get(QUrl("https://people.googleapis.com/v1/{resourceName=people/me}"));
                //        connect(reply, &QNetworkReply::finished, [reply](){
                //            qInfo() << reply->readAll();
                //        });
            });
}

void GoogleSSO::setCredentials(const QString& clientId, const QString& clientSecret) {
    m_google->setClientIdentifier(clientId);
    m_google->setClientIdentifierSharedKey(clientSecret);
}

// Invoked externally to initiate
void GoogleSSO::authenticate() {
    m_google->grant();
}

std::optional<QJsonArray> GoogleSSO::getSheetValues(const QString & aRange)
{
    QString url = QString("/spreadsheets/%1/values/%2")
                      .arg(SAMPLE_SPREADSHEET_ID, aRange);

    std::optional<QJsonObject> resultObject = getReplay(url);
    if(resultObject.has_value())
    {
       QJsonObject result =  resultObject.value();
        if (result.contains("values") == false)
        {
           slotSetErrorMessage("POST result did not contain the expire time string.");
            return std::nullopt;
        }

        QJsonValue token = result.value("values");
        QJsonArray values = token.toArray();
        return values;
    }

    return std::nullopt;
}

std::optional<QJsonObject> GoogleSSO::getReplay(const QString& aUrl)
{
    if(m_restManager.isNull())
        return std::nullopt;

    QNetworkReply *reply = m_restManager->get(m_sheetFactory.createRequest(aUrl));
    if(reply)
    {
        connect(reply,
                SIGNAL(errorOccurred(QNetworkReply::NetworkError)), this,
                SLOT(slotNetworkError(QNetworkReply::NetworkError)));

        int timeout = 5000;
        if (!reply->isFinished())
        {
            QEventLoop loop;
            QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));

            if (timeout > 0)
                QTimer::singleShot(timeout, &loop, SLOT(quit()));

            loop.exec(QEventLoop::ExcludeUserInputEvents);
        }


        // If we are not done then we timed out.
        if (!reply->isFinished())
        {
            reply->abort();
            reply->deleteLater();

            if (timeout > 0)
            {
                slotSetErrorMessage(QString("GET reply timed out after %1 milliseconds.").arg(timeout));
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
        slotSetErrorMessage(reply->errorString());
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

    emit signalError(m_errorMessage);
}

bool GoogleSSO::readCredentials()
{
    QDir dir;
    QFile file(dir.absolutePath() + m_credentialsFile );
    if (!file.open(QIODevice::ReadOnly))
    {
        QFileInfo info(file);
        QString message = QString("Nie mogę otworzyć pliku %1").arg(info.absoluteFilePath() );
        qWarning(message.toStdString().c_str());

        return false;
    }

    QByteArray loadData = file.readAll();
    file.close();

    QJsonParseError perror;
    QJsonDocument loadDoc(QJsonDocument::fromJson(loadData,&perror));
    if(perror.error != QJsonParseError::NoError)
    {
        qWarning(perror.errorString().toStdString().c_str());
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