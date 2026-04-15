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

    bool init();

    std::optional<QStringList> getSheets(const QString & aSheetId);
    std::optional<QJsonArray> getSheetValues(const QString & aSheetId, const QString & aTabId);
    std::optional<QJsonObject> getDocument(const QString & anId);
    std::optional<QByteArray> getDocumentAsPdf(const QString & anId);

    void updateDocument(const QString & anId,
                        const QString & newName,
                        const QString &aData);

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
    std::optional<QJsonObject> getReplay(const QString & endPoint, const QString& aUrl);
    std::optional<QByteArray> getPdfReplay(const QString & endPoint, const QString& aUrl);
    std::optional<QJsonObject> postReplay(const QString & endPoint, const QString& aUrl,
                                          const QByteArray &aData = QByteArray());


    std::optional<QString> copyDocument(const QString & anId, const QString &newName);

    void init_internal();
    bool readCredentials();
    bool checkTokenFile();
    void writeTokenFile();

    QPointer<QOAuth2AuthorizationCodeFlow> m_google {nullptr};
    QPointer<QNetworkAccessManager> m_networkManager {nullptr};
    QPointer<QRestAccessManager> m_restManager {nullptr};
    QPointer<QOAuthHttpServerReplyHandler> m_replyHandler {nullptr};

    QNetworkRequestFactory m_requestFactory {};
    QNetworkReply::NetworkError m_neworkReplyError {QNetworkReply::NoError};
    Modele_Danych::CredentialsData m_credentials{};

    QTime m_expiredAt {};
    QDate m_tokenDate {};

    QString m_errorMessage {};
    QString m_activeToken {};

    const QString m_scope {"https://www.googleapis.com/auth/spreadsheets.readonly "
                          "https://www.googleapis.com/auth/documents "
                          "https://www.googleapis.com/auth/drive "
                          "https://www.googleapis.com/auth/drive.metadata.readonly"};

    const QString m_sheetsEndPoint {"https://sheets.googleapis.com/v4"};
    const QString m_docEndPoint {"https://docs.googleapis.com/v1"};
    const QString m_fileEndPoint {"https://www.googleapis.com/drive/v3"};
    const QString m_tokenFile{"token.json"};




    bool m_isAuthenticated {false};
    bool m_credFileRead{false};
    int m_timeout{50000};  
};

#endif // GOOGLESSO_H
