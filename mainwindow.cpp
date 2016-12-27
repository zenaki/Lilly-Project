#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, int user_id) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::setWindowTitle("Aplikasi Rumah Susun");
//    MainWindow::showMaximized();

    Label_Finger_Log = new QLabel(this);
    Label_Finger_Log->setMinimumSize(QSize(300, 15));
    Label_Finger_Log->setMaximumSize(QSize(300, 15));
    Label_RFID_Log = new QLabel(this);
    Label_RFID_Log->setMinimumSize(QSize(300, 15));
    Label_RFID_Log->setMaximumSize(QSize(300, 15));

    this->statusBar()->addWidget(Label_Finger_Log);
    this->statusBar()->addWidget(Label_RFID_Log);

    tempLbl_FingerID = Label_Finger_Log;
    tempLbl_RFID = Label_RFID_Log;

//    set = false; baca = false; daftar = false;
//    dapatFP = false; dapatRFID = false;
    setFingerID = false;
    getSetting();
    this->setDefault(true);
    id_user = user_id;

//    getFingerID = false; getRFID = false;
    jml_keluarga = 0;

    tmr = new QTimer(this);
//    connect(tmr, SIGNAL(timeout()), parent, SLOT(serial_timeOut()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setDefault(bool b) {
    ui->actionConnect->setEnabled(b);
    ui->actionDisconnect->setEnabled(!b);
    ui->actionRefresh->setEnabled(!b);

    ui->gb_daftar_kelapa_keluarga->setEnabled(!b);
    ui->gb_daftar_anggota_keluarga->setEnabled(!b);
    ui->pb_save_keluarga_baru->setEnabled(!b);

    ui->gb_list_kepala_keluarga->setEnabled(!b);
    ui->gb_list_anggota_keluarga->setEnabled(!b);
}

