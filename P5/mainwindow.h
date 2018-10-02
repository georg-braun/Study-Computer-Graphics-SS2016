#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QtCore>
#include<QLabel>


#include <mutex>

#include "ardata.h"
#include "detector.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;

    Detector myCvDetector ;


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    ArData arData ;

    void startProgram() ;


private slots:
    void on_btnCalibration_clicked();
    void on_btnGenerateMarker_clicked();
};

#endif // MAINWINDOW_H
