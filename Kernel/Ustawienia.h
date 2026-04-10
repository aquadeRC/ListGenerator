#ifndef USTAWIENIA_H
#define USTAWIENIA_H

#include <QObject>

class Ustawienia : public QObject
{
    Q_OBJECT
public:
    explicit Ustawienia(QObject *parent = nullptr);

    void load();
    void save();

    static QString getProjektyId();
    static QString getDecyjzeId();
    static QString getTemplateId();
    static QString getCredentialFile();
    static QString getDataDir();

    static void setProjektyId(const QString& aValue);
    static void setDecyjzeId(const QString& aValue);
    static void setTemplateId(const QString& aValue);
    static void setCredentialFile(const QString& aValue);

signals:
    void signalError(const QString & anError);

private:
    static inline QString m_projekty_file_id {};
    static inline QString m_decyje_file_id {};
    static inline QString m_template_file_id {};
    static inline QString m_credentials_file {};
    static inline QString m_data_dir {"/Kernel/Data/"};

    const QString m_settings_file{"Settings.json"};
    const QString m_projekty_file_id_field {"Projekty_Spreadshhet_ID"};
    const QString m_decyje_file_id_field {"Decyzje_Spreadshhet_ID"};
    const QString m_template_file_id_field {"Template_ID"};
    const QString m_credentials_file_field {"Credentials_File"};
};

#endif // USTAWIENIA_H
