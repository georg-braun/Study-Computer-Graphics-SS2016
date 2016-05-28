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

   // w.attachOriginalImageToLabel( myCvDetector . lblWebcamOriginal );
   // w.attachDetectedImageToLabel( myCvDetector . lblWebcamDetected );

    myCvDetector.startDetection();
    myCvDetector.start();

    ui->widget->loadProjectionMatrix(); // Camera Parameters should be set, calc the Projection ;)


}

