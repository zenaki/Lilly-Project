#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    connect(ui->serialPortInfoListBox, SIGNAL(currentIndexChanged(int)), this, SLOT(showPortInfo(int)));
    connect(ui->serialPortInfoListBox_2, SIGNAL(currentIndexChanged(int)), this, SLOT(showPortInfo_2(int)));

    ui->tabWidget->setCurrentIndex(0);

    fillPortsInfo();
    fillPortsParameters();

    readSetting();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::fillPortsInfo()
{
    ui->serialPortInfoListBox->clear();
    ui->serialPortInfoListBox_2->clear();
    QString description;
    QString manufacturer;
    QString serialNumber;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : "N/A")
             << (!manufacturer.isEmpty() ? manufacturer : "N/A")
             << (!serialNumber.isEmpty() ? serialNumber : "N/A")
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : "N/A")
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : "N/A");

        ui->serialPortInfoListBox->addItem(list.first(), list);
        ui->serialPortInfoListBox_2->addItem(list.first(), list);
    }

    ui->serialPortInfoListBox->addItem(tr("N/A"));
    ui->serialPortInfoListBox_2->addItem(tr("N/A"));
}

void SettingsDialog::fillPortsParameters()
{
    ui->baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->baudRateBox->addItem(tr("Custom"));

    ui->dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->dataBitsBox->setCurrentIndex(3);

    ui->parityBox->addItem(tr("None"), QSerialPort::NoParity);
    ui->parityBox->addItem(tr("Even"), QSerialPort::EvenParity);
    ui->parityBox->addItem(tr("Odd"), QSerialPort::OddParity);
    ui->parityBox->addItem(tr("Mark"), QSerialPort::MarkParity);
    ui->parityBox->addItem(tr("Space"), QSerialPort::SpaceParity);

    ui->stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->stopBitsBox->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    ui->stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    ui->flowControlBox->addItem(tr("None"), QSerialPort::NoFlowControl);
    ui->flowControlBox->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    ui->flowControlBox->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);

    ui->baudRateBox_2->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->baudRateBox_2->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->baudRateBox_2->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->baudRateBox_2->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->baudRateBox_2->addItem(tr("Custom"));

    ui->dataBitsBox_2->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->dataBitsBox_2->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->dataBitsBox_2->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->dataBitsBox_2->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->dataBitsBox_2->setCurrentIndex(3);

    ui->parityBox_2->addItem(tr("None"), QSerialPort::NoParity);
    ui->parityBox_2->addItem(tr("Even"), QSerialPort::EvenParity);
    ui->parityBox_2->addItem(tr("Odd"), QSerialPort::OddParity);
    ui->parityBox_2->addItem(tr("Mark"), QSerialPort::MarkParity);
    ui->parityBox_2->addItem(tr("Space"), QSerialPort::SpaceParity);

    ui->stopBitsBox_2->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->stopBitsBox->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    ui->stopBitsBox_2->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    ui->flowControlBox_2->addItem(tr("None"), QSerialPort::NoFlowControl);
    ui->flowControlBox_2->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    ui->flowControlBox_2->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
}

void SettingsDialog::showPortInfo(int idx)
{
    if (idx == -1)
        return;

    QStringList list = ui->serialPortInfoListBox->itemData(idx).toStringList();
    ui->descriptionLabel->setText(tr("Description: %1").arg(list.count() > 1 ? list.at(1) : "N/A"));
    ui->manufacturerLabel->setText(tr("Manufacturer: %1").arg(list.count() > 2 ? list.at(2) : "N/A"));
    ui->serialNumberLabel->setText(tr("Serial number: %1").arg(list.count() > 3 ? list.at(3) : "N/A"));
    ui->locationLabel->setText(tr("Location: %1").arg(list.count() > 4 ? list.at(4) : "N/A"));
    ui->vidLabel->setText(tr("Vendor Identifier: %1").arg(list.count() > 5 ? list.at(5) : "N/A"));
    ui->pidLabel->setText(tr("Product Identifier: %1").arg(list.count() > 6 ? list.at(6) : "N/A"));
}

void SettingsDialog::showPortInfo_2(int idx)
{
    if (idx == -1)
        return;

    QStringList list = ui->serialPortInfoListBox_2->itemData(idx).toStringList();
    ui->descriptionLabel_2->setText(tr("Description: %1").arg(list.count() > 1 ? list.at(1) : "N/A"));
    ui->manufacturerLabel_2->setText(tr("Manufacturer: %1").arg(list.count() > 2 ? list.at(2) : "N/A"));
    ui->serialNumberLabel_2->setText(tr("Serial number: %1").arg(list.count() > 3 ? list.at(3) : "N/A"));
    ui->locationLabel_2->setText(tr("Location: %1").arg(list.count() > 4 ? list.at(4) : "N/A"));
    ui->vidLabel_2->setText(tr("Vendor Identifier: %1").arg(list.count() > 5 ? list.at(5) : "N/A"));
    ui->pidLabel_2->setText(tr("Product Identifier: %1").arg(list.count() > 6 ? list.at(6) : "N/A"));
}

