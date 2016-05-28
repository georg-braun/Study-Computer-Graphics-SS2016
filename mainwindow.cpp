#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //myCvDetector.xptr = &(ui->widget->xtest) ;
    //ui->widget->xtest = &(myCvDetector.xptr);
    myCvDetector.drawArPtr = &(ui->widget->drawAR);
    myCvDetector.detectorInitializedPtr = &(ui->widget->detectorInitialized);
    myCvDetector.texPtr = &(ui->widget->tex) ;
    myCvDetector.modelView_matrixPtr = &(ui->widget->modelView_matrix);
    myCvDetector.cameraMatrixPtr = &(ui->widget->cameraMatrix);

    myCvDetector.mutexPtr = &imageMutex ;
    ui->widget->mutexPtr = &imageMutex ;

   // w.attachOriginalImageToLabel( myCvDetector . lblWebcamOriginal );
   // w.attachDetectedImageToLabel( myCvDetector . lblWebcamDetected );

    //w.attachDetectorToGlImage(myCvDetector.tex);
    myCvDetector.startDetection();
    ui->widget->loadProjectionMatrix(); // Camera Parameters should be set, calc your Projection ;)

}


MainWindow::~MainWindow()
{
    delete ui;
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
