#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

#include "mainwindow.h"
#include "init_mysql.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

private slots:
    void on_pb_login_clicked();

    void on_pushButton_clicked();

private:
    Ui::LoginDialog *ui;

    init_mysql initMysql;
    QSqlDatabase db; QStringList user;
    bool get_database();

    MainWindow *mainWindow;
};

#endif // LOGINDIALOG_H
