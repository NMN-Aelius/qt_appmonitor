#ifndef DATACOLLECTION_H
#define DATACOLLECTION_H

#include <QObject>
#include <QSerialPort>

class SerialWorker : public QObject
{
    Q_OBJECT
public:
    explicit SerialWorker(QObject *parent = nullptr);
    ~SerialWorker();

public slots:
    void startSerial(const QString &portName);
    void stopSerial();
    void writeData(const QByteArray &data);

signals:
    void dataReceived(const QByteArray &data);
    void errorOccurred(const QString &error);

private slots:
    void handleReadyRead();

private:
    QSerialPort *serial;
};

#endif // DATACOLLECTION_H
