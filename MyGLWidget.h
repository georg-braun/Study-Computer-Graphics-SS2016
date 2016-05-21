#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QGLWidget>
#include <QWidget>
#include <QTimer>
//#include "modelloader.h"
#include <QOpenGLTexture>
#include <QtDebug>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include "OGL_OCV_common.hpp"
class MyGLWidget : public QGLWidget
{
private:
    Q_OBJECT    // declare own signals and slots (MOC-System)
protected:

    QTimer *timer = new QTimer(this) ;

    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();


    QImage      mRenderQtImg;           /// Qt image to be rendered
    cv::Mat     mOrigImage;             /// original OpenCV image to be shown

    QColor      mBgColor;		/// Background color
    int         mOutH;                  /// Resized Image height
    int         mOutW;                  /// Resized Image width
    float       mImgRatio;             /// height/width ratio

    int         mPosX;                  /// Top left X position to render image in the center of widget
    int         mPosY;                  /// Top left Y position to render image in the center of widget

public:
    MyGLWidget();
    MyGLWidget(QWidget *parent); // Konstruktur noch angeben?
    GLuint backgroundimage ;
    void showImage(cv::Mat image);
    QImage loadTexture2(char *filename, GLuint &textureID);
    QImage tex;
};

#endif // MYGLWIDGET_H
