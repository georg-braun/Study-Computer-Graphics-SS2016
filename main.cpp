#include "mainwindow.h"
#include <QApplication>

#include "calibrator.h"
#include "detector.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    w.startProgram();

    // Calibration
    /*
    Calibrator myCvCalibrator ;
    myCvCalibrator.calibrateCamera();
    */

    return a.exec();
}
