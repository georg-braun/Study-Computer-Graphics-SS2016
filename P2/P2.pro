#-------------------------------------------------
#
# Project created by QtCreator 2016-04-04T12:05:06
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = P2
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    MyGLWidget.cpp

HEADERS  += mainwindow.h \
    MyGLWidget.h

FORMS    += mainwindow.ui


LIBS += -lopengl32
