#ifndef FORMKELUARGA_H
#define FORMKELUARGA_H

#include <QDialog>
#include "global.h"

namespace Ui {
class FormKeluarga;
}

class FormKeluarga : public QDialog
{
    Q_OBJECT

public:
    explicit FormKeluarga(QWidget *parent = 0, int mode = 0,
                          QSqlDatabase dbase = QSqlDatabase::database("local", false), QSerialPort *serial = NULL,
                          QLineEdit *tmpFingerID = NULL, QLineEdit *tmpRFID = NULL,
                          QPushButton *tmpPB_FingerID = NULL, QPushButton *tmpPB_RFID = NULL,
                          QLabel *tmpLbl_FingerID = NULL, QLabel *tmpLbl_RFID = NULL);
    ~FormKeluarga();

    QLineEdit *tempFingerID; QLineEdit *tempRFID;
    QPushButton *tempPB_FingerID; QPushButton *tempPB_RFID;
    QLabel *tempLbl_FingerID; QLabel *tempLbl_RFID;

//    int jml_keluarga;
    struct keluarga Kel;

    QSerialPort *serial_port;
    QSqlDatabase db;

    void setForm(int row, struct keluarga kel);
private slots:
    void on_pb_save_clicked();
    void on_pb_cancel_clicked();

    void on_pb_getFingerID_clicked();

    void on_pb_getRFID_clicked();

private:
    Ui::FormKeluarga *ui;

    int form_mode;
    QStringList getKamarTersedia(int id);
    bool checkFingerID_available(QString id);
};

#endif // FORMKELUARGA_H
