#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "global.h"
#include "settingsdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionRefresh_triggered();
    void on_actionConnect_triggered();
    void on_actionDisconnect_triggered();
    void on_actionSetting_triggered();
    void readData_Serial();

    void on_pb_Baca_FingerPrint_clicked();

    void on_pb_Daftar_FingerPrint_clicked();

private:
    Ui::MainWindow *ui;

    struct serial s;
    QSerialPort *serial_port; bool serial_connect; QString str_read;
    SettingsDialog *settingsDialog;
    bool set;
    void getSetting();
    bool baca; bool daftar; bool dapatFP; bool dapatRFID;
};

#endif // MAINWINDOW_H
