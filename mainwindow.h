#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "global.h"
#include "settings.h"
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

private:
    Ui::MainWindow *ui;

    struct serial s[2];
    QSerialPort *serial1; bool s1_connect;
    QSerialPort *serial2; bool s2_connect;
    Settings *settings;
    SettingsDialog *settingsDialog;
    bool set;
    void getSetting();
};

#endif // MAINWINDOW_H
