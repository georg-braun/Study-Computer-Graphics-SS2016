#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>
#include <iostream>
#include <ctime>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/aruco.hpp>
#include<opencv2/calib3d.hpp>
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
    cv::Ptr<cv::aruco::Dictionary> dictionary ;



    // Infos
    std::vector< int > ids;
    std::vector< std::vector< cv::Point2f > > corners, rejected;
    cv::Mat imageOriginal, imageCopy;
    bool estimatePos = true ;

    // Calibration
    cv::Size imgSize;
    QTimer* qCalibrationTimer ;
    cv::Ptr<cv::aruco::GridBoard> gridboard ;
    cv::Ptr<cv::aruco::Board> board ;
    cv::Ptr<cv::aruco::DetectorParameters> detectorParams ;
    int calibrationFlags = 0;
    cv::Mat cameraMatrix, distCoeffs;
    std::vector< cv::Mat > rvecs, tvecs;
    double repError;
    std::vector< std::vector< cv::Point2f > > allCornersConcatenated;
    std::vector< int > allIdsConcatenated;
    std::vector< int > markerCounterPerFrame;
    const bool saveCameraParams(const std::string &filename, cv::Size imageSize, float aspectRatio, int flags,
                          const cv::Mat &cameraMatrix, const cv::Mat &distCoeffs, double totalAvgErr) ;
    bool readCameraParameters(std::string filename) ;
    // collected frames for calibration
    std::vector< std::vector< std::vector< cv::Point2f > > > allCorners;
    std::vector< std::vector< int > > allIds;

    // Private Methods
    void createMarker() ;
    void initializeCalibration();
    void checkCalbrationImages();
    void safeCalibration();


    bool initializeDetection();

    void MainWindow::exitProgram();                    // function prototype

public slots:
    void processFrameAndUpdateGUI();                // function prototype
    void calibration() ;

public:
    void startDetection();
    void stopDetection();
    void calibrateCamera() ;



    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

};

#endif // MAINWINDOW_H
