#include "ModelMatrix.h"
#include <glm/gtc/type_ptr.hpp>

ModelMatrix::ModelMatrix( void )
{
	LoadIdentity( );
}

ModelMatrix::~ModelMatrix( void )
{
}

const GLfloat* ModelMatrix::PtrGet( void ) const
{
	return glm::value_ptr( model );
}

glm::mat4 ModelMatrix::MatrixGet( void ) const
{
	return model;
}

void ModelMatrix::LoadIdentity( void )
{
	model = glm::mat4( 1.f );
}

void ModelMatrix::PushMatrix( void )
{
	modelStack.push( model );
}

void ModelMatrix::PopMatrix( void )
{
	model = modelStack.top( );
	modelStack.pop( );
}

void ModelMatrix::Translate( GLdouble x, GLdouble y, GLdouble z )
{
	model = glm::translate( model, glm::vec3( x, y, z ) );
}

void ModelMatrix::Translate( GLfloat x, GLfloat y, GLfloat z )
{
	model = glm::translate( model, glm::vec3( x, y, z ) );
}

void ModelMatrix::Rotate( GLdouble angle, GLdouble x, GLdouble y, GLdouble z )
{
	model = glm::rotate( model, glm::radians( static_cast<GLfloat>( angle ) ), glm::vec3( x, y, z ) );
}

void ModelMatrix::Rotate( GLfloat angle, GLfloat x, GLfloat y, GLfloat z )
{
	model = glm::rotate( model, glm::radians( angle ), glm::vec3( x, y, z ) );
}

void ModelMatrix::Scale( GLdouble x, GLdouble y, GLdouble z )
{
	model = glm::scale( model, glm::vec3( x, y, z ) );
}

void ModelMatrix::Scale( GLfloat x, GLfloat y, GLfloat z )
{
	model = glm::scale( model, glm::vec3( x, y, z ) );
}

void ModelMatrix::Scale( GLdouble xyz )
{
	model = glm::scale( model, glm::vec3( xyz, xyz, xyz ) );
}

void ModelMatrix::Scale( GLfloat xyz )
{
	model = glm::scale( model, glm::vec3( xyz, xyz, xyz ) );
}
