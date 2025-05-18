#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <qmessagebox.h>
#include <QFile>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = nullptr);
        ~MainWindow();

    private slots:
        void on_serConBtn_clicked();

        void on_serDisConnect_clicked();

        void updateSerialPort();

        void defaultBandwidth();

        void readData();

        void bg_Image_home();

    private:
        Ui::MainWindow *ui;

        QSerialPort *serial = nullptr;

        void createMenu();
        QMenu *mfile, *mview, *mhelp;
        QAction *fileNew, *fileOpen, *fileSave, *fileSaveAs;
        QAction *viewZoomIn, *viewZoomOut, *viewDefault;
        QAction *helpFeedback, *helpGuide;

        void onSaveTriggered();

};

#endif // MAINWINDOW_H
