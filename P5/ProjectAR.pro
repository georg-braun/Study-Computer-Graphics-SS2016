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
    MyGLWidget.cpp \
    markergenerator.cpp \
    calibrator.cpp \
    detector.cpp \
    ardata.cpp \
    modelloader.cpp

HEADERS  += mainwindow.h \
    MyGLWidget.h \
    markergenerator.h \
    calibrator.h \
    detector.h \
    ardata.h \
    modelloader.h

FORMS    += mainwindow.ui

LIBS += -lopengl32

LIBS += -LC:/assimp/code -lassimp
INCLUDEPATH += c:/assimp/include
DEPENDPATH += c:/assimp/include


INCLUDEPATH += C:/opencvbin/install/include
LIBS += -LC:\\opencvbin\\bin \
    libopencv_aruco310 \
    libopencv_calib3d310 \
    libopencv_core310 \
    libopencv_features2d310 \
    libopencv_flann310 \
    libopencv_highgui310 \
    libopencv_imgcodecs310 \
    libopencv_imgproc310 \
    libopencv_ml310 \
    libopencv_objdetect310 \
    libopencv_photo310 \
    libopencv_shape310 \
    libopencv_stitching310 \
    libopencv_superres310 \
    libopencv_video310 \
    libopencv_videoio310 \
    libopencv_videostab310

RESOURCES += \
    shader.qrc \
    textures.qrc


