#include "formkeluarga.h"
#include "ui_formkeluarga.h"

FormKeluarga::FormKeluarga(QWidget *parent, int mode,
                           QSqlDatabase dbase, QSerialPort *serial,
                           QLineEdit *tmpFingerID, QLineEdit *tmpRFID,
                           QPushButton *tmpPB_FingerID, QPushButton *tmpPB_RFID,
                           QLabel *tmpLbl_FingerID, QLabel *tmpLbl_RFID) :
    QDialog(parent),
    ui(new Ui::FormKeluarga)
{
    ui->setupUi(this);

    tempFingerID = tmpFingerID;
    tempRFID = tmpRFID;
    tempPB_FingerID = tmpPB_FingerID;
    tempPB_RFID = tmpPB_RFID;
    tempLbl_FingerID = tmpLbl_FingerID;
    tempLbl_RFID = tmpLbl_RFID;

    db = dbase;
    serial_port = serial;

    if (mode == 0) { //Edit Kepala Keluarga Keluar
        ui->label_13->setHidden(false);
        ui->label_14->setHidden(false);
        ui->cb_kamar->setHidden(false);
    } else if (mode == 1 || mode == 2) { //Edit Anggota Keluarga || Tambah Anggota Keluarga
        ui->label_13->setHidden(true);
        ui->label_14->setHidden(true);
        ui->cb_kamar->setHidden(true);
    }

    form_mode = mode;
}

FormKeluarga::~FormKeluarga()
{
    delete ui;
}

void FormKeluarga::setForm(int row, struct keluarga kel)
{
    Kel = kel;
    if (form_mode == 0) {
        ui->le_no_ktp->setText(Kel.no_ktp);
        ui->le_nama_lengkap->setText(Kel.nama_lengkap);
        ui->le_temp_lahir->setText(Kel.tempat_lahir);
        ui->de_tgl_lahir->setDate(QDate::fromString(Kel.tanggal_lahir, "yyyy-MM-dd"));
        ui->le_agama->setText(Kel.agama);
        ui->le_no_telp->setText(Kel.no_telp);
        ui->cb_kamar->addItems(this->getKamarTersedia(Kel.id_kpl_keluarga));
        ui->cb_kamar->setCurrentText(QString::number(Kel.no_kamar));
        ui->le_fingerID->setText(Kel.FingerID);
        ui->le_rfID->setText(Kel.RFID);
    } else if (form_mode == 1) {
        ui->le_no_ktp->setText(Kel.anggotaKeluarga[row].no_ktp);
        ui->le_nama_lengkap->setText(Kel.anggotaKeluarga[row].nama_lengkap);
        ui->le_temp_lahir->setText(Kel.anggotaKeluarga[row].tempat_lahir);
        ui->de_tgl_lahir->setDate(QDate::fromString(Kel.anggotaKeluarga[row].tanggal_lahir, "yyyy-MM-dd"));
        ui->le_agama->setText(Kel.anggotaKeluarga[row].agama);
        ui->le_no_telp->setText(Kel.anggotaKeluarga[row].no_telp);
        ui->le_fingerID->setText(Kel.anggotaKeluarga[row].FingerID);
        ui->le_rfID->setText(Kel.anggotaKeluarga[row].RFID);
    }
}

void FormKeluarga::on_pb_save_clicked()
{
    this->close();
}

void FormKeluarga::on_pb_cancel_clicked()
{
    this->close();
}

QStringList FormKeluarga::getKamarTersedia(int id)
{
    QSqlQuery q(db);
    QStringList result;
    q.prepare("SELECT nomor_kamar from rusun where status = 0 or id_kepala_keluarga = " + QString::number(id));
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

void FormKeluarga::on_pb_getFingerID_clicked()
{
    tempFingerID = ui->le_fingerID;
    tempPB_FingerID = ui->pb_getFingerID;
    bool setFinger = false; int index = 0;
    while (setFinger) {
        index++;
        setFinger = this->checkFingerID_available(QString::number(index));
    }
    tempPB_FingerID->setText("Wait");
    tempPB_FingerID->setEnabled(false);
    serial_port->write(QByteArray("enroll;"+QByteArray::number(index)+"#\r\n"));
}

void FormKeluarga::on_pb_getRFID_clicked()
{
    tempRFID = ui->le_rfID;
    tempPB_RFID = ui->pb_getRFID;
    tempPB_RFID->setText("Wait");
    tempPB_RFID->setEnabled(false);
}

bool FormKeluarga::checkFingerID_available(QString id)
{
    QSqlQuery q(db);
    q.prepare("SELECT true from keluarga where finger_id = '" + id + "' and status = 1");
    if (!q.exec()) {
        return false;
    } else {
        while (q.next()) {
            return true;
        }
        return false;
    }
}
