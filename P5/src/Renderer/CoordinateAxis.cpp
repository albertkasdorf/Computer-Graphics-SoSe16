#include "CoordinateAxis.h"
#include <memory>
#include "../GL/Buffer.h"
#include "../GL/ShaderProgram.h"
#include "../GL/VertexArray.h"

namespace Renderer
{

CoordinateAxis::CoordinateAxis( void )
	:
	vbo( std::make_unique<GL::Buffer>( ) ),
	vao( std::make_unique<GL::VertexArray>( ) ),
	count( 0 ),
	axisLenght( 25.0 )
{
}

CoordinateAxis::~CoordinateAxis( void )
{
}

void CoordinateAxis::Initialize( void )
{
	InitializeGeometry( );
	InitializeShader( );
}

void CoordinateAxis::Render( glm::mat4 & projection, glm::mat4 & view, glm::mat4 & model )
{
	GLfloat lineWidth = 1.0;
	GLint depthTest = GL_FALSE;
	
	shader->ProgramUse( );
	shader->ProjectionMatrixSet( projection );
	shader->ViewMatrixSet( view );
	shader->ModelMatrixSet( model );

	glGetFloatv( GL_LINE_WIDTH, &lineWidth );
	glLineWidth( 2.0 );

	glGetIntegerv( GL_DEPTH_TEST, &depthTest );
	glDisable( GL_DEPTH_TEST );

	glBindVertexArray( vao->IdGet( ) );
	{
		glDrawArrays( GL_LINES, 0, count );
	}
	glBindVertexArray( 0 );

	if( depthTest == GL_FALSE )
		glDisable( GL_DEPTH_TEST );
	else
		glEnable( GL_DEPTH_TEST );

	glLineWidth( lineWidth );

	shader->ProgramUse( false );
}

void CoordinateAxis::InitializeGeometry( void )
{
	GLsizeiptr size = 0;
	GLvoid * data = nullptr;
	GLsizei stride = 0;
	GLvoid * pointer = nullptr;

	std::vector<glm::vec3> geometry{
		glm::vec3( 0.0, 0.0, 0.0 ),
		glm::vec3( 1.0, 0.0, 0.0 ),
		glm::vec3( axisLenght, 0.0, 0.0 ),
		glm::vec3( 1.0, 0.0, 0.0 ),

		glm::vec3( 0.0, 0.0, 0.0 ),
		glm::vec3( 0.0, 1.0, 0.0 ),
		glm::vec3( 0.0, axisLenght, 0.0 ),
		glm::vec3( 0.0, 1.0, 0.0 ),

		glm::vec3( 0.0, 0.0, 0.0 ),
		glm::vec3( 0.0, 0.0, 1.0 ),
		glm::vec3( 0.0, 0.0, axisLenght ),
		glm::vec3( 0.0, 0.0, 1.0 ),
	};

	count = static_cast<GLsizei>( geometry.size( ) );
	size = sizeof( glm::vec3 ) * geometry.size( );
	data = &geometry[0];

	glBindBuffer( GL_ARRAY_BUFFER, vbo->IdGet( ) );
	glBufferData( GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW );

	glBindVertexArray( vao->IdGet( ) );
	{
		glBindBuffer( GL_ARRAY_BUFFER, vbo->IdGet( ) );

		glEnableVertexAttribArray( 0 );
		glEnableVertexAttribArray( 1 );

		stride = sizeof( glm::vec3 ) * 2;
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, stride, pointer );

		pointer = reinterpret_cast<GLvoid*>( sizeof( glm::vec3 ) );
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, stride, pointer );
	}
	glBindVertexArray( 0 );
}

void CoordinateAxis::InitializeShader( void )
{
	shader = std::make_unique<GL::ShaderProgram>(
		"./Shader/CoordinateAxis.vert",
		"./Shader/CoordinateAxis.frag" );
	shader->CompileAndLink( );
}

}
