#-------------------------------------------------
#
# Project created by QtCreator 2016-05-21T10:26:17
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ProjectAR
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    MyGLWidget.cpp

HEADERS  += mainwindow.h \
    MyGLWidget.h

FORMS    += mainwindow.ui

LIBS += -lopengl32


INCLUDEPATH += c:\\OpenCV-3.1.0\\opencv\\build\\include

LIBS +=-Lc:\\OpenCV-3.1.0\\mybuild\\lib\\Debug \
    -lopencv_aruco310d \
    -lopencv_calib3d310d \
    -lopencv_core310d \
    -lopencv_features2d310d \
    -lopencv_flann310d \
    -lopencv_highgui310d \
    -lopencv_imgcodecs310d \
    -lopencv_imgproc310d \
    -lopencv_ml310d \
    -lopencv_objdetect310d \
    -lopencv_photo310d \
    -lopencv_shape310d \
    -lopencv_stitching310d \
    -lopencv_superres310d \
    -lopencv_ts310d \
    -lopencv_video310d \
    -lopencv_videoio310d \
    -lopencv_videostab310d


