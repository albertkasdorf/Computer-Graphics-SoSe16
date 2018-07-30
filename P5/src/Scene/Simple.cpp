
#include "Simple.h"
#include "../Camera.h"
#include "../ElapsedTime.h"
#include "../GL/Buffer.h"
#include "../GL/ShaderProgram.h"
#include "../GL/VertexArray.h"

namespace Scene
{

Simple::Simple( void ) :
	vbo( std::make_unique<GL::Buffer>( ) ),
	ebo( std::make_unique<GL::Buffer>( ) ),
	vao( std::make_unique<GL::VertexArray>( ) )
{
}

Simple::~Simple( void )
{
}

void Simple::Initialize( const GLint width, const GLint height )
{
	glDisable( GL_CULL_FACE );

	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );

	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	// Activate Wireframe Mode
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	//glClearDepth( 1.0 );

	InitializeGeometry( );
	InitializeShader( );

	elapsedTime = std::make_unique<ElapsedTime>( );

	camera = std::make_unique<Camera>( );
	camera->PositionSet( 0.0, 0.0, 30.0 );
	camera->ViewDirectionSet( 0.0, 0.0, -1.0 );

	//----
	FramebufferSizeCallback( width, height );
}

void Simple::InitializeGeometry( void )
{
	GLfloat vertices[] = {
		+5.0, +5.0, 0.0f,  // Top Right
		+5.0, -5.0, 0.0f,  // Bottom Right
		-5.0, -5.0, 0.0f,  // Bottom Left
		-5.0, +5.0, 0.0f   // Top Left 
	};
	GLuint indices[] = {  // Note that we start from 0!
		0, 1, 3,  // First Triangle
		1, 2, 3   // Second Triangle
	};

	glBindVertexArray( vao->IdGet( ) );

	glBindBuffer( GL_ARRAY_BUFFER, vbo->IdGet( ) );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo->IdGet( ) );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( indices ), indices, GL_STATIC_DRAW );

	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), (GLvoid*)0 );

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	
	glBindVertexArray( 0 );
}

void Simple::InitializeShader( void )
{
	shader = std::make_unique<GL::ShaderProgram>(
		"./Shader/Default.vert", "./Shader/Default.frag" );
		//"./Shader/Simple.vertex", "./Shader/Simple.fragment", false );
	shader->CompileAndLink( );
}

void Simple::Terminate( void )
{

}

void Simple::Render( void )
{
	elapsedTime->Update( );
	camera->Update( elapsedTime.get( ) );

	glClearColor( 0.0, 0.0, 0.0, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	shader->ProgramUse( );
	{
		shader->ProjectionMatrixSet( camera->ViewToProjectionMatrixGet( ) );
		shader->ViewMatrixSet( camera->WorldToViewMatrixGet( ) );
		shader->ModelMatrixSet( glm::mat4( 1.0 ) );

		glBindVertexArray( vao->IdGet( ) );
			//glDrawArrays( GL_TRIANGLES, 0, 6 );
			glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );
		glBindVertexArray( 0 );
	}
	shader->ProgramUse( false );
}

void Simple::FramebufferSizeCallback( const GLint width, const GLint height )
{
	camera->FramebufferSizeCallback( width, height );
}

void Simple::KeyCallback( const int key, const int scancode, const int action, const int mods )
{
	camera->KeyCallback( key, scancode, action, mods );
}

void Simple::CursorPosCallback( const double xpos, const double ypos )
{
	camera->CursorPosCallback( xpos, ypos );
}

void Simple::ScrollCallback( const double xoffset, const double yoffset )
{
}

void Simple::MouseButtonCallback( const int button, const int action, const int mods )
{
	camera->MouseButtonCallback( button, action, mods );
}

}
