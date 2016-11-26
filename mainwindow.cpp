#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::setWindowTitle("Software Lilly");
//    MainWindow::showMaximized();

    set = false; baca = false; daftar = false;
    dapatFP = false; dapatRFID = false;
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
    if (!set || this->s.serialCommunication.name == "N/A" || this->s.serialCommunication.name == "" || this->s.Database.host == "") {
        settingsDialog = new SettingsDialog;
        settingsDialog->exec();
        getSetting();
    }
    if (this->s.serialCommunication.name != "N/A" || this->s.Database.host != "") {
        serial_port = new QSerialPort;
        connect(serial_port, SIGNAL(readyRead()), this, SLOT(readData_Serial()));
        serial_port->setPortName(this->s.serialCommunication.name);
        serial_port->setBaudRate(this->s.serialCommunication.baudRate);
        serial_port->setDataBits(this->s.serialCommunication.dataBits);
        serial_port->setParity(this->s.serialCommunication.parity);
        serial_port->setStopBits(this->s.serialCommunication.stopBits);
        serial_port->setFlowControl(this->s.serialCommunication.flowControl);
        if (serial_port->open(QIODevice::ReadWrite)) {
            serial_connect = true;
        } else {
            serial_connect = false;
            serial_port = NULL;
        }
        db = initMysql.connect_db();
        if (db.isOpen()) {
            database_connect = true;
        } else {
            database_connect = false;
        }
    } else {
        set = false;
        serial_connect = false;
    }

    if (serial_connect && database_connect) {
        QMessageBox::information(this, "Serial Communication & Database", " Berhasil Terkoneksi");
        ui->actionConnect->setEnabled(false); ui->actionDisconnect->setEnabled(true);
    } else {
        if (!serial_connect && database_connect) {
            QMessageBox::critical(this, "Serial Communication & Database", "Koneksi Serial Gagal !!");
        } else if (serial_connect && !database_connect) {
            QMessageBox::critical(this, "Serial Communication & Database", "Koneksi Database Gagal !!");
        } else {
            QMessageBox::critical(this, "Serial Communication & Database", "Koneksi Serial dan Database Gagal !!");
        }
        ui->actionConnect->setEnabled(true); ui->actionDisconnect->setEnabled(false);
    }
}

void MainWindow::on_actionDisconnect_triggered()
{
    if (serial_connect && database_connect) {
        if (serial_port->isOpen()) {
            serial_port->close();
            serial_connect = false;
        }
        if (db.isOpen()) {
            db.close();
            database_connect = false;
        }
        if (serial_connect && database_connect) {
            QMessageBox::critical(this, "Serial Communication & Database", "Serial dan Database gagal Disconnect");
        } else {
            if (!serial_connect && database_connect) {
                QMessageBox::information(this, "Serial Communication & Database", "Serial Berhasil Disconnect/nDatabase gagal Disconnect");
            } else if (serial_connect && !database_connect) {
                QMessageBox::information(this, "Serial Communication & Database", "Serial gagal Disconnect/nDatabase Berhasil Disconnect");
            } else {
                QMessageBox::critical(this, "Serial Communication & Database", "Serial dan Database Berhasil Disconnect");
                ui->actionConnect->setEnabled(true); ui->actionDisconnect->setEnabled(false);
            }
        }
    } else {
        QMessageBox::warning(this, "Serial Communication", "Tidak ada Koneksi");
        ui->actionConnect->setEnabled(true); ui->actionDisconnect->setEnabled(false);
    }
}

void MainWindow::on_actionSetting_triggered()
{
    settingsDialog = new SettingsDialog;
    settingsDialog->exec();
}

