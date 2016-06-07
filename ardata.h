#ifndef ARDATA_H
#define ARDATA_H

#include<QtCore>
#include<QImage>
#include<opencv2/core/core.hpp>

#include<vector>

class ArData
{
public:
    ArData();

    QMutex mutex ;

    bool drawAR = false ;
    bool detectorInitialized = false ;
    cv::Mat cameraMatrix;
    QImage tex;
    cv::Mat marker0modelView_matrix ;
    cv::Mat marker1modelView_matrix ;
    cv::Mat marker2modelView_matrix ;
    bool marker0Detected ;
    bool marker1Detected ;
    bool marker2Detected ;



};

#endif // ARDATA_H
