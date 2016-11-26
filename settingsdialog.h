#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "global.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

private:
    Ui::SettingsDialog *ui;

    struct setting s;

    void fillPortsInfo();
    void fillPortsParameters();
    void setSetting();
    void getSetting();
    void readSetting();
    void writeSetting();

private slots:
    void showPortInfo(int idx);
    void on_pbApply_clicked();
    void on_pbCancel_clicked();
};

#endif // SETTINGSDIALOG_H
