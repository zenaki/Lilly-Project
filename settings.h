#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include "global.h"

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

    struct serial s[2];

private:
    Ui::Settings *ui;

    void fillPortsInfo();
    void fillPortsParameters();
    void setSetting();
    void getSetting();
    void readSetting();
    void writeSetting();

private slots:
    void showPortInfo(int idx);
    void showPortInfo_2(int idx);
    void on_pbApply_clicked();
    void on_pbCancel_clicked();
};

#endif // SETTINGS_H
