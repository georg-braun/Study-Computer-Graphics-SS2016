#-------------------------------------------------
#
# Project created by QtCreator 2016-04-11T14:25:44
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = P3
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    MyGLWidget.cpp \
    modelloader.cpp \
    planet.cpp

HEADERS  += mainwindow.h \
    MyGLWidget.h \
    modelloader.h \
    planet.h \
    enums.h

FORMS    += mainwindow.ui


LIBS += -lopengl32

RESOURCES += \
    shader.qrc \
    textures.qrc

DISTFILES +=

LIBS += -LC:/assimp/code -lassimp
INCLUDEPATH += c:/assimp/include
DEPENDPATH += c:/assimp/include
