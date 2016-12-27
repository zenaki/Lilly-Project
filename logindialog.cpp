#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    connect(ui->le_username, SIGNAL(returnPressed()), ui->pb_login, SIGNAL(clicked()));
    connect(ui->le_password, SIGNAL(returnPressed()), ui->pb_login, SIGNAL(clicked()));

    db = initMysql.connect_db();
    db.open();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Database", "Database is not open ..!!");
        this->close();
    }
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_pb_login_clicked()
{
    if (this->get_database()) {
        for (int i = 0; i < user.length(); i++) {
            if ((user.at(i+1) == ui->le_username->text()) && (user.at(i+2) == ui->le_password->text())) {
                mainWindow = new MainWindow(this, user.at(i).toInt());
                mainWindow->show();
                this->hide();
                break;
            }
        }
    } else {
        QMessageBox::critical(this, "Login Failure", "Cannot Get Database ..");
    }
}

bool LoginDialog::get_database() {
    QSqlQuery q(db);

    q.prepare("SELECT id_user, user_name, password FROM admin WHERE status = 1");
    if (!q.exec()) {
        return false;
    }
    else {
        while (q.next()) {
            user.append(q.value(0).toString().toLatin1());
            user.append(q.value(1).toString().toLatin1());
            user.append(q.value(2).toString().toLatin1());
        }
    }

    db.close();
    return true;
}

void LoginDialog::on_pushButton_clicked()
{
    this->close();
}
