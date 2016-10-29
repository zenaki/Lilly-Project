#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::setWindowTitle("Software Lilly");
    MainWindow::showMaximized();

    set = false;
    getSetting();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionRefresh_triggered()
{

}

void MainWindow::on_actionConnect_triggered()
{
    if (!set) {
        settingsDialog = new SettingsDialog;
        settingsDialog->exec();
    }

    if (this->s[0].name != "N/A") {
        serial1 = new QSerialPort;
        serial1->setPortName(this->s[0].name);
        serial1->setBaudRate(this->s[0].baudRate);
        serial1->setDataBits(this->s[0].dataBits);
        serial1->setParity(this->s[0].parity);
        serial1->setStopBits(this->s[0].stopBits);
        serial1->setFlowControl(this->s[0].flowControl);
        if (serial1->open(QIODevice::ReadWrite)) {
            s1_connect = true;
        } else {
            s1_connect = false;
            serial1 = NULL;
        }
    }

    if (this->s[1].name != "N/A") {
        serial2 = new QSerialPort;
        serial2->setPortName(this->s[1].name);
        serial2->setBaudRate(this->s[1].baudRate);
        serial2->setDataBits(this->s[1].dataBits);
        serial2->setParity(this->s[1].parity);
        serial2->setStopBits(this->s[1].stopBits);
        serial2->setFlowControl(this->s[1].flowControl);
        if (serial2->open(QIODevice::ReadWrite)) {
            s2_connect = true;
        } else {
            s2_connect = false;
            serial2 = NULL;
        }
    }

    if (s1_connect && s2_connect) {
        QMessageBox::information(this, "Serial Communication", "Finger Print dan RFID Berhasil Terkoneksi");
        ui->actionConnect->setEnabled(false); ui->actionDisconnect->setEnabled(true);
    } else if (s1_connect && !s2_connect) {
        QMessageBox::information(this, "Serial Communication", "Finger Print Berhasil Terkoneksi");
        ui->actionConnect->setEnabled(false); ui->actionDisconnect->setEnabled(true);
    } else if (!s1_connect && s2_connect) {
        QMessageBox::information(this, "Serial Communication", "RFID Berhasil Terkoneksi");
        ui->actionConnect->setEnabled(false); ui->actionDisconnect->setEnabled(true);
    } else {
        QMessageBox::critical(this, "Serial Communication", "Connection Fail !!");
        ui->actionConnect->setEnabled(true); ui->actionDisconnect->setEnabled(false);
    }
}

void MainWindow::on_actionDisconnect_triggered()
{
    if (s1_connect || s2_connect) {
        if (serial1->isOpen()) {
            serial1->close();
        }
        if (serial2->isOpen()) {
            serial2->close();
        }

        QMessageBox::information(this, "Serial Communication", "Berhasil Disconnect");
        ui->actionConnect->setEnabled(true); ui->actionDisconnect->setEnabled(false);
    } else {
        QMessageBox::warning(this, "Serial Communication", "Tidak ada Koneksi");
    }
}

void MainWindow::on_actionSetting_triggered()
{
    settingsDialog = new SettingsDialog;
    settingsDialog->exec();
}

void MainWindow::getSetting()
{
    if (QFile("settings.ini").exists()) {
        QSettings sett("settings.ini", QSettings::IniFormat);

        //** FINGER_PRINT **//

        this->s[0].name = sett.value("FINGER_PRINT/NAME").toString();
        this->s[0].c_name_id = sett.value("FINGER_PRINT/C_NAME_ID").toInt();

        this->s[0].baudRate = static_cast<QSerialPort::BaudRate>(
                    sett.value("FINGER_PRINT/BAUDRATE").toInt());
        this->s[0].c_baudRate_id = sett.value("FINGER_PRINT/C_BAUDRATE_ID").toInt();
        this->s[0].stringBaudRate = sett.value("FINGER_PRINT/STRING_BAUDRATE").toString();

        this->s[0].dataBits = static_cast<QSerialPort::DataBits>(
                    sett.value("FINGER_PRINT/DATA_BITS").toInt());
        this->s[0].c_dataBits_id = sett.value("FINGER_PRINT/C_DATA_BITS_ID").toInt();
        this->s[0].stringDataBits = sett.value("FINGER_PRINT/STRING_DATA_BITS").toString();

        this->s[0].parity = static_cast<QSerialPort::Parity>(
                    sett.value("FINGER_PRINT/PARITY").toInt());
        this->s[0].c_parity_id = sett.value("FINGER_PRINT/C_PARITY_ID").toInt();
        this->s[0].stringParity = sett.value("FINGER_PRINT/STRING_PARITY").toString();

        this->s[0].stopBits = static_cast<QSerialPort::StopBits>(
                    sett.value("FINGER_PRINT/STOP_BITS").toInt());
        this->s[0].c_stopBits_id = sett.value("FINGER_PRINT/C_STOP_BITS_ID").toInt();
        this->s[0].stringStopBits = sett.value("FINGER_PRINT/STRING_STOP_BITS").toString();

        this->s[0].flowControl = static_cast<QSerialPort::FlowControl>(
                    sett.value("FINGER_PRINT/FLOW_CONTROL").toInt());
        this->s[0].c_flowControl_id = sett.value("FINGER_PRINT/C_FLOW_CONTROL_ID").toInt();
        this->s[0].stringFlowControl = sett.value("FINGER_PRINT/STRING_FLOW_CONTROL").toString();

        //** RFID **//

        this->s[1].name = sett.value("RFID/NAME").toString();
        this->s[1].c_name_id = sett.value("RFID/C_NAME_ID").toInt();

        this->s[1].baudRate = static_cast<QSerialPort::BaudRate>(
                    sett.value("RFID/BAUDRATE").toInt());
        this->s[1].c_baudRate_id = sett.value("RFID/C_BAUDRATE_ID").toInt();
        this->s[1].stringBaudRate = sett.value("RFID/STRING_BAUDRATE").toString();

        this->s[1].dataBits = static_cast<QSerialPort::DataBits>(
                    sett.value("RFID/DATA_BITS").toInt());
        this->s[1].c_dataBits_id = sett.value("RFID/C_DATA_BITS_ID").toInt();
        this->s[1].stringDataBits = sett.value("RFID/STRING_DATA_BITS").toString();

        this->s[1].parity = static_cast<QSerialPort::Parity>(
                    sett.value("RFID/PARITY").toInt());
        this->s[1].c_parity_id = sett.value("RFID/C_PARITY_ID").toInt();
        this->s[1].stringParity = sett.value("RFID/STRING_PARITY").toString();

        this->s[1].stopBits = static_cast<QSerialPort::StopBits>(
                    sett.value("RFID/STOP_BITS").toInt());
        this->s[1].c_stopBits_id = sett.value("RFID/C_STOP_BITS_ID").toInt();
        this->s[1].stringStopBits = sett.value("RFID/STRING_STOP_BITS").toString();

        this->s[1].flowControl = static_cast<QSerialPort::FlowControl>(
                    sett.value("RFID/FLOW_CONTROL").toInt());
        this->s[1].c_flowControl_id = sett.value("RFID/C_FLOW_CONTROL_ID").toInt();
        this->s[1].stringFlowControl = sett.value("RFID/STRING_FLOW_CONTROL").toString();

        if (this->s[0].name != "N/A") {
            set = true;
        } else {
            set = false;
        }
    } else {
        set = false;
    }
}
