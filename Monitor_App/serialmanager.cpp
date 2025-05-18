#include "SerialManager.h"
#include <QDebug>

SerialManager::SerialManager(QObject *parent) : QObject(parent)
{
    connect(&serial, &QSerialPort::readyRead, this, &SerialManager::readData);
}

QStringList SerialManager::availablePorts()
{
    QStringList list;
    const auto ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : ports) {
        list << info.portName();
    }
    return list;
}

void SerialManager::connectToPort(const QString &portName)
{
    if (serial.isOpen())
        serial.close();

    serial.setPortName(portName);
    serial.setBaudRate(QSerialPort::Baud9600);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    if (!serial.open(QIODevice::ReadWrite)) {
        qWarning() << "Cannot open serial port:" << serial.errorString();
    } else {
        qDebug() << "Connected to" << portName;
    }
}

void SerialManager::disconnectPort()
{
    if (serial.isOpen())
        serial.close();
}

void SerialManager::readData()
{
    const QByteArray data = serial.readAll();
    emit dataReceived(QString::fromUtf8(data));
}
