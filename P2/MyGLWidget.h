#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QGLWidget>
#include <QWidget>
#include <QInputEvent>
#include <QOpenGLFunctions_3_1>


class MyGLWidget : public QGLWidget
{


private:
    Q_OBJECT    // declare own signals and slots (MOC-System)

    int     zRotation = 0 ;
    GLfloat zoom  = 0 ;
    GLfloat moveY = 0 ;
    GLfloat moveX = 0 ;


public:
    MyGLWidget();
    MyGLWidget(QWidget *parent); // Konstruktur noch angeben?

protected:
    void initializeGL() ;
    void resizeGL(int width, int height) ;
    void paintGL() ;
    void wheelEvent( QWheelEvent * event ) ;
    void keyPressEvent(QKeyEvent *event) ;

// https://www.proggen.org/doku.php?id=frameworks:qt:basic:connections
public slots:
    void receiveRotationZ( int degrees );

signals:
    void zoomFactorChanged( int );


};

#endif // MYGLWIDGET_H
