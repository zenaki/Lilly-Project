#ifndef GLOBAL_H
#define GLOBAL_H

#include <QDebug>
#include <QMessageBox>

#include <QSerialPort>
#include <QSerialPortInfo>

#include <QFile>
#include <QSettings>
#include <QVariant>

#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>

#include <QLineEdit>
#include <QStandardItemModel>

#include <QTimer>
#include <QDateTime>

#define MAX_KELUARGA 2
#define MAX_ANGGOTA_KELUARGA 5

struct serial {
    QString name;
    int c_name_id;

    qint32 baudRate;
    int c_baudRate_id;
    QString stringBaudRate;

    QSerialPort::DataBits dataBits;
    int c_dataBits_id;
    QString stringDataBits;

    QSerialPort::Parity parity;
    int c_parity_id;
    QString stringParity;

    QSerialPort::StopBits stopBits;
    int c_stopBits_id;
    QString stringStopBits;

    QSerialPort::FlowControl flowControl;
    int c_flowControl_id;
    QString stringFlowControl;
};

struct database {
    QString host;
    QString db_name;
    QString username;
    QString password;
};

struct setting {
    struct serial serialCommunication;
    struct database Database;
};

struct anggota {
    int id_keluarga;
    QString no_ktp;
    QString nama_lengkap;
    QString tempat_lahir;
    QString tanggal_lahir;
    QString agama;
    QString no_telp;
    QString foto;

    QString FingerID;
    QString RFID;
};

struct keluarga {
    int id_kpl_keluarga;
    QString no_ktp;
    QString nama_lengkap;
    QString tempat_lahir;
    QString tanggal_lahir;
    QString agama;
    QString no_telp;
    QString foto;

    int no_kamar;

    QString FingerID;
    QString RFID;

    int jml_anggota_keluarga;

    struct anggota anggotaKeluarga[MAX_ANGGOTA_KELUARGA];
};

#endif // GLOBAL_H
