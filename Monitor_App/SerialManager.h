#ifndef SERIALMANAGER_H
#define SERIALMANAGER_H


#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>


class SerialManager :public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList availablePorts READ availablePorts NOTIFY availablePortsChanged)

public:
    explicit SerialManager(QObject *parent = nullptr);
    QStringList availablePorts();

    Q_INVOKABLE void connectToPort(const QString &portName);
    Q_INVOKABLE void disconnectPort();

signals:
    void dataReceived(const QString &data);
    void availablePortsChanged();

private slots:
    void readData();

private:
    QSerialPort serial;
};

#endif // SERIALMANAGER_H