void MainWindow::readData_Serial()
{
    str_read.append(serial_port->readAll());
    if (str_read.indexOf("FP::") > 0 && str_read.indexOf("*") > 0 && (str_read.indexOf("FP::") < str_read.indexOf("*"))) {
        int a = str_read.indexOf("FP::");
        int b = str_read.indexOf("*");
        if (str_read.indexOf("GET") > 0) dapatFP = true;
        str_read = str_read.mid(a+4, b-a);
        str_read.remove("FP::").remove("GET").remove("*").remove("\r\n");
        if (dapatFP) {
            ui->pte_log->appendPlainText("GET Finger Print << " + str_read);
            dapatFP = false;
        } else {
            ui->pte_log->appendPlainText(str_read);
        }
        str_read.clear();
    } else if (str_read.indexOf("RFID::") > 0 && str_read.indexOf("*") > 0 && (str_read.indexOf("RFID::") < str_read.indexOf("*"))) {
        int a = str_read.indexOf("RFID::");
        int b = str_read.indexOf("*");
        if (str_read.indexOf("GET") > 0) dapatRFID = true;
        str_read = str_read.mid(a+6, b-a);
        str_read.remove("GET").remove("*").remove(" ").remove("\r\n");
        if (dapatRFID) {
            ui->pte_log->appendPlainText("GET RFID << " + str_read);
            dapatRFID = false;
        } else {
            ui->pte_log->appendPlainText(str_read);
        }
        str_read.clear();
    }
}

void MainWindow::getSetting()
{
    if (QFile("settings.ini").exists()) {
        QSettings sett("settings.ini", QSettings::IniFormat);

        this->s.serialCommunication.name = sett.value("SERIAL/NAME").toString();
        this->s.serialCommunication.c_name_id = sett.value("SERIAL/C_NAME_ID").toInt();

        this->s.serialCommunication.baudRate = static_cast<QSerialPort::BaudRate>(
                    sett.value("SERIAL/BAUDRATE").toInt());
        this->s.serialCommunication.c_baudRate_id = sett.value("SERIAL/C_BAUDRATE_ID").toInt();
        this->s.serialCommunication.stringBaudRate = sett.value("SERIAL/STRING_BAUDRATE").toString();

        this->s.serialCommunication.dataBits = static_cast<QSerialPort::DataBits>(
                    sett.value("SERIAL/DATA_BITS").toInt());
        this->s.serialCommunication.c_dataBits_id = sett.value("SERIAL/C_DATA_BITS_ID").toInt();
        this->s.serialCommunication.stringDataBits = sett.value("SERIAL/STRING_DATA_BITS").toString();

        this->s.serialCommunication.parity = static_cast<QSerialPort::Parity>(
                    sett.value("SERIAL/PARITY").toInt());
        this->s.serialCommunication.c_parity_id = sett.value("SERIAL/C_PARITY_ID").toInt();
        this->s.serialCommunication.stringParity = sett.value("SERIAL/STRING_PARITY").toString();

        this->s.serialCommunication.stopBits = static_cast<QSerialPort::StopBits>(
                    sett.value("SERIAL/STOP_BITS").toInt());
        this->s.serialCommunication.c_stopBits_id = sett.value("SERIAL/C_STOP_BITS_ID").toInt();
        this->s.serialCommunication.stringStopBits = sett.value("SERIAL/STRING_STOP_BITS").toString();

        this->s.serialCommunication.flowControl = static_cast<QSerialPort::FlowControl>(
                    sett.value("SERIAL/FLOW_CONTROL").toInt());
        this->s.serialCommunication.c_flowControl_id = sett.value("SERIAL/C_FLOW_CONTROL_ID").toInt();
        this->s.serialCommunication.stringFlowControl = sett.value("SERIAL/STRING_FLOW_CONTROL").toString();

        if (this->s.serialCommunication.name != "N/A") {
            set = true;
        } else {
            set = false;
        }
    } else {
        set = false;
    }
}

void MainWindow::on_pb_Baca_FingerPrint_clicked()
{
//    if (serial_port->isOpen() && serial_connect) {
//        if (!baca && !daftar) {
//            serial_port->write("finger;0#\r\n");
//        }
//    }
}

void MainWindow::on_pb_Daftar_FingerPrint_clicked()
{
    if (serial_port->isOpen() && serial_connect) {
        serial_port->write("enroll;" + QString::number(ui->sb_id->value()).toUtf8() + "#\r\n");
//        if (!baca && !daftar) {
//            serial_port->write("enroll;" + QString::number(ui->sb_id->value()).toUtf8() + "#\r\n");
//        } else if (baca && !daftar) {
//            serial_port->write("finger;0#\r\n");
//            serial_port->write("enroll;" + QString::number(ui->sb_id->value()).toUtf8() + "#\r\n");
//        }
    }
}
