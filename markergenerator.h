#ifndef MARKERGENERATOR_H
#define MARKERGENERATOR_H

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/aruco.hpp>

class MarkerGenerator
{
private:
    cv::Ptr<cv::aruco::Dictionary> dictionary ;
public:
    MarkerGenerator();
    void createMarker() ;
};

#endif // MARKERGENERATOR_H
