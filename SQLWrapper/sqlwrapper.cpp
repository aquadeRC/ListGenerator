#include "sqlwrapper.h"
#include <QDebug>
#include <QSqlDatabase>

SQLWrapper::SQLWrapper() {


}

void SQLWrapper::init()
{
    if (!QSqlDatabase::drivers().contains("QSQLITE"))
        qDebug() <<"Unable to load database" << "This demo needs the SQLITE driver";

    m_db = QSqlDatabase::addDatabase("QSQLITE", "Northwind");
    m_db.setDatabaseName("Employees");
    bool ok = m_db.open();
}
