#ifndef ARDATA_H
#define ARDATA_H

#include<QtCore>
#include<QImage>
#include<opencv2/core/core.hpp>



class ArData
{
public:
    ArData();

    QMutex mutex ;

    bool drawAR = false ;
    bool detectorInitialized = false ;
    cv::Mat cameraMatrix;
    QImage tex;
    cv::Mat modelView_matrix ;



};

#endif // ARDATA_H
