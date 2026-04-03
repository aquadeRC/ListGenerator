#include "googlesso.h"

#include <QString>
#include <QDir>
#include <QUrl>
#include <QOAuthHttpServerReplyHandler>
#include <QDesktopServices>
#include <QtNetworkAuth>

// Get these from https://console.developers.google.com/apis/credentials
#define CLIENT_ID "318197049534-ag95mu1j6cdcj16q1s0bsmrgv2jkpudm.apps.googleusercontent.com"
#define CLIENT_SECRET "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"
#define AUTH_URI "https://accounts.google.com/o/oauth2/auth"
#define TOKEN_URI "https://oauth2.googleapis.com/token"
#define REDIRECT_URI "http://localhost" // "http://127.0.0.1:1234/"

#define SAMPLE_SPREADSHEET_ID "1g1S-g0OqpXYvv0SxqhUCo-JUIF3hshegyjcByQbeI9I"
#define SAMPLE_RANGE_NAME "Projekty!A2:J2"

GoogleSSO::GoogleSSO(QObject *parent)
    : QObject(parent),
      m_isAuthenticated(false) {

    m_google = new QOAuth2AuthorizationCodeFlow(this);
    m_networkManager = new QNetworkAccessManager(this);
    m_restManager = new QRestAccessManager(m_networkManager, this);

    init();
}

GoogleSSO::~GoogleSSO() {
    delete m_google;
}

void GoogleSSO::init(){
    m_google->setScope("https://www.googleapis.com/auth/spreadsheets.readonly");

    m_sheetFactory = QNetworkRequestFactory({"https://sheets.googleapis.com/v4"});


    connect(m_networkManager,
            SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError> &)), this,
            SLOT(slotSSLErrorHandler(QNetworkReply*, const QList<QSslError> &)));

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

    const QUrl authUri(AUTH_URI);
    const auto clientId = CLIENT_ID;
    const QUrl tokenUri(TOKEN_URI);
    const auto clientSecret(CLIENT_SECRET);
    const QUrl redirectUri(REDIRECT_URI);
    const auto port = static_cast<quint16>(redirectUri.port());

    m_google->setAuthorizationUrl(authUri);
    m_google->setClientIdentifier(clientId);
    m_google->setTokenUrl(tokenUri);
    m_google->setClientIdentifierSharedKey(clientSecret);

    m_google->setModifyParametersFunction(
        [&](QAbstractOAuth::Stage stage, QMultiMap<QString, QVariant> *parameters) {
            if (stage == QAbstractOAuth::Stage::RequestingAccessToken) {
                QByteArray code = parameters->value("code").toByteArray();
                parameters->insert("code", QUrl::fromPercentEncoding(code));
            }
        });


    QOAuthHttpServerReplyHandler* replyHandler = new QOAuthHttpServerReplyHandler(port, this);
    m_google->setReplyHandler(replyHandler);

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