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

#endif // GLOBAL_H
