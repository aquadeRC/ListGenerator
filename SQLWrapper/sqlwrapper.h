#ifndef SQLWRAPPER_H
#define SQLWRAPPER_H

#include "SQLWrapper_global.h"
#include <QSqlDatabase>

class SQLWRAPPER_EXPORT SQLWrapper
{
public:
    SQLWrapper();
    void init();

 private:
    QSqlDatabase m_db;
};

#endif // SQLWRAPPER_H
