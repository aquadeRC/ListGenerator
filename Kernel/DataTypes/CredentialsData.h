#ifndef CREDENTIALSDATA_H
#define CREDENTIALSDATA_H
#include <QString>
#include <QStringList>

namespace Modele_Danych {
struct CredentialsData
{
    QString m_client_id;
    QString m_project_id;
    QString m_auth_uri;
    QString m_token_uri;
    QString m_token_provider;
    QString m_client_secret;
    QString m_redirect_uri;

    QString m_token;
    QString m_expiry;
    QString m_account;
    QString m_universe_domain;
    QStringList m_scopes;
    QString m_refresh_token;



};
}
#endif // CREDENTIALSDATA_H
