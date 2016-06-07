#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QGLWidget>
#include <QWidget>
#include <QTimer>
//#include "modelloader.h"
#include <QOpenGLTexture>
#include <QtDebug>
#include <vector>
#include <stack>

#include "ardata.h"

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<QOpenGLBuffer>
#include<QOpenGLShaderProgram>

#include"modelloader.h"

typedef struct {
    GLfloat x, y, z, h;
    GLfloat r, g, b, t;
} Vertex;
static const int verticesCount = 8 ;

class MyGLWidget : public QGLWidget
{
private:
    Q_OBJECT    // declare own signals and slots (MOC-System)
protected:

    QTimer timer ;//= new QTimer(this) ;

    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

    // Fetched Data from ArData Class
    cv::Mat cameraMatrix;
    QImage  tex;
    cv::Mat marker0ModelView ;
    cv::Mat marker1ModelView ;
    cv::Mat marker2ModelView ;
    bool marker0Detected = false ;
    bool marker1Detected = false ;
    bool marker2Detected = false ;
    bool    drawAr = false ;
    bool    detectorInitialized = false ;
    bool    projectionCalculated = false ;

    cv::Mat_<double> persp ;

    void drawAxis();
    void fetchArData() ;

    // Buffer
    QOpenGLBuffer vboMarker0 ;
    QOpenGLBuffer iboMarker0 ;
    QOpenGLBuffer vboMarker1 ;
    QOpenGLBuffer iboMarker1 ;
    QOpenGLBuffer vboMarker2 ;
    QOpenGLBuffer iboMarker2 ;


    int attrVertices = 0;
    int unifMatrix = 0 ;

    //QOpenGLBuffer vboArray[5] ;
    //QOpenGLBuffer iboArray[5] ;

    void loadModel() ;

 //-------------------------
    // Dynamisch
    GLfloat* vboData[5] ;
    GLuint* indexData[5] ;
    unsigned int vboLength[5] ;
    unsigned int iboLength[5] ;



    // Buffer
    void initalizeBuffer();
    void fillBuffer();

/*
    void addVertice(int     verticeNo ,
                     GLfloat x ,
                     GLfloat y ,
                     GLfloat z ,
                     GLfloat r ,
                     GLfloat g ,
                     GLfloat b ) ;
*/
    // Shader
    QOpenGLShaderProgram shaderProgram;
    void initalizeShader();

    // Matritzen
    std::stack<QMatrix4x4> projectionMatrixStack ;
    std::stack<QMatrix4x4> modelViewMatrixStack ;
//-------------------------


public:
    MyGLWidget();
    MyGLWidget(QWidget *parent); // Konstruktur noch angeben?

    ArData * arDataPtr ;

    bool drawAR = false ;
    bool readyToCalcProjection = false ;

    void loadProjectionMatrix() ;        // Invoked Outside (wait for Detector to calc. Camera Matrix)

};

#endif // MYGLWIDGET_H
