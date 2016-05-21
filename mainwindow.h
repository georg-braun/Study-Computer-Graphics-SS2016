#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

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
    void MainWindow::exitProgram();                    // function prototype
public slots:
    void processFrameAndUpdateGUI();                // function prototype

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

};

#endif // MAINWINDOW_H
