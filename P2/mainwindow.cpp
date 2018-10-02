#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Slider: Z-Rotation Connection
    //connect(ui->sldRotationZ, SIGNAL(valueChanged(int)), ui->widget, SLOT(receiveRotationZ(int)) );
    connect(ui->widget, SIGNAL(zoomFactorChanged(int)), ui->sbxZoom, SLOT(setValue(int)));


}

MainWindow::~MainWindow()
{
    delete ui;

}
