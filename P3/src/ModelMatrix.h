#pragma once
#include <stack>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/mat4x4.hpp>

class ModelMatrix
{
public:
	ModelMatrix( void );
	~ModelMatrix( void );

private:
	glm::mat4 model;
	std::stack<glm::mat4> modelStack;

public:
	const GLfloat* PtrGet( void ) const;
	glm::mat4 MatrixGet( void ) const;

	void LoadIdentity( void );

	void PushMatrix( void );
	void PopMatrix( void );

	void Translate( GLdouble x, GLdouble y, GLdouble z );
	void Translate( GLfloat x, GLfloat y, GLfloat z );

	void Rotate( GLdouble angle, GLdouble x, GLdouble y, GLdouble z );
	void Rotate( GLfloat angle, GLfloat x, GLfloat y, GLfloat z );

	void Scale( GLdouble x, GLdouble y, GLdouble z );
	void Scale( GLfloat x, GLfloat y, GLfloat z );

	void Scale( GLdouble xyz );
	void Scale( GLfloat xyz );
};

