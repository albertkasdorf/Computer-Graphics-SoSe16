#include "myglwidget.h"

#include <QInputEvent>

MyGLWidget::MyGLWidget(QWidget*parent)
	: QGLWidget(parent)
{
	zRotation = 0;
	xCameraTranslation = 0;
	yCameraTranslation = 0;
	zCameraTranslation = -7;

	setFocusPolicy( Qt::StrongFocus );
}

void MyGLWidget::initializeGL()
{
	glEnable( GL_CULL_FACE );

	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );

	glShadeModel( GL_SMOOTH );
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	glClearDepth( 1.0f );

	// Hintergrundfarbe ist Schwarz.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void MyGLWidget::resizeGL(int w, int h)
{
	// Set viewport to cover the whole window
	glViewport(0, 0, w, h);

	// Set projection matrix to ...
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	glFrustum(-0.05, 0.05, -0.05, 0.05, 0.1, 100.0);
}

void MyGLWidget::paintGL()
{
	// Clear buffer to set color and alpha
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Apply model view transformations
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef( xCameraTranslation, yCameraTranslation, zCameraTranslation);

	glRotatef( zRotation, 1.f, 1.f, 1.f );

	glBegin( GL_QUADS );
		// Vorne
		glColor3f( 1.f, 0.f, 0.f );		// Red
		glVertex3f( -1.f, +1.f, +1.f);
		glVertex3f( -1.f, -1.f, +1.f);
		glVertex3f( +1.f, -1.f, +1.f);
		glVertex3f( +1.f, +1.f, +1.f);

		// Hinten
		glColor3f( 0.f, 1.f, 0.f );		// Green
		glVertex3f( +1.f, +1.f, -1.f );
		glVertex3f( +1.f, -1.f, -1.f );
		glVertex3f( -1.f, -1.f, -1.f );
		glVertex3f( -1.f, +1.f, -1.f );

		// Links
		glColor3f( 0.f, 0.f, 1.f );		// Blue
		glVertex3f( -1.f, +1.f, +1.f );
		glVertex3f( -1.f, +1.f, -1.f );
		glVertex3f( -1.f, -1.f, -1.f );
		glVertex3f( -1.f, -1.f, +1.f );

		// Rechts
		glColor3f( 1.f, 1.f, 0.f );		// Yellow
		glVertex3f( +1.f, +1.f, +1.f );
		glVertex3f( +1.f, -1.f, +1.f );
		glVertex3f( +1.f, -1.f, -1.f );
		glVertex3f( +1.f, +1.f, -1.f );

		// Oben
		glColor3f( 0.f, 1.f, 1.f );		// Cyan
		glVertex3f( -1.f, +1.f, +1.f );
		glVertex3f( +1.f, +1.f, +1.f );
		glVertex3f( +1.f, +1.f, -1.f );
		glVertex3f( -1.f, +1.f, -1.f );

		// Unten
		glColor3f( 1.f, 0.f, 1.f );		// Magenta
		glVertex3f( +1.f, -1.f, +1.f );
		glVertex3f( -1.f, -1.f, +1.f );
		glVertex3f( -1.f, -1.f, -1.f );
		glVertex3f( +1.f, -1.f, -1.f );

	glEnd( );
}

void MyGLWidget::receiveRotationZ(int degree)
{
	zRotation = static_cast<float>( degree );
	glDraw();
}

void MyGLWidget::keyPressEvent(QKeyEvent * event)
{
	switch( event->key() )
	{
	case Qt::Key_W:
		event->accept();
		yCameraTranslation++;
		glDraw();
		break;

	case Qt::Key_S:
		event->accept();
		yCameraTranslation--;
		glDraw();
		break;

	case Qt::Key_A:
		event->accept();
		xCameraTranslation--;
		glDraw();
		break;

	case Qt::Key_D:
		event->accept();
		xCameraTranslation++;
		glDraw();
		break;

	default:
		QGLWidget::keyPressEvent( event );
	}
}

void MyGLWidget::wheelEvent(QWheelEvent * event)
{
	event->accept( );
	if( event->delta( ) > 0 )
	{
		// A positive value indicates that the wheel was rotated
		// forwards away from the user
		zCameraTranslation--;
	}
	else if( event->delta() < 0 )
	{
		// A negative value indicates that the wheel was rotated
		// backwards toward the user.
		zCameraTranslation++;
	}
	emit sendCameraTranslationZ(-zCameraTranslation);
	glDraw();
}
