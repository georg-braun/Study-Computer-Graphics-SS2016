#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // Slider: Z-Rotation Connection
    //connect(ui->sldRotationZ, SIGNAL(valueChanged(int)), ui->widget, SLOT(receiveRotationZ(int)) );
    //connect(ui->widget, SIGNAL(zoomFactorChanged(int)), ui->sbxZoom, SLOT(setValue(int)));
/*
    QTimer *timer = new QTimer(this) ;
    connect(timer, SIGNAL(timeout()),ui->widget,SLOT(updateGL()));
    timer->start(40);
    */
}

MainWindow::~MainWindow()
{
    delete ui;

}
