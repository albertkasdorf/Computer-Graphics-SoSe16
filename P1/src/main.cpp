/* Computergrafik Praktikum
 * Klassisches OpenGL mit freeGLUT
 * FH Aachen, 2016
 */

#include <GL/glut.h>
#include <GL/freeglut.h>

#define WINDOW_CAPTION "Computergrafik Praktikum"
#define WINDOW_XPOS 50
#define WINDOW_YPOS 50
#define WINDOW_WIDTH 480
#define WINDOW_HEIGHT 480
#define UPDATE_RATE 60


// global variable for engine purposes
// not pretty and hardware-dependent, but gets the job done
unsigned int counter = 0;


// OpenGL settings initialization
void init()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//glEnable( GL_CULL_FACE );

	glShadeModel( GL_SMOOTH );
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glClearDepth(1.0f);

	// Hintergrundfarbe ist Schwarz.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}


// Handler for timer event
void update(int v)
{
	// Schedule a draw event
	glutPostRedisplay();

	// Needs to be registered again
	glutTimerFunc((unsigned int) 1000.0f / UPDATE_RATE, update, v);
}


// Handler for window resize and creation event
void reshape(GLsizei width, GLsizei height)
{
	const bool perspective = true;

	// Compute aspect ratio
	height = (height == 0) ? 1 : height;
	GLfloat aspect = (GLfloat)width / (GLfloat)height;

	// Set viewport to cover the whole window
	glViewport(0, 0, width, height);

	// Set projection matrix to ...
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	if( perspective )
	{
		// a perspective projection
		gluPerspective(45.0f, aspect, 0.1, 100.0);
	}
	else
	{
		// a orthographic projection
		glOrtho( -3.f, 3.f, -3.f, 3.f, -10.f, 10.f );
	}
}


// Handler for window draw event
void draw()
{
	// Clear buffer to set color and alpha
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Apply model view transformations
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -7.0f);

	glRotatef( static_cast<float>( counter ), 0.f, 1.f, 0.f );

	// 1.5 Drehen Sie das Dreieck mit glRotate 45° um die Z-Achse gegen den Uhrzeigersinn.
	// Erst Transformation dann Rotation.
	glRotatef( -45.f, 0.f, 0.f, 1.f );

	// Draw shape
	glBegin(GL_TRIANGLES);				// gegen den Uhrzeigersinn
		glColor3f( 1.f, 0.f, 0.f );
		glVertex3f( +1.f, -1.f, 0.f);	// unten rechts
		glColor3f( 0.f, 1.f, 0.f );
		glVertex3f( +1.f, +1.f, 0.f);	// oben rechts
		glColor3f( 0.f, 0.f, 1.f );
		glVertex3f( -1.f, +1.f, 0.f);	// oben links

		glColor3f( 1.f, 0.f, 0.f );
		glVertex3f( +1.f, -1.f, 0.f );	// unten rechts
		glColor3f( 0.f, 0.f, 1.f );
		glVertex3f( -1.f, +1.f, 0.f );	// oben links
		glColor3f( 0.f, 1.f, 1.f );
		glVertex3f( -1.f, -1.f, 0.f );	// unten links
	glEnd();

	// Execute all issued GL commands
	glFlush(); // replace with glutSwapBuffers() for double buffered mode

	// Increment counter
	counter++;
}

void keyboard( unsigned char key, int x, int y )
{
	switch( key )
	{
	case 27:	// Escape-Key
		glutLeaveMainLoop();
		break;
	}
}


// main function
int main(int argc, char **argv)
{
	// initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE); // GLUT_DOUBLE for double buffered mode

	// Create output window
	glutInitWindowPosition(WINDOW_XPOS, WINDOW_YPOS);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow(WINDOW_CAPTION);

	// Register callback handlers for GLUT events
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutDisplayFunc(draw); // Note: not called continuously
	glutTimerFunc((unsigned int) 1000.0f / UPDATE_RATE, update, 0); // Note: inaccurate, but good enough here

	// Initialize OpenGL parameters
	init();

	// Jump into the main drawing loop
	glutMainLoop();

	return 0;
}