void SettingsDialog::on_pbApply_clicked()
{
    writeSetting();
    this->close();
}

void SettingsDialog::on_pbCancel_clicked()
{
    this->close();
}

void SettingsDialog::setSetting()
{
    //** FINGER_PRINT **//

    for (int i = ui->serialPortInfoListBox->count(); i >= 0; i--) {
        ui->serialPortInfoListBox->setCurrentIndex(i);
        if (ui->serialPortInfoListBox->currentText() == this->s[0].name) {
            break;
        }
    }
    ui->baudRateBox->setCurrentIndex(this->s[0].c_baudRate_id);
    ui->dataBitsBox->setCurrentIndex(this->s[0].c_dataBits_id);
    ui->parityBox->setCurrentIndex(this->s[0].c_parity_id);
    ui->stopBitsBox->setCurrentIndex(this->s[0].c_stopBits_id);
    ui->flowControlBox->setCurrentIndex(this->s[0].c_flowControl_id);

    //** RFID **//

    for (int i = ui->serialPortInfoListBox->count(); i >= 0; i--) {
        ui->serialPortInfoListBox->setCurrentIndex(i);
        if (ui->serialPortInfoListBox->currentText() == this->s[1].name) {
            break;
        }
    }
    ui->baudRateBox->setCurrentIndex(this->s[1].c_baudRate_id);
    ui->dataBitsBox->setCurrentIndex(this->s[1].c_dataBits_id);
    ui->parityBox->setCurrentIndex(this->s[1].c_parity_id);
    ui->stopBitsBox->setCurrentIndex(this->s[1].c_stopBits_id);
    ui->flowControlBox->setCurrentIndex(this->s[1].c_flowControl_id);
}

void SettingsDialog::getSetting()
{
    //** FINGER_PRINT **//

    this->s[0].name = ui->serialPortInfoListBox->currentText();
    this->s[0].c_name_id = ui->serialPortInfoListBox->currentIndex();

    this->s[0].baudRate = static_cast<QSerialPort::BaudRate>(
                ui->baudRateBox->currentData().toInt());
    this->s[0].c_baudRate_id = ui->baudRateBox->currentIndex();
    this->s[0].stringBaudRate = ui->baudRateBox->currentText();

    this->s[0].dataBits = static_cast<QSerialPort::DataBits>(
                ui->dataBitsBox->currentData().toInt());
    this->s[0].c_dataBits_id = ui->dataBitsBox->currentIndex();
    this->s[0].stringDataBits = ui->dataBitsBox->currentText();

    this->s[0].parity = static_cast<QSerialPort::Parity>(
                ui->parityBox->currentData().toInt());
    this->s[0].c_parity_id = ui->parityBox->currentIndex();
    this->s[0].stringParity = ui->parityBox->currentText();

    this->s[0].stopBits = static_cast<QSerialPort::StopBits>(
                ui->stopBitsBox->currentData().toInt());
    this->s[0].c_stopBits_id = ui->stopBitsBox->currentIndex();
    this->s[0].stringStopBits = ui->stopBitsBox->currentText();

    this->s[0].flowControl = static_cast<QSerialPort::FlowControl>(
                ui->flowControlBox->currentData().toInt());
    this->s[0].c_flowControl_id = ui->flowControlBox->currentIndex();
    this->s[0].stringFlowControl = ui->flowControlBox->currentText();

    //** RFID **//

    this->s[1].name = ui->serialPortInfoListBox_2->currentText();
    this->s[1].c_name_id = ui->serialPortInfoListBox_2->currentIndex();

    this->s[1].baudRate = static_cast<QSerialPort::BaudRate>(
                ui->baudRateBox_2->currentData().toInt());
    this->s[1].c_baudRate_id = ui->baudRateBox_2->currentIndex();
    this->s[1].stringBaudRate = ui->baudRateBox_2->currentText();

    this->s[1].dataBits = static_cast<QSerialPort::DataBits>(
                ui->dataBitsBox_2->currentData().toInt());
    this->s[1].c_dataBits_id = ui->dataBitsBox_2->currentIndex();
    this->s[1].stringDataBits = ui->dataBitsBox_2->currentText();

    this->s[1].parity = static_cast<QSerialPort::Parity>(
                ui->parityBox_2->currentData().toInt());
    this->s[1].c_parity_id = ui->parityBox_2->currentIndex();
    this->s[1].stringParity = ui->parityBox_2->currentText();

    this->s[1].stopBits = static_cast<QSerialPort::StopBits>(
                ui->stopBitsBox_2->currentData().toInt());
    this->s[1].c_stopBits_id = ui->stopBitsBox_2->currentIndex();
    this->s[1].stringStopBits = ui->stopBitsBox_2->currentText();

    this->s[1].flowControl = static_cast<QSerialPort::FlowControl>(
                ui->flowControlBox_2->currentData().toInt());
    this->s[1].c_flowControl_id = ui->flowControlBox_2->currentIndex();
    this->s[1].stringFlowControl = ui->flowControlBox_2->currentText();
}

