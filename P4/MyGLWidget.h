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
#include "sonne.h"
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
    GLfloat*     vboData ;
    GLuint*      indexData ;
    unsigned int vboLength ;
    unsigned int iboLength ;
    bool         hasTexureCoord ;
    // Buffer
    QOpenGLBuffer vbo;
    QOpenGLBuffer ibo;
    void initalizeBuffer();
    void fillBuffer();

    // Matritzen
    QMatrix4x4 projectionMatrix ;
    QMatrix4x4 viewMatrix ;
    QMatrix4x4 modelMatrix ;
    std::stack<QMatrix4x4> modelMatrixStack ;

    // Camera - Parameter
    QVector3D cameraPos   = QVector3D(0.0f, 0.0f, 3.0f);
    QVector3D cameraFront = QVector3D(0.0f, 0.0f, -1.0f);
    QVector3D cameraUp    = QVector3D(0.0f, 1.0f, 0.0f);
    GLfloat   yaw         = -90.0f;	// Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
    GLfloat   pitch       =   0.0f;

    // Shader
    QOpenGLShaderProgram shaderProgram[2];
    void initalizeShader();
    void initializeShaderProgramPlanet();
    void initializeShaderProgramSun();

    // Lokalisiere bzw. definiere die Schnittstelle für die Eckpunkte und Texturen, ..
    int attrVerticesPlanet ;
    int attrTexCoordsPlanet ;
    int attrNormalsPlanet ;
    int unifMatrixPerspectivePlanet ;
    int unifMatrixViewPlanet ;
    int unifkaPlanet ; // ambient
    int unifkdPlanet ; // diffuse
    int unifksPlanet ; // specular
    int unifNormalMatrixPlanet ;
    int unifLightPositionPlanet ;
    int unifViewPositionPlanet ;

    int attrVerticesSun ;
    int attrTexCoordsSun ;
    int attrNormalsSun ;
    int unifMatrixPerspectiveSun ;
    int unifMatrixViewSun ;
    int unifTimerSun ;

    // Beleuchtung
    QVector3D ka = QVector3D(0.2,0.2,0.2);  // Ambient
    QVector3D kd = QVector3D(0.5,0.5,0.5);  // Diffuse
    QVector3D ks = QVector3D(0.5,0.5,0.5);  // Specular


    // Textures
    QOpenGLTexture *textures[13];
    void initializeTextures();

    // Modell
    void loadModel();


    // Planeten
    Planet merkur, venus, erde, erdemond, mars, phobos, deimos, jupiter, saturn, uranus, neptun;
    Sonne sonne ;
    void initializePlanets();


    // Zeitmessung
    QElapsedTimer tmrRender ;               // Misst vergangene Zeit zwischen Rendern ;

    int   unifMatrixModelDefault  = 0 ;     // Uniform Value für Model Matrix. Hier deklariert, um Referenz an die Planten Klasse zu übergeben.
    int   unifMatrixModelNormalen = 0 ;
    int   elapsedTime             = 0 ;     // Zeit zwischen dem Rendern. Referenz wird an die Planeten übergeben, damit die damit arbeiten können.
    int   counter                 = 0 ;
    float time                    = 0.0;


    // Paused
    bool paused = false ;

protected:

    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();


    // Events
    void keyPressEvent(QKeyEvent *event);


public:
    MyGLWidget();
    MyGLWidget(QWidget *parent); // Konstruktur noch angeben?
    ~MyGLWidget();


};

#endif // MYGLWIDGET_H
