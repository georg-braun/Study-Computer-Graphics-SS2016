#include "MyGLWidget.h"
#include <iostream>



MyGLWidget::MyGLWidget()
{

}



MyGLWidget::MyGLWidget(QWidget *parent):QGLWidget(parent)
{

     std::cout << glGetString (GL_VERSION ) ;
     setFocusPolicy(Qt::StrongFocus);
}



void MyGLWidget::initializeGL()
{



    glEnable(GL_DEPTH_TEST);  // Activate depth comparisons and update depth buffer

    glEnable(GL_CULL_FACE);   // Draw Front or Back?

    glDepthFunc(GL_LEQUAL);   // Specify the depth buffer
    glShadeModel(GL_SMOOTH);  // GL_FLAT or GL_SMOOTH (interpolated)

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Are there Interpretations? Set hint!

    glClearDepth(1.0f);  // Clear value for depth buffer (used by glClear)

    // P1.3 - Black Background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Clear values used by glClear (Color-Buffer)

}


void MyGLWidget::resizeGL(int width, int height)
{
   // Compute aspect ratio
   height = (height == 0) ? 1 : height;  // ?

   // Set viewport to cover the whole window
   glViewport(0, 0, width, height);

   // Set projection matrix to a perspective projection
   glMatrixMode(GL_PROJECTION);  // Use projection matrix
   glLoadIdentity(); // Einheitsmatrix laden

   glFrustum(-0.05, 0.05, -0.05, 0.05, 0.1, 100.0);

}


void MyGLWidget::paintGL()
{

    // Test
    glClear(GL_COLOR_BUFFER_BIT);


    // Clear buffer to set color and alpha
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    glMatrixMode(GL_PROJECTION);  // Which matrix is active?


    // Apply model view transformations
    glMatrixMode(GL_MODELVIEW);  // Which matrix is active?
    glLoadIdentity(); // Einheitsmatrix laden

    // Transformation
    glTranslatef(0.0f, 0.0f, -7.0f); // Initial
    //glTranslatef(0.0f, 0.0f, -zoom); // Zoom
    glTranslatef(moveX, moveY, -zoom); // Zoom

    //glRotatef(-45, 0, 0, 1);
    glRotatef((zRotation), 0, 1, 0);



    // Set color for drawing
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

    // Würfel - Rechte Hand Regel: Daumen zeigt nach außen.
    glBegin(GL_QUADS) ;

        // Back
        glColor3f(1,0,0);
        glVertex3f( -1.0f, -1.0f, -1.0f);
        glVertex3f( -1.0f,  1.0f, -1.0f);
        glVertex3f(  1.0f,  1.0f, -1.0f);
        glVertex3f(  1.0f, -1.0f, -1.0f);
        // Front
        glColor3f(0,0,1);
        glVertex3f(  1.0f, -1.0f, 1.0f);
        glVertex3f(  1.0f,  1.0f, 1.0f);
        glVertex3f( -1.0f,  1.0f, 1.0f);
        glVertex3f( -1.0f, -1.0f, 1.0f);

        // Right
        glColor3f(0,1,0);
        glVertex3f(  1.0f,  1.0f,  1.0f);
        glVertex3f(  1.0f, -1.0f,  1.0f);
        glVertex3f(  1.0f, -1.0f, -1.0f);
        glVertex3f(  1.0f,  1.0f, -1.0f);
        // Left
        glColor3f(0,1,0);
        glVertex3f( -1.0f, -1.0f,  1.0f);
        glVertex3f( -1.0f,  1.0f,  1.0f);
        glVertex3f( -1.0f,  1.0f, -1.0f);
        glVertex3f( -1.0f, -1.0f, -1.0f);

        // Top
        glColor3f(1,1,0);
        glVertex3f(  1.0f,  1.0f, -1.0f);
        glVertex3f( -1.0f,  1.0f, -1.0f);
        glVertex3f( -1.0f,  1.0f,  1.0f);
        glVertex3f(  1.0f,  1.0f,  1.0f);

        // Bottom
        glColor3f(1,1,0);
        glVertex3f( -1.0f,  -1.0f,  1.0f);
        glVertex3f( -1.0f,  -1.0f, -1.0f);
        glVertex3f(  1.0f,  -1.0f, -1.0f);
        glVertex3f(  1.0f,  -1.0f,  1.0f);


    glEnd() ;


    /*
    // Quadrat
    glBegin(GL_TRIANGLES);
       // 1. Triangle
       glColor3f(1,0,0);
       glVertex3f( 1.0f,  -1.0f,  0.0f);
       glColor3f(0,1,0);
       glVertex3f( 1.0f,  1.0f,  0.0f);
       glColor3f(0,0,1);
       glVertex3f(-1.0f,  1.0f,  0.0f);
       // 2. Triangle
       glColor3f(1,0,0);
       glVertex3f( 1.0f,  -1.0f,  0.0f);
       glColor3f(0,0,1);
       glVertex3f( -1.0f,  1.0f,  0.0f);
       glColor3f(1,1,0);
       glVertex3f(-1.0f,  -1.0f,  0.0f);
    glEnd();
    */


    // Execute all issued GL commands
    //glFlush(); // replace with glutSwapBuffers() for double buffered mode

}

void MyGLWidget::wheelEvent ( QWheelEvent * event )
{
    zoom += event->delta() / 120 ;
    emit zoomFactorChanged(zoom);
    glDraw() ;
}


void MyGLWidget::keyPressEvent(QKeyEvent *event)
{

    switch ( event->key()) {
        case Qt::Key_W : moveY += 0.1 ;
             break ;
        case Qt:: Key_S : moveY -= 0.1 ;
             break ;
        case Qt::Key_A : moveX -= 0.1 ;
             break ;
        case Qt::Key_D : moveX += 0.1 ;
             break ;
        default : QGLWidget::keyPressEvent(event) ;
    }
/*
    if (event->key() == Qt::Key_W) {
        moveY += 0.01 ;
    }
    else if (event->key() == Qt::Key_S) {
        moveY -= 0.01 ;
    }
    else if (event->key() == Qt::Key_A) {
        moveX -= 0.01 ;
    }
    else if (event->key() == Qt::Key_D) {
        moveX += 0.01 ;
    }
    else {
        QGLWidget::keyPressEvent(event) ;
    }
    */
    glDraw() ;
}


void MyGLWidget::receiveRotationZ( int degrees )
{
    zRotation = degrees ;
    glDraw();
}

