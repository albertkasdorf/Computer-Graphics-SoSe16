#ifndef MYOPENGLWIDGET_H
#define MYOPENGLWIDGET_H

#include <QWidget>
#include <qopenglwidget.h>

class MyOpenGLWidget : public QOpenGLWidget
{
public:
	MyOpenGLWidget(QWidget *parent = nullptr);
};

#endif // MYOPENGLWIDGET_H
