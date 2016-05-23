#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/aruco.hpp>
#include <opencv2/core/cvstd.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;

    cv::VideoCapture capWebcam;             // Capture object to use with webcam
    QTimer* qtimer;                 // timer for processFrameAndUpdateGUI()
    QImage MainWindow::convertOpenCVMatToQtQImage(cv::Mat mat);       // function prototype
    cv::Ptr<cv::aruco::Dictionary> dictonary ;
    void MainWindow::exitProgram();                    // function prototype
public slots:
    void processFrameAndUpdateGUI();                // function prototype

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

};

#endif // MAINWINDOW_H
