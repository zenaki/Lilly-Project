#include "init_mysql.h"

init_mysql::init_mysql()
{
}

QSqlDatabase init_mysql::connect_db(){

    QFile db_path("settings.ini");
    if (db_path.exists()) {
        QSettings db_sett("settings.ini", QSettings::IniFormat);
        host = db_sett.value("DATABASE/HOST").toString();
        db_name = db_sett.value("DATABASE/DB_NAME").toString();
        user_name = db_sett.value("DATABASE/USERNAME").toString();
        password = db_sett.value("DATABASE/PASSWORD").toString();

        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName(host);
        db.setDatabaseName(db_name);
        db.setUserName(user_name);
        db.setPassword(password);

        return (QSqlDatabase) db;
    } else {
        host = "localhost";
        db_name = "rumah_susun";
        user_name = "root";
        password = "root";

        QSettings db_sett("settings.ini", QSettings::IniFormat);
        db_sett.beginGroup("DATABASE");
        db_sett.setValue("HOST", host.toUtf8());
        db_sett.setValue("DB_NAME", db_name.toUtf8());
        db_sett.setValue("USERNAME", user_name.toUtf8());
        db_sett.setValue("PASSWORD", password.toUtf8());
        db_sett.endGroup();

        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName(host);
        db.setDatabaseName(db_name);
        db.setUserName(user_name);
        db.setPassword(password);

        return (QSqlDatabase) db;
    }
}

void init_mysql::close(QSqlDatabase db)
{
    QString connection;
    connection = db.connectionName();
    db.close();
    db = QSqlDatabase();
    db.removeDatabase(connection);
}
