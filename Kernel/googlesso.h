#ifndef GOOGLESSO_H
#define GOOGLESSO_H

#include <QObject>
#include <QNetworkReply>
#include <QOAuth2AuthorizationCodeFlow>
#include <QPointer>
#include <QNetworkRequestFactory>
#include <QNetworkAccessManager>
#include <QRestAccessManager>
#include <QOAuthHttpServerReplyHandler>

#include "DataTypes/CredentialsData.h"

class GoogleSSO : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isAuthenticated READ isAuthenticated WRITE setAuthenticated NOTIFY isAuthenticatedChanged)

public:
    explicit GoogleSSO(QObject *parent = nullptr);
    virtual ~GoogleSSO();

    void setAuthenticated(bool isAuthenticated) {
        if (m_isAuthenticated != isAuthenticated) {
            m_isAuthenticated = isAuthenticated;
            emit isAuthenticatedChanged();
        }
    }

    bool isAuthenticated() const {
        return m_isAuthenticated;
    }

    QString getToken() {
        return m_activeToken;
    }

    QString getError() {
        return m_errorMessage;
    }

    std::optional<QJsonArray> getSheetValues(const QString & aRange);

public slots:
    void setCredentials(const QString& clientId,
                        const QString& clientSecret);
    void authenticate();
    void slotNetworkError(QNetworkReply::NetworkError anError);
    void slotSetErrorMessage(const QString & anError);

signals:
    void gotToken(const QString& token);
    void signalError(const QString& anError);
    void isAuthenticatedChanged();

protected slots:
    void slotSSLErrorHandler(QNetworkReply* aReplay,
                             const QList<QSslError> & anErrorList);

private:
    void init();
    std::optional<QJsonObject> getReplay(const QString& aUrl);

    bool readCredentials();

    QPointer<QOAuth2AuthorizationCodeFlow> m_google {nullptr};
    QPointer<QNetworkAccessManager> m_networkManager {nullptr};
    QPointer<QRestAccessManager> m_restManager {nullptr};
    QPointer<QOAuthHttpServerReplyHandler> m_replyHandler {nullptr};


    QNetworkRequestFactory m_sheetFactory {};
    QNetworkReply::NetworkError m_neworkReplyError {QNetworkReply::NoError};
    QString m_activeToken {};
    QString m_errorMessage {};
    QString m_scope {"https://www.googleapis.com/auth/spreadsheets.readonly"};
    QString m_sheetsEndPoint {"https://sheets.googleapis.com/v4"};
    QString m_credentialsFile{"credentials.json"};
    bool m_isAuthenticated {false};

    Modele_Danych::CredentialsData m_credentials{};
};

#endif // GOOGLESSO_H
