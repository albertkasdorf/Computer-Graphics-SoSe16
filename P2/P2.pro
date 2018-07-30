#-------------------------------------------------
#
# Project created by QtCreator 2016-03-25T12:24:03
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = P2
TEMPLATE = app


SOURCES +=\
        MainWindow.cpp \
    Main.cpp \
    myglwidget.cpp \
    MyOpenGLWidget.cpp

HEADERS  += MainWindow.h \
    myglwidget.h \
    MyOpenGLWidget.h

FORMS    += MainWindow.ui

DISTFILES += \
    Beschreibung.txt

#LIBS += -lopengl32
