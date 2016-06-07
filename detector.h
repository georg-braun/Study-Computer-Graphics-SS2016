#ifndef DETECTOR_H
#define DETECTOR_H

#include <QMainWindow>
#include <vector>
#include <iostream>
#include <ctime>
#include <QLabel>

#include<QtCore>
#include<QMessageBox>
#include<string.h>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/aruco.hpp>
#include<opencv2/calib3d.hpp>
#include <opencv2/core/cvstd.hpp>
#include "ardata.h"



//class Detector : public QMainWindow
class Detector : public QThread
{
    Q_OBJECT

private:

    cv::VideoCapture capWebcam;             // Capture object to use with webcam
    QImage convertOpenCVMatToQtQImage(cv::Mat mat);       // function prototype
    cv::Ptr<cv::aruco::Dictionary> dictionary ;

    bool marker0Detected ;
    bool marker1Detected ;
    bool marker2Detected ;
    int  marker0ArrayPos ;
    int  marker1ArrayPos ;
    int  marker2ArrayPos ;


    // Infos
    std::vector< int > ids;
    std::vector< std::vector< cv::Point2f > > corners, rejected;
    cv::Mat imageOriginal, imageCopy;
    bool estimatePos = true ;

    // Calibration
    cv::Size imgSize;
    cv::Ptr<cv::aruco::DetectorParameters> detectorParams ;
    cv::Mat cameraMatrix, distCoeffs;
    std::vector< cv::Vec3d > rvecs, tvecs;

    bool readCameraParameters(std::string filename) ;
    bool initializeDetection();

    void calcModelViewMatrices();
    void exitProgram();                    // function prototype

    QImage      mRenderQtImg;           /// Qt image to be rendered
    cv::Mat     mOrigImage;             /// original OpenCV image to be shown
    void convertImage(cv::Mat image);

public:    
    Detector();

    void processFrameAndUpdateGUI();                // function prototype
    bool runDetection = true ;
    void run() ;    // Thread Method

    void startDetection();

    ArData * arDataPtr ;
    QLabel * lblOriginal ;

    cv::Mat_<double> marker0ModelView ;
    cv::Mat_<double> marker1ModelView ;
    cv::Mat_<double> marker2ModelView ;

};

#endif // DETECTOR_H
