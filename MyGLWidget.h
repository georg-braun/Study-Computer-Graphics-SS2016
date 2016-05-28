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

    QTimer timer = new QTimer(this) ;

    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

    // Fetched Data from ArData Class
    cv::Mat cameraMatrix;
    QImage  tex;
    cv::Mat modelView_matrix ;
    bool    drawAr = false ;
    bool    detectorInitialized = false ;
    bool    projectionCalculated = false ;

    cv::Mat_<double> persp ;

    void drawAxis();
    void fetchArData() ;

public:
    MyGLWidget();
    MyGLWidget(QWidget *parent); // Konstruktur noch angeben?

    ArData * arDataPtr ;

    bool drawAR = false ;
    bool readyToCalcProjection = false ;

    void loadProjectionMatrix() ;        // Invoked Outside (wait for Detector to calc. Camera Matrix)

};

#endif // MYGLWIDGET_H
