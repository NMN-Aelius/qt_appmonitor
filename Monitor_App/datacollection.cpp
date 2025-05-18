#include "datacollection.h"

SerialWorker::SerialWorker(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
    connect(serial, &QSerialPort::readyRead, this, &SerialWorker::handleReadyRead);
}

SerialWorker::~SerialWorker()
{
    if(serial->isOpen())
        serial->close();
}

void SerialWorker::startSerial(const QString &portName)
{
    if(serial->isOpen())
        serial->close();

    serial->setPortName(portName);
    serial->setBaudRate(QSerialPort::Baud115200);
    // ... thiết lập thêm tùy theo nhu cầu

    if(!serial->open(QIODevice::ReadWrite))
    {
        emit errorOccurred(serial->errorString());
    }
}

void SerialWorker::stopSerial()
{
    if(serial->isOpen())
        serial->close();
}

void SerialWorker::writeData(const QByteArray &data)
{
    if(serial->isOpen())
        serial->write(data);
}

void SerialWorker::handleReadyRead()
{
    QByteArray data = serial->readAll();
    emit dataReceived(data);
}
