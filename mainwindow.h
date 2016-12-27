#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "global.h"
#include "settingsdialog.h"
#include "init_mysql.h"
#include "formkeluarga.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, int user_id = 0);
    ~MainWindow();
    
    QSerialPort *serial_port; bool serial_connect; QString str_read;
    QSqlDatabase db;

    bool getFingerID; bool getRFID;
    QLineEdit *tempFingerID; QLineEdit *tempRFID;
    QPushButton *tempPB_FingerID; QPushButton *tempPB_RFID;
    QLabel *tempLbl_FingerID; QLabel *tempLbl_RFID;

    int jml_keluarga;
    struct keluarga Kel[MAX_KELUARGA];
    struct keluarga Kel_Baru;

    bool checkFingerID_available(QString id);
    bool checkRFID_available(QString id);
    void setFingerPrintID(int id);

    bool setFingerID; bool SetSuccess;

    QLabel *Label_Finger_Log;
    QLabel *Label_RFID_Log;

    QTimer *tmr;
private slots:
    void on_actionRefresh_triggered();
    void on_actionConnect_triggered();
    void on_actionDisconnect_triggered();
    void on_actionSetting_triggered();

    void readData_Serial();

//    void on_pb_Baca_FingerPrint_clicked();
//    void on_pb_Daftar_FingerPrint_clicked();

    void on_pb_getFingerID_clicked();
    void on_pb_getRFID_clicked();
    void on_pb_tambah_anggota_clicked();
    void on_pb_save_keluarga_baru_clicked();

    void on_table_kepala_keluarga_clicked(const QModelIndex &index);
    void on_table_kepala_keluarga_doubleClicked(const QModelIndex &index);
    void on_table_anggota_keluarga_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;

    void setDefault(bool b);
    struct setting s;
    SettingsDialog *settingsDialog;
    bool set;

    void getSetting();

    bool baca; bool daftar; bool dapatFP; bool dapatRFID;
    init_mysql initMysql; bool database_connect;
    int id_user;

    void getDataRusun();

    FormKeluarga *FK;

    void setPendaftaran();
    QStringList getKamarTersedia();
    void setDataRusun();

    int rowSelected_KplKel;
};

#endif // MAINWINDOW_H