void MainWindow::on_actionRefresh_triggered()
{
    this->getDataRusun();
    this->setDataRusun();
    this->setPendaftaran();
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
        db.open();
        if (db.isOpen()) {
            database_connect = true;
            this->getDataRusun();
            this->setDataRusun();
            this->setPendaftaran();
        } else {
            database_connect = false;
        }
    } else {
        set = false;
        serial_connect = false;
    }

    if (serial_connect && database_connect) {
        QMessageBox::information(this, "Serial Communication & Database", " Berhasil Terkoneksi");
        this->setDefault(false);
    } else {
        if (!serial_connect && database_connect) {
            QMessageBox::critical(this, "Serial Communication & Database", "Koneksi Serial Gagal !!");
        } else if (serial_connect && !database_connect) {
            QMessageBox::critical(this, "Serial Communication & Database", "Koneksi Database Gagal !!");
        } else {
            QMessageBox::critical(this, "Serial Communication & Database", "Koneksi Serial dan Database Gagal !!");
        }
        this->setDefault(true);
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
                QMessageBox::critical(this, "Serial Communication & Database", "Serial Berhasil Disconnect/nDatabase gagal Disconnect");
            } else if (serial_connect && !database_connect) {
                QMessageBox::critical(this, "Serial Communication & Database", "Serial gagal Disconnect/nDatabase Berhasil Disconnect");
            } else {
                QMessageBox::information(this, "Serial Communication & Database", "Serial dan Database Berhasil Disconnect");
                this->setDefault(true);
            }
        }
    } else {
        QMessageBox::warning(this, "Serial Communication", "Tidak ada Koneksi");
        this->setDefault(true);
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
        if (str_read.indexOf("Daftar") > 0) {
            setFingerID = true;
        }
        QStringList check = str_read.split(" ");
        for (int i = 0; i < check.length(); i++) {
            QString temp = check.at(i);
            temp.remove("#FP::").remove("Daftar::").remove("*").remove("\r").remove("\n").remove("\r\n");
            check.replace(i,temp);
        }
        int a = str_read.indexOf("FP::");
        int b = str_read.indexOf("*");
        if (str_read.indexOf("GET") > 0) dapatFP = true;
        str_read = str_read.mid(a+4, b-a);
        str_read.remove("FP::").remove("*").remove("\r\n");
        tempLbl_FingerID->setText("Finger Print : " + str_read + " ");
        if (dapatFP) {
//            if (getFingerID) {
////                valFingerID = str_read;
//            }
            dapatFP = false;
        } else if (setFingerID) {
            if (check.length() > 3) {
                if (check.at(2) == "Berhasil") {
                    setFingerID = false;
                    tempFingerID->setText(check.at(1));
                    tempPB_FingerID->setText("GET");
                    tempPB_FingerID->setEnabled(true);
                    tempPB_FingerID = NULL;
//                    getFingerID = false;
                } else if (check.at(2) == "Gagal") {
                    setFingerID = false;
                    tempPB_FingerID->setText("GET");
                    tempPB_FingerID->setEnabled(true);
                    tempPB_FingerID = NULL;
                    QMessageBox::critical(this, "Finger Print Sensor", "Gagal Menyimpan Data Jari !!");
                } else {
                    qDebug() << str_read;
                }
            }
        }
        str_read.clear();
    } else if (str_read.indexOf("RFID::") > 0 && str_read.indexOf("*") > 0 && (str_read.indexOf("RFID::") < str_read.indexOf("*"))) {
        QStringList check = str_read.split(" ");
        for (int i = 0; i < check.length(); i++) {
            QString temp = check.at(i);
            temp.remove("#FP::").remove("*\r\n");
            check.replace(i,temp);
        }
        int a = str_read.indexOf("RFID::");
        int b = str_read.indexOf("*");
        if (str_read.indexOf("GET") > 0) dapatRFID = true;
        str_read = str_read.mid(a+6, b-a);
        str_read.remove("GET").remove("*").remove(" ").remove("\r\n");
        tempLbl_RFID->setText("RFID Reader : " + str_read);
        if (dapatRFID) {
            if (tempRFID && tempPB_RFID && tempPB_RFID->text() == "Wait") {
                if (this->checkRFID_available(str_read)) {
                    tempRFID = NULL;
                    tempPB_RFID->setText("GET");
                    tempPB_RFID->setEnabled(true);
                    tempPB_RFID = NULL;
                    QMessageBox::critical(this, "RFID Reader", "RFID sudah pernah terdaftar");
                } else {
                    tempRFID->setText(str_read);
                    tempRFID = NULL;
                    tempPB_RFID->setText("GET");
                    tempPB_RFID->setEnabled(true);
                    tempPB_RFID = NULL;
//                    getRFID = false;
                }
            }
            dapatRFID = false;
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

        this->s.Database.host = sett.value("DATABASE/HOST").toString();
        this->s.Database.db_name = sett.value("DATABASE/DB_NAME").toString();
        this->s.Database.username = sett.value("DATABASE/USERNAME").toString();
        this->s.Database.password = sett.value("DATABASE/PASSWORD").toString();

        if (!this->s.serialCommunication.name.isEmpty() && !this->s.Database.host.isEmpty()) {
            set = true;
        } else {
            set = false;
        }
    } else {
        set = false;
    }
}

//void MainWindow::on_pb_Baca_FingerPrint_clicked()
//{
////    if (serial_port->isOpen() && serial_connect) {
////        if (!baca && !daftar) {
////            serial_port->write("finger;0#\r\n");
////        }
////    }
//}

//void MainWindow::on_pb_Daftar_FingerPrint_clicked()
//{
////    if (serial_port->isOpen() && serial_connect) {
////        serial_port->write("enroll;" + QString::number(ui->sb_id->value()).toUtf8() + "#\r\n");
//////        if (!baca && !daftar) {
//////            serial_port->write("enroll;" + QString::number(ui->sb_id->value()).toUtf8() + "#\r\n");
//////        } else if (baca && !daftar) {
//////            serial_port->write("finger;0#\r\n");
//////            serial_port->write("enroll;" + QString::number(ui->sb_id->value()).toUtf8() + "#\r\n");
//////        }
////    }
//}

