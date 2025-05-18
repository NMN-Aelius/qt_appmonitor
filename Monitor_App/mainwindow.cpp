#include "mainwindow.h"
#include "./ui_mainwindow.h"

//Multiple inheritance from many DAD
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createMenu();
    connect(ui->serConnect, &QPushButton::clicked, this, &MainWindow::on_serConBtn_clicked);
    connect(ui->serDisConnect, &QPushButton::clicked, this, &MainWindow::on_serDisConnect_clicked);
    connect(ui->cbPort, &QComboBox::showPopup, this, &MainWindow::updateSerialPort);
    updateSerialPort();
    defaultBandwidth();
    bg_Image_home();

}

MainWindow::~MainWindow()
{
    delete ui;
}

//Toolbar menu creation
void MainWindow::createMenu()
{
    //The shortcut is available with key "Alt+ First letter", with "&" character if dont want remove that one
    //So whe dont need to add specific more.
    fileNew = new QAction(tr("&New"), this); //"this" mean current main window app
    // fileNew->setShortcut(QKeySequence("Ctrl+N"));
    fileOpen = new QAction(tr("&Open"), this);
    fileSave = new QAction(tr("&Save"), this);
    connect(fileSave, &QAction::triggered, this, &MainWindow::onSaveTriggered);
    mfile = menuBar()->addMenu(tr("&File"));

    viewDefault = new QAction(tr("&Default"), this);
    viewZoomIn = new QAction(tr("&Zoom In"), this);
    viewZoomOut = new QAction(tr("&Zoom Out"), this);
    mview = menuBar()->addMenu(tr("&View"));

    helpGuide = new QAction(tr("&Guide"), this);
    helpFeedback = new QAction(tr("&Feedback"), this);
    mhelp = menuBar()->addMenu(tr("&Help"));

    mfile->addAction(fileNew);
    mfile->addAction(fileOpen);
    mfile->addAction(fileSave);

    mview->addAction(viewDefault);
    mview->addAction(viewZoomIn);
    mview->addAction(viewZoomOut);

    mhelp->addAction(helpFeedback);
    mhelp->addAction(helpGuide);
}

//Save file content!!
void MainWindow::onSaveTriggered()
{
    QFile file("File name saved.txt");
    if(file.open(QIODeviceBase::WriteOnly | QIODeviceBase::Text))
    {
        QTextStream output(&file);
        output<<"Save tại Main Window!!";
        file.close();
    }
    QMessageBox::information(this, "Save Window", "File của bạn đã được lưu!!");
}

void MainWindow::readData()
{
    QByteArray data = serial->readAll();
    while(data.contains("\n")&&data.contains(">"))
    {
        /*
         Len of string count from 1
         but index of string count from 0
         So index of \n is len of previous data
        */
        int lenOfStrWithStart = data.indexOf('\n');
        int lenOfStrNoStart = data.indexOf('>');
        QByteArray _data = data.left(lenOfStrWithStart).trimmed();
        _data = _data.mid(lenOfStrNoStart+1);

        //This append function auto move cursor to the next line
        ui->serialtable->append(QString::fromUtf8(_data));

        //Delete previous data for avoiding issue
        data.remove(0, lenOfStrWithStart+1);    //include \n character so we need to +1
        _data.remove(0, lenOfStrWithStart-1);   //eliminate 2 character '>' and '\n' so the left is size -1
    }
}

void MainWindow::on_serConBtn_clicked()
{
    QString portname = ui->cbPort->currentText();
    int baudrate = ui->cbChannel->currentText().toInt();

    serial = new QSerialPort(this);
    serial->setPortName(portname);
    serial->setBaudRate(baudrate);
    serial->setParity(QSerialPort::NoParity);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->setStopBits(QSerialPort::OneStop);

    if(serial->open(QIODeviceBase::ReadWrite))
    {
        ui->serialtable->clear();
        connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);
        ui->serConnect->setStyleSheet("background-color: green; color: black");
        ui->serDisConnect->setStyleSheet("background-color: white; color: black");
        ui->serConnect->setEnabled(false);
        ui->serDisConnect->setEnabled(true);
        // QMessageBox::information(this, "Success Serial", "Open done!!");
    }
    else
    {
        QMessageBox::critical(this, "Fail Serial", "Fail to open port!!"+ serial->errorString());
        delete serial;
        serial = NULL;
    }
}

void MainWindow::on_serDisConnect_clicked()
{
    if(serial->isOpen())
    {
        serial->close();
        if(!serial->isOpen())
        {
            ui->serConnect->setStyleSheet("background-color: white; color: black");
            ui->serDisConnect->setStyleSheet("background-color: red; color: black");
            ui->serConnect->setEnabled(true);
            ui->serDisConnect->setEnabled(false);
            // QMessageBox::information(this, "Success Serial", "Close done!!");
        }
        else
        {
            QMessageBox::critical(this, "Fail Serial", "Fail to close port!!"+ serial->errorString());
        }
    }
}

void MainWindow::updateSerialPort()
{
    ui->cbPort->clear();
    const auto state = QSerialPortInfo::availablePorts();
    //for(int i=0; i< state.size();i++)
    //{
    //    const QSerialPortInfo &ports = state[i];
    //}
    for(const QSerialPortInfo &ports : state)
    {
        ui->cbPort->addItem(ports.portName());
    }
}
void MainWindow::defaultBandwidth()
{
    ui->cbChannel->setCurrentText("115200");
}

void MainWindow::bg_Image_home()
{
    QPixmap pixmap(":/images/sword.jpg");
    //Asrgument set for image scale follow variable IgnoreAspectRatio
    pixmap = pixmap.scaled(ui->lblBgImage->size(), Qt::IgnoreAspectRatio);

    QPalette palette;
    // setBrush(QPalette::ColorRole role, const QBrush &brush)
    palette.setBrush(QPalette::Window, pixmap);
    ui->lblBgImage->setPalette(palette);
    ui->lblBgImage->setAutoFillBackground(true);
}
