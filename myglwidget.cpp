#include "MyGLWidget.h"


//---------------------------------------------------------------------------------------------------------
MyGLWidget::MyGLWidget()
{

}

//---------------------------------------------------------------------------------------------------------

MyGLWidget::MyGLWidget(QWidget *parent)
    : QGLWidget(parent)
    , vbo(QOpenGLBuffer::VertexBuffer)
    , ibo(QOpenGLBuffer::IndexBuffer)
{

    connect(&timer, SIGNAL(timeout()),this,SLOT(updateGL()));
    timer.start(40);
}

//---------------------------------------------------------------------------------------------------------

void MyGLWidget::fetchArData() {

    arDataPtr->mutex.lock();
    cameraMatrix = arDataPtr->cameraMatrix ;
    modelView_matrix = arDataPtr->modelView_matrix ;
    drawAR = arDataPtr->drawAR ;
    tex = arDataPtr->tex ;
    detectorInitialized =  arDataPtr->detectorInitialized ;
    arDataPtr->mutex.unlock();
}

//---------------------------------------------------------------------------------------------------------

void MyGLWidget::initializeGL()
{
    // Enable GL textures
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    //glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    setFixedHeight(480);
    setFixedWidth(640);

    //glEnable(GL_CULL_FACE);   // Draw Front or Back?

    glClearColor(1.0, 0.0, 0.0, 0.0) ;
    glClearDepth(1.0) ;
    glDepthFunc(GL_LESS) ;
    glEnable(GL_DEPTH_TEST) ;
    glShadeModel(GL_SMOOTH) ;

    glEnable(GL_TEXTURE_2D); // Enable texturing
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glDisable(GL_TEXTURE_2D);

    fillBuffer();
    initalizeBuffer();
}


//---------------------------------------------------------------------------------------------------------

void MyGLWidget::resizeGL(int width, int height)
{
       // Set viewport to cover the whole window
       glViewport(0, 0, 640, 480);
}



//---------------------------------------------------------------------------------------------------------

void MyGLWidget::paintGL()
{

    fetchArData();

    if ( detectorInitialized ) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity() ;
        glOrtho(-4, 4, -3, 3, 0.1, 100);

      // -- Verarbeitung des OpenCV-Bildes
        glEnable(GL_TEXTURE_2D); // Enable texturing
        tex = QGLWidget::convertToGLFormat(tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
        glDisable(GL_TEXTURE_2D);

      // -- OpenCV-Bild (Background) zeichnen

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glEnable(GL_TEXTURE_2D);

        glPushMatrix();
        glTranslatef(0.0,0.0,-99) ;

        glBegin(GL_QUADS);
            glTexCoord2f(0,0); glVertex3f(-4, -3, -0.0);
            glTexCoord2f(1,0); glVertex3f( 4, -3, -0.0);
            glTexCoord2f(1,1); glVertex3f( 4,  3, -0.0);
            glTexCoord2f(0,1); glVertex3f(-4,  3, -0.0);
        glEnd();

        glPopMatrix();

        glDisable(GL_TEXTURE_2D);



        // Vllt Fehler wenn nil zugriff!! Deshalb Status Variablen
        if ( drawAR && projectionCalculated ) {

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity() ;
            glLoadMatrixd((double*)persp.data);


            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glLoadMatrixd((double*)modelView_matrix.data);

            glTranslatef(0.005,0.008,0);    // Kleine Korrektur

            // Würfel zeichnen
            vbo.bind();
            ibo.bind();
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_COLOR_ARRAY);
            glVertexPointer(3,GL_FLOAT,sizeof(GLfloat)*8,(char*)NULL+0);
            glColorPointer(3,GL_FLOAT,sizeof(GLfloat)*8,(char*)NULL+sizeof(GLfloat)*4) ;
            glDrawElements(GL_QUADS,24,GL_UNSIGNED_BYTE,0);
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_COLOR_ARRAY);
            vbo.release();
            ibo.release();

            // drawAxis();

            glPopMatrix();

            drawAR = false ;
        }

    } // if detectorInitialized


}

//---------------------------------------------------------------------------------------------------------

void MyGLWidget::drawAxis() {

      // X - Axis red
      glBegin(GL_LINES);
      glColor3f(1.0f, 0.0f, 0.0f);
      glVertex3f(-0.05f, 0.0f, 0.0f);
      glVertex3f(0.05f, 0.0f, 0.0f);
      glEnd();
      // Y - Axis green
      glBegin(GL_LINES);
      glColor3f(0.0f, 1.0f, 0.0f);
      glVertex3f(0.0f, -0.05f, 0.0f);
      glVertex3f(0.0f, 0.05f, 0.0f);
      glEnd();
      // Z - Axis blue
      glBegin(GL_LINES);
      glColor3f(0.0f, 0.0f, 1.0f);
      glVertex3f(0.0f, 0.0f, 0.05f);
      glVertex3f(0.0f, 0.0f, -0.05f);
      glEnd();

}

