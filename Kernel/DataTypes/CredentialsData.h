#ifndef CREDENTIALSDATA_H
#define CREDENTIALSDATA_H
#include <QString>

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
};
}
#endif // CREDENTIALSDATA_H