void MainWindow::getDataRusun()
{
    QSqlQuery q(db);
    QStringList result;
    q.prepare(
          " SELECT "
          "    rsn.nomor_kamar, "
          "    kel.id_keluarga, "
          "    ident.nomor_ktp, "
          "    ident.nama, "
          "    ident.tempat_lahir, "
          "    ident.tanggal_lahir, "
          "    ident.agama, "
          "    ident.nomor_hp, "
          "    ident.foto, "
          "    kel.finger_id, "
          "    kel.rfid "
          " FROM "
          "    rumah_susun.rusun rsn, "
          "    rumah_susun.keluarga kel, "
          "    rumah_susun.identitas ident "
          " WHERE "
          "    rsn.status = 1 and "
          "    kel.id_kepala_keluarga = 0 and "
          "    kel.id_identitas = ident.id_identitas;"
    );
    if (q.exec()) {
        jml_keluarga = 0;
        while (q.next()) {
            Kel[jml_keluarga].no_kamar = q.value(0).toInt();
            Kel[jml_keluarga].id_kpl_keluarga = q.value(1).toInt();
            Kel[jml_keluarga].no_ktp = q.value(2).toString().toLatin1();
            Kel[jml_keluarga].nama_lengkap = q.value(3).toString().toLatin1();
            Kel[jml_keluarga].tempat_lahir = q.value(4).toString().toLatin1();
            Kel[jml_keluarga].tanggal_lahir = q.value(5).toString().toLatin1();
            Kel[jml_keluarga].agama = q.value(6).toString().toLatin1();
            Kel[jml_keluarga].no_telp = q.value(7).toString().toLatin1();
            Kel[jml_keluarga].foto = q.value(8).toString().toLatin1();
            Kel[jml_keluarga].FingerID = q.value(9).toString().toLatin1();
            Kel[jml_keluarga].RFID = q.value(10).toString().toLatin1();
            Kel[jml_keluarga].jml_anggota_keluarga = 0;
            jml_keluarga++;
        }
    }

    if (jml_keluarga > 0) {
        for (int i = 0; i < jml_keluarga; i++) {
            q.prepare(
                  " SELECT "
                  "    kel.id_keluarga, "
                  "    ident.nomor_ktp, "
                  "    ident.nama, "
                  "    ident.tempat_lahir, "
                  "    ident.tanggal_lahir, "
                  "    ident.agama, "
                  "    ident.nomor_hp, "
                  "    ident.foto, "
                  "    kel.finger_id, "
                  "    kel.rfid "
                  " FROM "
                  "    rumah_susun.keluarga kel, "
                  "    rumah_susun.identitas ident "
                  " WHERE "
                  "    kel.id_kepala_keluarga = " +
                       QString::number(Kel[i].id_kpl_keluarga) + " and "
                  "    kel.id_identitas = ident.id_identitas;"
            );

            if (q.exec()) {
                while (q.next()) {
                    Kel[i].anggotaKeluarga[Kel[i].jml_anggota_keluarga].id_keluarga = q.value(0).toInt();
                    Kel[i].anggotaKeluarga[Kel[i].jml_anggota_keluarga].no_ktp = q.value(1).toString().toLatin1();
                    Kel[i].anggotaKeluarga[Kel[i].jml_anggota_keluarga].nama_lengkap = q.value(2).toString().toLatin1();
                    Kel[i].anggotaKeluarga[Kel[i].jml_anggota_keluarga].tempat_lahir = q.value(3).toString().toLatin1();
                    Kel[i].anggotaKeluarga[Kel[i].jml_anggota_keluarga].tanggal_lahir = q.value(4).toString().toLatin1();
                    Kel[i].anggotaKeluarga[Kel[i].jml_anggota_keluarga].agama = q.value(5).toString().toLatin1();
                    Kel[i].anggotaKeluarga[Kel[i].jml_anggota_keluarga].no_telp = q.value(6).toString().toLatin1();
                    Kel[i].anggotaKeluarga[Kel[i].jml_anggota_keluarga].foto = q.value(7).toString().toLatin1();
                    Kel[i].anggotaKeluarga[Kel[i].jml_anggota_keluarga].FingerID = q.value(8).toString().toLatin1();
                    Kel[i].anggotaKeluarga[Kel[i].jml_anggota_keluarga].RFID = q.value(9).toString().toLatin1();
                    Kel[i].jml_anggota_keluarga++;
                }
            }
        }
    }
}

