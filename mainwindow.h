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

    std::mutex imageMutex ;
    ArData arData ;

    void startProgram() ;

    void attachOriginalImageToLabel( QLabel* lblDetectorRef ) ;
    void attachDetectedImageToLabel( QLabel* lblDetectorRef ) ;
    void attachDetectorToGlImage ( QImage& image ) ;

};

#endif // MAINWINDOW_H