void SettingsDialog::readSetting()
{
    if (QFile("settings.ini").exists()) {
        QSettings sett("settings.ini", QSettings::IniFormat);

        this->s[0].name = sett.value("FINGER_PRINT/NAME").toString();
        this->s[0].c_baudRate_id = sett.value("FINGER_PRINT/C_BAUDRATE_ID").toInt();
        this->s[0].c_dataBits_id = sett.value("FINGER_PRINT/C_DATA_BITS_ID").toInt();
        this->s[0].c_parity_id = sett.value("FINGER_PRINT/C_PARITY_ID").toInt();
        this->s[0].c_stopBits_id = sett.value("FINGER_PRINT/C_STOP_BITS_ID").toInt();
        this->s[0].c_flowControl_id = sett.value("FINGER_PRINT/C_FLOW_CONTROL_ID").toInt();

        this->s[1].name = sett.value("RFID/NAME").toString();
        this->s[1].c_baudRate_id = sett.value("RFID/C_BAUDRATE_ID").toInt();
        this->s[1].c_dataBits_id = sett.value("RFID/C_DATA_BITS_ID").toInt();
        this->s[1].c_parity_id = sett.value("RFID/C_PARITY_ID").toInt();
        this->s[1].c_stopBits_id = sett.value("RFID/C_STOP_BITS_ID").toInt();
        this->s[1].c_flowControl_id = sett.value("RFID/C_FLOW_CONTROL_ID").toInt();

        setSetting();
    }
}

void SettingsDialog::writeSetting()
{
    getSetting();
    QSettings sett("settings.ini", QSettings::IniFormat);

    //** FINGER PRINT **//

    sett.beginGroup("FINGER_PRINT");
    sett.setValue("NAME", this->s[0].name);
    sett.setValue("C_NAME_ID", this->s[0].c_name_id);

    sett.setValue("BAUDRATE", this->s[0].baudRate);
    sett.setValue("C_BAUDRATE_ID", this->s[0].c_baudRate_id);
    sett.setValue("STRING_BAUDRATE", this->s[0].stringBaudRate);

    sett.setValue("DATA_BITS", this->s[0].dataBits);
    sett.setValue("C_DATA_BITS_ID", this->s[0].c_dataBits_id);
    sett.setValue("STRING_DATA_BITS", this->s[0].stringDataBits);

    sett.setValue("PARITY", this->s[0].parity);
    sett.setValue("C_PARITY_ID", this->s[0].c_parity_id);
    sett.setValue("STRING_PARITY", this->s[0].stringParity);

    sett.setValue("STOP_BITS", this->s[0].stopBits);
    sett.setValue("C_STOP_BITS_ID", this->s[0].c_stopBits_id);
    sett.setValue("STRING_STOP_BITS", this->s[0].stringStopBits);

    sett.setValue("FLOW_CONTROL", this->s[0].flowControl);
    sett.setValue("C_FLOW_CONTROL_ID", this->s[0].c_flowControl_id);
    sett.setValue("STRING_FLOW_CONTROL", this->s[0].stringFlowControl);
    sett.endGroup();

    //** RFID **//

    sett.setValue("RFID/NAME", this->s[1].name);
    sett.setValue("RFID/C_NAME_ID", this->s[1].c_name_id);

    sett.setValue("RFID/BAUDRATE", this->s[1].baudRate);
    sett.setValue("RFID/C_BAUDRATE_ID", this->s[1].c_baudRate_id);
    sett.setValue("RFID/STRING_BAUDRATE", this->s[1].stringBaudRate);

    sett.setValue("RFID/DATA_BITS", this->s[1].dataBits);
    sett.setValue("RFID/C_DATA_BITS_ID", this->s[1].c_dataBits_id);
    sett.setValue("RFID/STRING_DATA_BITS", this->s[1].stringDataBits);

    sett.setValue("RFID/PARITY", this->s[1].parity);
    sett.setValue("RFID/C_PARITY_ID", this->s[1].c_parity_id);
    sett.setValue("RFID/STRING_PARITY", this->s[1].stringParity);

    sett.setValue("RFID/STOP_BITS", this->s[1].stopBits);
    sett.setValue("RFID/C_STOP_BITS_ID", this->s[1].c_stopBits_id);
    sett.setValue("RFID/STRING_STOP_BITS", this->s[1].stringStopBits);

    sett.setValue("RFID/FLOW_CONTROL", this->s[1].flowControl);
    sett.setValue("RFID/C_FLOW_CONTROL_ID", this->s[1].c_flowControl_id);
    sett.setValue("RFID/STRING_FLOW_CONTROL", this->s[1].stringFlowControl);
}
