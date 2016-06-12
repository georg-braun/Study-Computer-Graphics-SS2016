#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "calibrator.h"
#include "markergenerator.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}


MainWindow::~MainWindow()
{
    myCvDetector.runDetection = false ; // Thread zum Stoppen bringen.
    myCvDetector.wait(1000) ; // 1s Wartezeit, damit der Thread sich beenden kann.
    myCvDetector.terminate();
    delete ui;
}


void MainWindow::startProgram() {

    // Detector und OpenGL an gemeinsame Datenbasis anklemmen
    myCvDetector.arDataPtr = &arData ;
    ui->widget->arDataPtr = &arData ;
    myCvDetector.lblOriginal = ui->lblOriginal ;

    myCvDetector.startDetection();
    myCvDetector.start();

    ui->widget->loadProjectionMatrix(); // Camera Parameters should be set, calc the Projection ;)


}



void MainWindow::on_btnCalibration_clicked()
{
    Calibrator myCvCalibrator ;
    myCvCalibrator.calibrateCamera();
    QApplication::quit();
}

void MainWindow::on_btnGenerateMarker_clicked()
{
    MarkerGenerator generator ;
    generator.createMarker();
}
