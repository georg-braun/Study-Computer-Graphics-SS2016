#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QGLWidget>
#include <QWidget>
#include <QTimer>
//#include "modelloader.h"
#include <QOpenGLTexture>
#include <QtDebug>
#include <vector>

#include "ardata.h"

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

class MyGLWidget : public QGLWidget
{
private:
    Q_OBJECT    // declare own signals and slots (MOC-System)
protected:

    QTimer *timer = new QTimer(this) ;

    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void perspectiveGL( GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar );

    QImage      mRenderQtImg;           /// Qt image to be rendered
    cv::Mat     mOrigImage;             /// original OpenCV image to be shown


    int         mOutH;                  /// Resized Image height
    int         mOutW;                  /// Resized Image width
    float       mImgRatio;             /// height/width ratio
    cv::Mat_<double> persp ;
    void drawAxis();

    void fetchArData() ;

public:
    MyGLWidget();
    MyGLWidget(QWidget *parent); // Konstruktur noch angeben?
    GLuint backgroundimage ;
    void showImage(cv::Mat image);
    QImage loadTexture2(char *filename, GLuint &textureID);

    std::vector<cv::Vec3f> v3fCircles ;
    void sendMakerPos(std::vector<cv::Vec3f> _v3fCircles) ;

     ArData * arDataPtr ;
     void attachArData();

    bool drawAR = false ;
    bool        readyToCalcProjection = false ;

    cv::Mat cameraMatrix;                // Attached to Detector
    QImage tex;                          // Attached to Detector
    cv::Mat modelView_matrix ;           // Attached to Detector
    bool drawAr = false ;                // Attached to Detector
    bool detectorInitialized = false ;   // Attached to Detector
    bool projectionCalculated = false ;  // Attached to Detector
    void loadProjectionMatrix() ;        // Invoked Outside (wait for Detector to calc. Camera Matrix)

    /*
    bool * drawArPtr ;  // Marker erkannt?
    bool * detectorInitializedPtr; // OpenCV Detector liefert Bilder?
    QImage * texPtr ;
    cv::Mat * modelView_matrixPtr ;
    cv::Mat * cameraMatrixPtr ;
    */

};

#endif // MYGLWIDGET_H
