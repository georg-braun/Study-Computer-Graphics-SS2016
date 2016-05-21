#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QGLWidget>
#include <QWidget>
#include <QTimer>
//#include "modelloader.h"
#include <QOpenGLTexture>


class MyGLWidget : public QGLWidget
{
private:
    Q_OBJECT    // declare own signals and slots (MOC-System)
protected:

    QTimer *timer = new QTimer(this) ;

    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();


public:
    MyGLWidget();
    MyGLWidget(QWidget *parent); // Konstruktur noch angeben?
};

#endif // MYGLWIDGET_H
