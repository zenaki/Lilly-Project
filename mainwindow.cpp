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
    if (!set || this->s.name == "N/A" || this->s.name == "") {
        settingsDialog = new SettingsDialog;
        settingsDialog->exec();
        getSetting();
    }
    if (this->s.name != "N/A") {
        serial_port = new QSerialPort;
        connect(serial_port, SIGNAL(readyRead()), this, SLOT(readData_Serial()));
        serial_port->setPortName(this->s.name);
        serial_port->setBaudRate(this->s.baudRate);
        serial_port->setDataBits(this->s.dataBits);
        serial_port->setParity(this->s.parity);
        serial_port->setStopBits(this->s.stopBits);
        serial_port->setFlowControl(this->s.flowControl);
        if (serial_port->open(QIODevice::ReadWrite)) {
            serial_connect = true;
        } else {
            serial_connect = false;
            serial_port = NULL;
        }
    } else {
        set = false;
        serial_connect = false;
    }

    if (serial_connect) {
        QMessageBox::information(this, "Serial Communication", " Berhasil Terkoneksi");
        ui->actionConnect->setEnabled(false); ui->actionDisconnect->setEnabled(true);
    } else {
        QMessageBox::critical(this, "Serial Communication", "Koneksi Gagal !!");
        ui->actionConnect->setEnabled(true); ui->actionDisconnect->setEnabled(false);
    }
}

void MainWindow::on_actionDisconnect_triggered()
{
    if (serial_connect) {
        if (serial_port->isOpen()) {
            serial_port->close();
            QMessageBox::information(this, "Serial Communication", "Berhasil Disconnect");
            ui->actionConnect->setEnabled(true); ui->actionDisconnect->setEnabled(false);
        }
    } else {
        QMessageBox::warning(this, "Serial Communication", "Tidak ada Koneksi");
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
        str_read.remove("FP::").remove("GET").remove("*");
//        if (str_read == "Pendaftaran Jari\r\n") {
//            baca = false; daftar = true;
//        } else if (str_read == "Menunggu Jari Ditempelkan\r\n") {
//            baca = true; daftar = false;
//        }
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
        str_read.remove("GET").remove("*").remove(" ");
        if (dapatRFID) {
            ui->pte_log->appendPlainText("GET RFID << " + str_read);
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

        this->s.name = sett.value("SERIAL/NAME").toString();
        this->s.c_name_id = sett.value("SERIAL/C_NAME_ID").toInt();

        this->s.baudRate = static_cast<QSerialPort::BaudRate>(
                    sett.value("SERIAL/BAUDRATE").toInt());
        this->s.c_baudRate_id = sett.value("SERIAL/C_BAUDRATE_ID").toInt();
        this->s.stringBaudRate = sett.value("SERIAL/STRING_BAUDRATE").toString();

        this->s.dataBits = static_cast<QSerialPort::DataBits>(
                    sett.value("SERIAL/DATA_BITS").toInt());
        this->s.c_dataBits_id = sett.value("SERIAL/C_DATA_BITS_ID").toInt();
        this->s.stringDataBits = sett.value("SERIAL/STRING_DATA_BITS").toString();

        this->s.parity = static_cast<QSerialPort::Parity>(
                    sett.value("SERIAL/PARITY").toInt());
        this->s.c_parity_id = sett.value("SERIAL/C_PARITY_ID").toInt();
        this->s.stringParity = sett.value("SERIAL/STRING_PARITY").toString();

        this->s.stopBits = static_cast<QSerialPort::StopBits>(
                    sett.value("SERIAL/STOP_BITS").toInt());
        this->s.c_stopBits_id = sett.value("SERIAL/C_STOP_BITS_ID").toInt();
        this->s.stringStopBits = sett.value("SERIAL/STRING_STOP_BITS").toString();

        this->s.flowControl = static_cast<QSerialPort::FlowControl>(
                    sett.value("SERIAL/FLOW_CONTROL").toInt());
        this->s.c_flowControl_id = sett.value("SERIAL/C_FLOW_CONTROL_ID").toInt();
        this->s.stringFlowControl = sett.value("SERIAL/STRING_FLOW_CONTROL").toString();

        if (this->s.name != "N/A") {
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
