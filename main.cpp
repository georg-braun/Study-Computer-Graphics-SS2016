#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    bool runDetection = true ;
    bool runCalibration = false ;

    if ( runCalibration ) {
        w.calibrateCamera();
    }
    if ( runDetection ) {
        w.startDetection();
    }

    return a.exec();
}
