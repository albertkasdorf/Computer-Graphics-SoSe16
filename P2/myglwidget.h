#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QGLWidget>

class MyGLWidget : public QGLWidget
{
	Q_OBJECT

public:
	MyGLWidget(QWidget*parent=nullptr);

protected:
	void initializeGL();
	void resizeGL(int, int);
	void paintGL();

	void keyPressEvent(QKeyEvent * event);
	void wheelEvent(QWheelEvent * event);

private:
	float zRotation;
	float xCameraTranslation;
	float yCameraTranslation;
	float zCameraTranslation;

public slots:
	void receiveRotationZ(int);

signals:
	void sendCameraTranslationZ(int);
};

#endif // MYGLWIDGET_H
