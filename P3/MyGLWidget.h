#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H


#include <QOpenGLBuffer>        // Buffer
#include <QGLWidget>
#include <QWidget>
#include <QInputEvent>
//#include <QOpenGLFunctions_3_1>
#include <QOpenGLShaderProgram>  // Shader
#include <stack>
#include <modelloader.h>
#include <QOpenGLTexture>

#include <iostream>
#include "planet.h"
#include <QTimer>
#include <QElapsedTimer>




typedef struct {
    GLfloat x, y, z, h;
    GLfloat r, g, b, t;
} Vertex;

static const int verticesCount = 8 ;


class MyGLWidget : public QGLWidget
{


private:

    Q_OBJECT    // declare own signals and slots (MOC-System)

    // Transformation-Variables
    int     zRotation = 0 ;
    GLfloat zoom      = 0 ;

    // Datenspeicher Vertices & Indicies  
    // Dynamisch
    GLfloat* vboData ;
    GLuint* indexData ;
    unsigned int vboLength ;
    unsigned int iboLength ;

    // Matritzen
    QMatrix4x4 projectionMatrix ;
    std::stack<QMatrix4x4> modelMatrixStack ;

    // Camera
    QVector3D cameraPos = QVector3D(0.0f, 0.0f, 3.0f);
    QVector3D cameraFront = QVector3D(0.0f, 0.0f, -1.0f);
    QVector3D cameraUp = QVector3D(0.0f, 1.0f, 0.0f);
    GLfloat yaw   = -90.0f;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
    GLfloat pitch =   0.0f;


    // Buffer
    QOpenGLBuffer vbo;
    QOpenGLBuffer ibo;

    // Shader
    QOpenGLShaderProgram shaderProgram;

    // Textures
    QOpenGLTexture* qTex ;
    QOpenGLTexture *textures[12];


    // Modelle
    void loadModel();

    // Planeten
    void createPlantes();

    // Zeitmessung
    QElapsedTimer tmrRender ; // Misst vergangene Zeit zwischen Rendern ;

    // Lokalisiere bzw. definiere die Schnittstelle für die Eckpunkte und Texturen, ..
    int attrVertices = 0;
    int attrTexCoords = 3 ;
    int unifMatrixPerspective = 0 ;
    int unifMatrixView = 0 ;



protected:

    int unifMatrixModel = 0 ;       // Uniform Value für Model Matrix. Hier deklariert, um Referenz an die Planten Klasse zu übergeben.
    int elapsedTime = 0 ;           // Zeit zwischen dem Rendern. Referenz wird an die Planeten übergeben, damit die damit arbeiten können.

    // Planeten
    Planet sonne, merkur, venus, erde, erdemond, mars, phobos, deimos, jupiter, saturn, uranus, neptun;

    // Paused
    bool paused = false ;

    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();


    // Buffer
    void initalizeBuffer();
    void fillBuffer();

    // Shader
    void initalizeShader();

    // Textures
    void initializeTextures();

    // Planeten
    void initializePlanets();

    // Events
    void wheelEvent(QWheelEvent * event );
    void keyPressEvent(QKeyEvent *event);


public:
    MyGLWidget();
    MyGLWidget(QWidget *parent); // Konstruktur noch angeben?
    ~MyGLWidget();





public slots:
    void receiveRotationZ(int degrees);

signals:
    void zoomFactorChanged(int);


};

#endif // MYGLWIDGET_H