void MainWindow::setPendaftaran()
{
    ui->cb_kamar->addItems(this->getKamarTersedia());
}

QStringList MainWindow::getKamarTersedia()
{
    QSqlQuery q(db);
    QStringList result;
    q.prepare("SELECT nomor_kamar from rusun where status = 0");
    if (!q.exec()) {
        return result;
    }
    else {
        while (q.next()) {
            result.append(q.value(0).toString().toLatin1());
        }
    }
    return result;
}

void MainWindow::on_pb_getFingerID_clicked()
{
    tempFingerID = ui->le_fingerID;
    tempPB_FingerID = ui->pb_getFingerID;
    bool setFinger = false; int index = 0;
    while (!setFinger) {
        index++;
        setFinger = this->checkFingerID_available(QString::number(index));
    }
    tempPB_FingerID->setText("Wait");
    tempPB_FingerID->setEnabled(false);
    serial_port->write(QByteArray("enroll;"+QByteArray::number(index)+"#\r\n"));
}

void MainWindow::on_pb_getRFID_clicked()
{
    tempRFID = ui->le_rfID;
    tempPB_RFID = ui->pb_getRFID;
    tempPB_RFID->setText("Wait");
    tempPB_RFID->setEnabled(false);

}

void MainWindow::on_pb_tambah_anggota_clicked()
{
    if (
            !ui->le_no_telp->text().isEmpty() &&
            !ui->le_nama_lengkap->text().isEmpty() &&
            (!ui->cb_kamar->currentText().isEmpty() || (!ui->cb_kamar->currentText().toInt() > 0)) &&
            !ui->le_fingerID->text().isEmpty() &&
            !ui->le_rfID->text().isEmpty()
            ) {
        FK = new FormKeluarga(this, 2, db, serial_port, tempFingerID, tempRFID, tempPB_FingerID, tempPB_RFID, tempLbl_FingerID, tempLbl_RFID);
        FK->exec();

        qDebug() << "Selesai Tambah Anggota";
    } else {
        QMessageBox::critical(this, "Form Anggota Keluarga", "Harap lengkapi data Kepala Keluarga terlebih dahulu !!");
    }
}

void MainWindow::on_pb_save_keluarga_baru_clicked()
{

}

void MainWindow::setDataRusun()
{
    QStandardItemModel *Model_Kepala_Keluarga = new QStandardItemModel(jml_keluarga, 8, this);
    Model_Kepala_Keluarga->setHorizontalHeaderItem(0, new QStandardItem(QString("Nama")));
    Model_Kepala_Keluarga->setHorizontalHeaderItem(1, new QStandardItem(QString("No. KTP")));
    Model_Kepala_Keluarga->setHorizontalHeaderItem(2, new QStandardItem(QString("Tempat Lahir")));
    Model_Kepala_Keluarga->setHorizontalHeaderItem(3, new QStandardItem(QString("Tanggal Lahir")));
    Model_Kepala_Keluarga->setHorizontalHeaderItem(4, new QStandardItem(QString("Agama")));
    Model_Kepala_Keluarga->setHorizontalHeaderItem(5, new QStandardItem(QString("No. Telepon")));
    Model_Kepala_Keluarga->setHorizontalHeaderItem(6, new QStandardItem(QString("Foto")));
    Model_Kepala_Keluarga->setHorizontalHeaderItem(7, new QStandardItem(QString("No. Kamar")));

    ui->table_kepala_keluarga->setModel(Model_Kepala_Keluarga);

    for (int i = 0; i < jml_keluarga; i++) {
        Model_Kepala_Keluarga->setItem(i, 0, new QStandardItem(Kel[i].nama_lengkap));
        Model_Kepala_Keluarga->setItem(i, 1, new QStandardItem(Kel[i].no_ktp));
        Model_Kepala_Keluarga->setItem(i, 2, new QStandardItem(Kel[i].tempat_lahir));
        Model_Kepala_Keluarga->setItem(i, 3, new QStandardItem(Kel[i].tanggal_lahir));
        Model_Kepala_Keluarga->setItem(i, 4, new QStandardItem(Kel[i].agama));
        Model_Kepala_Keluarga->setItem(i, 5, new QStandardItem(Kel[i].no_telp));
        Model_Kepala_Keluarga->setItem(i, 6, new QStandardItem(Kel[i].foto));
        Model_Kepala_Keluarga->setItem(i, 7, new QStandardItem(QString::number(Kel[i].no_kamar)));
    }
}

