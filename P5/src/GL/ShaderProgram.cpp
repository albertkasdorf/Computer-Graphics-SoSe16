#include "ShaderProgram.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include "../IO/File.h"

namespace GL
{

ShaderProgram::ShaderProgram( std::string vertexShaderPath, std::string fragmentShaderPath, bool useAssert ) :
	vertexShaderPath( vertexShaderPath ),
	fragmentShaderPath( fragmentShaderPath ),
	useAssert( useAssert ),
	program( 0 )
{
}

ShaderProgram::~ShaderProgram( )
{
	glDeleteProgram( program );
}

bool ShaderProgram::CompileAndLink( void )
{
	std::string vertexShaderText, fragmentShaderText;
	GLuint vertexShader, fragmentShader;
	GLint status;

	// Cleanup if already exist.
	glDeleteProgram( program );

	// Übergebene Textdateien einlesen und prüfen ob diese nicht leer sind.
	vertexShaderText = IO::File::ReadAllText( vertexShaderPath );
	if( useAssert )
		assert( !vertexShaderText.empty( ) );

	fragmentShaderText = IO::File::ReadAllText( fragmentShaderPath );
	if( useAssert )
		assert( !fragmentShaderText.empty( ) );

	// Compile both shader
	vertexShader = ShaderCreate( GL_VERTEX_SHADER, vertexShaderText );
	fragmentShader = ShaderCreate( GL_FRAGMENT_SHADER, fragmentShaderText );

	// Link shaders
	program = glCreateProgram( );
	glAttachShader( program, vertexShader );
	glAttachShader( program, fragmentShader );
	glLinkProgram( program );

	// Check for linking errors
	glGetProgramiv( program, GL_LINK_STATUS, &status );
	if( !status )
	{
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << ProgramInfoLogGet( program ) << std::endl;
		if( useAssert )
			assert( false );
	}
	glDeleteShader( vertexShader );
	glDeleteShader( fragmentShader );

	if( status == GL_TRUE )
	{
		locModel = this->UniformLocationGet( "model" );
		locView = this->UniformLocationGet( "view" );
		locProjection = this->UniformLocationGet( "projection" );
	}

	return true;
}

GLuint ShaderProgram::ShaderCreate( GLenum shaderType, std::string shaderText )
{
	GLuint shaderId = 0;
	const GLchar* shaderArray[1] = { shaderText.c_str( ) };
	GLint shaderLenght[1] = { static_cast<GLint>(shaderText.size( )) };
	GLint success;

	shaderId = glCreateShader( shaderType );
	glShaderSource( shaderId, 1, &shaderArray[0], &shaderLenght[0] );
	glCompileShader( shaderId );

	glGetShaderiv( shaderId, GL_COMPILE_STATUS, &success );
	if( !success )
	{
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << ShaderInfoLogGet( shaderId ) << std::endl;
		if( useAssert )
			assert( false );
	}

	return shaderId;
}

std::string ShaderProgram::ShaderInfoLogGet( const GLuint shaderId )
{
	GLint length;
	std::vector<GLchar> buffer;
	std::string log;

	glGetShaderiv( shaderId, GL_INFO_LOG_LENGTH, &length );
	if( useAssert )
		assert( length > 0 );
	buffer.resize( length );

	glGetShaderInfoLog( shaderId, length - 1, &length, &buffer[0] );
	if( useAssert )
		assert( length > 0 );
	log = std::string( buffer.data( ) );

	return log;
}

std::string ShaderProgram::ProgramInfoLogGet( const GLuint programId )
{
	GLint length;
	std::vector<GLchar> buffer;
	std::string log;

	glGetProgramiv( programId, GL_INFO_LOG_LENGTH, &length );
	if( useAssert )
		assert( length > 0 );
	buffer.resize( length );

	glGetProgramInfoLog( program, length - 1, &length, &buffer[0] );
	if( useAssert )
		assert( length > 0 );
	log = std::string( buffer.data( ) );

	return log;
}

GLint ShaderProgram::AttribLocationGet( const std::string name )
{
	GLint location = -1;

	location = glGetAttribLocation( program, name.c_str( ) );
	if( useAssert )
		assert( location != -1 );

	return location;
}

GLint ShaderProgram::UniformLocationGet( const std::string name )
{
	GLint location = -1;

	if( uniformLocationLookUp.find( name ) != uniformLocationLookUp.end( ) )
	{
		location = uniformLocationLookUp[name];
	}
	else
	{
		location = glGetUniformLocation( program, name.c_str( ) );
		if( useAssert )
			assert( location != -1 );

		uniformLocationLookUp[name] = location;
	}

	return location;
}

GLuint ShaderProgram::ProgramGet( void ) const
{
	return program;
}

// Deprecated
void ShaderProgram::ProgramUse( bool enable )
{
	if( enable )
	{
		GLint param;
		glGetIntegerv( GL_CURRENT_PROGRAM, &param );
		previousProgram = param;

		glUseProgram( program );
	}
	else
	{
		glUseProgram( previousProgram );
	}
}

void ShaderProgram::ProgramEnable( void )
{
	GLint param;
	glGetIntegerv( GL_CURRENT_PROGRAM, &param );
	previousProgram = param;

	glUseProgram( program );
}

void ShaderProgram::ProgramDisable( void )
{
	glUseProgram( previousProgram );
}

void ShaderProgram::ModelMatrixSet( const glm::mat4 & model )
{
	glUniformMatrix4fv( locModel, 1, GL_FALSE, glm::value_ptr( model ) );
}

void ShaderProgram::ViewMatrixSet( const glm::mat4 & view )
{
	glUniformMatrix4fv( locView, 1, GL_FALSE, glm::value_ptr( view ) );
}

void ShaderProgram::ProjectionMatrixSet( const glm::mat4 & projection )
{
	glUniformMatrix4fv( locProjection, 1, GL_FALSE, glm::value_ptr( projection ) );
}

void ShaderProgram::UniformSet( const std::string name, GLfloat value )
{
	GLint location = UniformLocationGet( name );
	glUniform1f( location, value );
}

void ShaderProgram::UniformSet( const std::string name, GLboolean value )
{
	GLint location = UniformLocationGet( name );
	glUniform1i( location, value );
}

void ShaderProgram::UniformSet( const std::string name, GLint value )
{
	GLint location = UniformLocationGet( name );
	glUniform1i( location, value );
}

void ShaderProgram::UniformSet( const std::string name, glm::vec3 & value )
{
	GLint location = UniformLocationGet( name );
	glUniform3fv( location, 1, glm::value_ptr( value ) );
}

void ShaderProgram::UniformSet(
	const std::string name, std::vector<float> & value )
{
	GLint location = UniformLocationGet( name );
	glUniform1fv( location, static_cast<GLsizei>(value.size( )), &value[0] );
}

void ShaderProgram::UniformMatrixSet(
	const std::string name, std::vector<glm::mat4> & value )
{
	const GLint location = UniformLocationGet( name );
	glUniformMatrix4fv(
		location,
		static_cast<GLsizei>(value.size( )),
		GL_FALSE,
		glm::value_ptr( value[0] ) );
}

void ShaderProgram::UniformSet( const std::string name, GLfloat v0, GLfloat v1, GLfloat v2 )
{
	const GLint location = UniformLocationGet( name );
	glUniform3f( location, v0, v1, v2 );
}

}
