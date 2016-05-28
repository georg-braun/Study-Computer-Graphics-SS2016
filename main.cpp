#include "mainwindow.h"
#include <QApplication>

#include "calibrator.h"
#include "detector.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    /*
    //Calibrator myCvCalibrator ;
    //myCvCalibrator.calibrateCamera();

    Detector myCvDetector ;
   // w.attachOriginalImageToLabel( myCvDetector . lblWebcamOriginal );
   // w.attachDetectedImageToLabel( myCvDetector . lblWebcamDetected );

    w.attachDetectorToGlImage(myCvDetector.tex);
    myCvDetector.startDetection();
*/

    return a.exec();
}
