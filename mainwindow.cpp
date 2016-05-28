#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::startProgram() {

    // Detector und OpenGL an gemeinsame Datenbasis anklemmen
    myCvDetector.arDataPtr = &arData ;
    ui->widget->arDataPtr = &arData ;

    //myCvDetector.attachArData();
    //ui->widget->attachArData();


    /*
    myCvDetector.drawArPtr = &(ui->widget->drawAR);
    myCvDetector.detectorInitializedPtr = &(ui->widget->detectorInitialized);
    myCvDetector.texPtr = &(ui->widget->tex) ;
    myCvDetector.modelView_matrixPtr = &(ui->widget->modelView_matrix);
    myCvDetector.cameraMatrixPtr = &(ui->widget->cameraMatrix);
    */

   // w.attachOriginalImageToLabel( myCvDetector . lblWebcamOriginal );
   // w.attachDetectedImageToLabel( myCvDetector . lblWebcamDetected );

    myCvDetector.startDetection();
    myCvDetector.start();

    ui->widget->loadProjectionMatrix(); // Camera Parameters should be set, calc the Projection ;)


}

void MainWindow::attachOriginalImageToLabel( QLabel* lblDetectorRef ) {

    lblDetectorRef = ui->lblOriginal ;
}


void MainWindow::attachDetectedImageToLabel( QLabel* lblDetectorRef ) {
    lblDetectorRef = ui->lblThresh ;
}

void MainWindow::attachDetectorToGlImage ( QImage& image ) {
   // image = &(ui->widget->tex) ;

}
