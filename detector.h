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

#include <mutex>

class Detector : public QMainWindow
{
    Q_OBJECT

private:

    cv::VideoCapture capWebcam;             // Capture object to use with webcam
    QTimer* qtimer;                 // timer for processFrameAndUpdateGUI()
    QImage convertOpenCVMatToQtQImage(cv::Mat mat);       // function prototype
    cv::Ptr<cv::aruco::Dictionary> dictionary ;


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

    cv::Mat modelViewFirstId ;
    void calcModelViewMatrixFirstId();
    void exitProgram();                    // function prototype


    QImage      mRenderQtImg;           /// Qt image to be rendered
    cv::Mat     mOrigImage;             /// original OpenCV image to be shown
    void showImage(cv::Mat image);
public slots:
    void processFrameAndUpdateGUI();                // function prototype

public:    
    Detector();


    QLabel* lblWebcamOriginal ;
    QLabel* lblWebcamDetected ;

    void startDetection();
    void stopDetection();

    int xptr = 0 ;
    bool * drawArPtr ;  // Marker erkannt?
    bool * detectorInitializedPtr; // OpenCV Detector liefert Bilder?
    QImage * texPtr ;
    cv::Mat * modelView_matrixPtr ;
    cv::Mat * cameraMatrixPtr ;

    std::mutex * mutexPtr ;

};

#endif // DETECTOR_H
