#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    connect(ui->serialPortInfoListBox, SIGNAL(currentIndexChanged(int)), this, SLOT(showPortInfo(int)));

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
    }

    ui->serialPortInfoListBox->addItem(tr("N/A"));
}

void SettingsDialog::fillPortsParameters()
{
    ui->baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->baudRateBox->addItem(QStringLiteral("57600"), QSerialPort::Baud57600);
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
    for (int i = ui->serialPortInfoListBox->count(); i >= 0; i--) {
        ui->serialPortInfoListBox->setCurrentIndex(i);
        if (ui->serialPortInfoListBox->currentText() == this->s.name) {
            break;
        }
    }
    ui->baudRateBox->setCurrentIndex(this->s.c_baudRate_id);
    ui->dataBitsBox->setCurrentIndex(this->s.c_dataBits_id);
    ui->parityBox->setCurrentIndex(this->s.c_parity_id);
    ui->stopBitsBox->setCurrentIndex(this->s.c_stopBits_id);
    ui->flowControlBox->setCurrentIndex(this->s.c_flowControl_id);
}

void SettingsDialog::getSetting()
{
    this->s.name = ui->serialPortInfoListBox->currentText();
    this->s.c_name_id = ui->serialPortInfoListBox->currentIndex();

    this->s.baudRate = static_cast<QSerialPort::BaudRate>(
                ui->baudRateBox->currentData().toInt());
    this->s.c_baudRate_id = ui->baudRateBox->currentIndex();
    this->s.stringBaudRate = ui->baudRateBox->currentText();

    this->s.dataBits = static_cast<QSerialPort::DataBits>(
                ui->dataBitsBox->currentData().toInt());
    this->s.c_dataBits_id = ui->dataBitsBox->currentIndex();
    this->s.stringDataBits = ui->dataBitsBox->currentText();

    this->s.parity = static_cast<QSerialPort::Parity>(
                ui->parityBox->currentData().toInt());
    this->s.c_parity_id = ui->parityBox->currentIndex();
    this->s.stringParity = ui->parityBox->currentText();

    this->s.stopBits = static_cast<QSerialPort::StopBits>(
                ui->stopBitsBox->currentData().toInt());
    this->s.c_stopBits_id = ui->stopBitsBox->currentIndex();
    this->s.stringStopBits = ui->stopBitsBox->currentText();

    this->s.flowControl = static_cast<QSerialPort::FlowControl>(
                ui->flowControlBox->currentData().toInt());
    this->s.c_flowControl_id = ui->flowControlBox->currentIndex();
    this->s.stringFlowControl = ui->flowControlBox->currentText();
}

void SettingsDialog::readSetting()
{
    if (QFile("settings.ini").exists()) {
        QSettings sett("settings.ini", QSettings::IniFormat);

        this->s.name = sett.value("SERIAL/NAME").toString();
        this->s.c_baudRate_id = sett.value("SERIAL/C_BAUDRATE_ID").toInt();
        this->s.c_dataBits_id = sett.value("SERIAL/C_DATA_BITS_ID").toInt();
        this->s.c_parity_id = sett.value("SERIAL/C_PARITY_ID").toInt();
        this->s.c_stopBits_id = sett.value("SERIAL/C_STOP_BITS_ID").toInt();
        this->s.c_flowControl_id = sett.value("SERIAL/C_FLOW_CONTROL_ID").toInt();

        setSetting();
    }
}

void SettingsDialog::writeSetting()
{
    getSetting();
    QSettings sett("settings.ini", QSettings::IniFormat);

    sett.beginGroup("SERIAL");
    sett.setValue("NAME", this->s.name);
    sett.setValue("C_NAME_ID", this->s.c_name_id);

    sett.setValue("BAUDRATE", this->s.baudRate);
    sett.setValue("C_BAUDRATE_ID", this->s.c_baudRate_id);
    sett.setValue("STRING_BAUDRATE", this->s.stringBaudRate);

    sett.setValue("DATA_BITS", this->s.dataBits);
    sett.setValue("C_DATA_BITS_ID", this->s.c_dataBits_id);
    sett.setValue("STRING_DATA_BITS", this->s.stringDataBits);

    sett.setValue("PARITY", this->s.parity);
    sett.setValue("C_PARITY_ID", this->s.c_parity_id);
    sett.setValue("STRING_PARITY", this->s.stringParity);

    sett.setValue("STOP_BITS", this->s.stopBits);
    sett.setValue("C_STOP_BITS_ID", this->s.c_stopBits_id);
    sett.setValue("STRING_STOP_BITS", this->s.stringStopBits);

    sett.setValue("FLOW_CONTROL", this->s.flowControl);
    sett.setValue("C_FLOW_CONTROL_ID", this->s.c_flowControl_id);
    sett.setValue("STRING_FLOW_CONTROL", this->s.stringFlowControl);
    sett.endGroup();
}
