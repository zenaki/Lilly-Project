#ifndef INIT_MYSQL_H
#define INIT_MYSQL_H

#include "global.h"

class init_mysql
{
public:
    init_mysql();

    QString host;
    QString db_name;
    QString user_name;
    QString password;

    QSqlDatabase connect_db();
    void close(QSqlDatabase db);
};

#endif // INIT_MYSQL_H