//---------------------------------------------------------------------------------------------------------

void MyGLWidget::loadProjectionMatrix() {

    fetchArData();  // Get the Data: Camera Matrix
    double nearVal = 0.1;
    double farVal = 100.0 ;

    // Beim Einlesen der cameraMatrix aus dem Config-File gehen Kodierungsinfos verloren.
    // Deshalb hier auf Nummer sicher gehen.
    cv::Mat cameraMatrixConverted ;
    cameraMatrix.convertTo(cameraMatrixConverted,CV_32F);



    if(persp.empty()) {
                persp.create(4,4);
                persp.setTo(0);

                // http://kgeorge.github.io/2014/03/08/calculating-opengl-perspective-matrix-from-opencv-intrinsic-matrix/
                qDebug() << cameraMatrixConverted.at<float>(0,0);
                qDebug() << cameraMatrixConverted.at<float>(1,1);
                qDebug() << cameraMatrixConverted.at<float>(0,2);
                qDebug() << cameraMatrixConverted.at<float>(1,2);

                double fx = cameraMatrixConverted.at<float>(0,0);
                double fy = cameraMatrixConverted.at<float>(1,1);
                double cx = cameraMatrixConverted.at<float>(0,2);
                double cy = cameraMatrixConverted.at<float>(1,2);
                persp(0,0) = fx/cx;
                persp(1,1) = fy/cy;
                persp(2,2) = -(farVal+nearVal)/(farVal-nearVal);
                persp(2,3) = -2.0*farVal*nearVal / (farVal-nearVal);
                persp(3,2) = -1.0;


                persp = persp.t(); //to col-major

                glMatrixMode(GL_PROJECTION);;
                glLoadMatrixd((double*)persp.data);
                projectionCalculated = true ;

            }

}

//---------------------------------------------------------------------------------------------------------

void MyGLWidget::addVertice(int     verticeNo ,
                            GLfloat x ,
                            GLfloat y ,
                            GLfloat z ,
                            GLfloat r ,
                            GLfloat g ,
                            GLfloat b )
{
    int arrayPos = verticeNo*(2*tupelSize) ;
    vertices[arrayPos  ] = x ;
    vertices[arrayPos+1] = y ;
    vertices[arrayPos+2] = z ;
    vertices[arrayPos+3] = 1 ;
    vertices[arrayPos+4] = r ;
    vertices[arrayPos+5] = g ;
    vertices[arrayPos+6] = b ;
    vertices[arrayPos+7] = 1 ;
}

//---------------------------------------------------------------------------------------------------------

void MyGLWidget::initalizeBuffer()
{
    // Erzeuge vbo
    vbo.create();
    vbo.bind();
    vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    vbo.allocate(vertices,sizeof(GLfloat) * (2*tupelSize) * verticesCount );
    vbo.release();
    // Erzeuge Index-Buffer
    ibo.create();
    ibo.bind();
    ibo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    ibo.allocate(indicies,sizeof(GLubyte) * 4 * 6);
    //ibo.allocate(indicies,sizeof(GLubyte) * 4 );
    ibo.release();
}

//---------------------------------------------------------------------------------------------------------

void MyGLWidget::fillBuffer()
{
    // Vertices
    // Back
    addVertice(0 , -0.01f , -0.01f , -0.01f , 1 , 0 , 0 );
    addVertice(1 ,  0.01f , -0.01f , -0.01f , 0 , 1 , 0 );
    addVertice(2 ,  0.01f ,  0.01f , -0.01f , 1 , 0 , 1 );
    addVertice(3 , -0.01f ,  0.01f , -0.01f , 0 , 0 , 1 );
    // Front
    addVertice(4 , -0.01f , -0.01f ,  0.01f , 1 , 0 , 1 );
    addVertice(5 ,  0.01f , -0.01f ,  0.01f , 1 , 1 , 0 );
    addVertice(6 ,  0.01f ,  0.01f ,  0.01f , 0 , 1 , 1 );
    addVertice(7 , -0.01f ,  0.01f ,  0.01f , 1 , 0 , 0 );

    // Indicies - Cube
    // Front
    indicies[0] = 4 ;
    indicies[1] = 5 ;
    indicies[2] = 6 ;
    indicies[3] = 7 ;

    // Back
    indicies[4] = 3 ;
    indicies[5] = 2 ;
    indicies[6] = 1 ;
    indicies[7] = 0 ;
    // Left
    indicies[8] = 7 ;
    indicies[9] = 3 ;
    indicies[10] = 0 ;
    indicies[11] = 4 ;
    // Right
    indicies[12] = 6 ;
    indicies[13] = 5 ;
    indicies[14] = 1 ;
    indicies[15] = 2 ;
    // Top
    indicies[16] = 6 ;
    indicies[17] = 2 ;
    indicies[18] = 3 ;
    indicies[19] = 7 ;
    // Bottom
    indicies[20] = 4 ;
    indicies[21] = 0 ;
    indicies[22] = 1 ;
    indicies[23] = 5 ;

}
