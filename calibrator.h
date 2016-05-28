#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include <vector>
#include <iostream>
#include <ctime>
#include <QTimer>

#include<QtCore>
#include<QMessageBox>
#include<string.h>
#include<QObject>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/aruco.hpp>
#include<opencv2/calib3d.hpp>
#include <opencv2/core/cvstd.hpp>


class Calibrator : public QObject
{
    Q_OBJECT


    cv::VideoCapture capWebcam;             // Capture object to use with webcam
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
    std::vector< cv::Mat > rvecsCalib, tvecsCalib;
    double repError;
    std::vector< std::vector< cv::Point2f > > allCornersConcatenated;
    std::vector< int > allIdsConcatenated;
    std::vector< int > markerCounterPerFrame;
    const bool saveCameraParams(const std::string &filename, cv::Size imageSize, float aspectRatio, int flags,
                          const cv::Mat &cameraMatrix, const cv::Mat &distCoeffs, double totalAvgErr) ;
    // collected frames for calibration
    std::vector< std::vector< std::vector< cv::Point2f > > > allCorners;
    std::vector< std::vector< int > > allIds;

    // Private Methods

    void initializeCalibration();
    void checkCalbrationImages();
    void safeCalibration();


    void exitProgram();


public slots:
    void calibration() ;

public:
    void calibrateCamera() ;


public:
    Calibrator();
};

#endif // CALIBRATOR_H