void MainWindow::on_table_kepala_keluarga_clicked(const QModelIndex &index)
{
    rowSelected_KplKel = index.row();

    QStandardItemModel *Model_Anggota_Keluarga = new QStandardItemModel(Kel[rowSelected_KplKel].jml_anggota_keluarga, 7, this);
    Model_Anggota_Keluarga->setHorizontalHeaderItem(0, new QStandardItem(QString("Nama")));
    Model_Anggota_Keluarga->setHorizontalHeaderItem(1, new QStandardItem(QString("No. KTP")));
    Model_Anggota_Keluarga->setHorizontalHeaderItem(2, new QStandardItem(QString("Tempat Lahir")));
    Model_Anggota_Keluarga->setHorizontalHeaderItem(3, new QStandardItem(QString("Tanggal Lahir")));
    Model_Anggota_Keluarga->setHorizontalHeaderItem(4, new QStandardItem(QString("Agama")));
    Model_Anggota_Keluarga->setHorizontalHeaderItem(5, new QStandardItem(QString("No. Telepon")));
    Model_Anggota_Keluarga->setHorizontalHeaderItem(6, new QStandardItem(QString("Foto")));

    ui->table_anggota_keluarga->setModel(Model_Anggota_Keluarga);

    for (int i = 0; i < Kel[rowSelected_KplKel].jml_anggota_keluarga; i++) {
        Model_Anggota_Keluarga->setItem(i, 0, new QStandardItem(Kel[rowSelected_KplKel].anggotaKeluarga[i].nama_lengkap));
        Model_Anggota_Keluarga->setItem(i, 1, new QStandardItem(Kel[rowSelected_KplKel].anggotaKeluarga[i].no_ktp));
        Model_Anggota_Keluarga->setItem(i, 2, new QStandardItem(Kel[rowSelected_KplKel].anggotaKeluarga[i].tempat_lahir));
        Model_Anggota_Keluarga->setItem(i, 3, new QStandardItem(Kel[rowSelected_KplKel].anggotaKeluarga[i].tanggal_lahir));
        Model_Anggota_Keluarga->setItem(i, 4, new QStandardItem(Kel[rowSelected_KplKel].anggotaKeluarga[i].agama));
        Model_Anggota_Keluarga->setItem(i, 5, new QStandardItem(Kel[rowSelected_KplKel].anggotaKeluarga[i].no_telp));
        Model_Anggota_Keluarga->setItem(i, 6, new QStandardItem(Kel[rowSelected_KplKel].anggotaKeluarga[i].foto));
    }
}

void MainWindow::on_table_kepala_keluarga_doubleClicked(const QModelIndex &index)
{
    FK = new FormKeluarga(this, 0, db, serial_port, tempFingerID, tempRFID, tempPB_FingerID, tempPB_RFID, tempLbl_FingerID, tempLbl_RFID);
    FK->setForm(index.row(), Kel[index.row()]);
    FK->exec();
}

void MainWindow::on_table_anggota_keluarga_doubleClicked(const QModelIndex &index)
{
    FK = new FormKeluarga(this, 1, db, serial_port, tempFingerID, tempRFID, tempPB_FingerID, tempPB_RFID, tempLbl_FingerID, tempLbl_RFID);
    FK->setForm(index.row(), Kel[index.row()]);
    FK->exec();
}

bool MainWindow::checkFingerID_available(QString id)
{
    QSqlQuery q(db);
    q.prepare("SELECT true from keluarga where finger_id = '" + id + "' and status = 1");
    if (!q.exec()) {
        return true;
    } else {
        while (q.next()) {
            return false;
        }
        return true;
    }
}

bool MainWindow::checkRFID_available(QString id)
{
    QSqlQuery q(db);
    q.prepare("SELECT true from keluarga where rfid = '" + id + "' and status = 1");
    if (!q.exec()) {
        return false;
    } else {
        while (q.next()) {
            return true;
        }
        return false;
    }
}
