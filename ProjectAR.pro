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
    modelloader.cpp

HEADERS  += mainwindow.h \
    MyGLWidget.h \
    modelloader.h

FORMS    += mainwindow.ui

LIBS += -lopengl32

LIBS += -LC:/assimpsdk/lib/assimp_debug-dll_win32/ -lassimp
INCLUDEPATH += C:/assimp/include
DEPENDPATH += C:/assimp/include
