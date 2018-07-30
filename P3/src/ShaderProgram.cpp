#include "ShaderProgram.h"
#include "File.h"
#include <cassert>
#include <iostream>
#include <vector>

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
	vertexShaderText = File::ReadAllText( vertexShaderPath );
	if( useAssert )
		assert( !vertexShaderText.empty( ) );

	fragmentShaderText = File::ReadAllText( fragmentShaderPath );
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

GLint ShaderProgram::AttribLocationGet( const std::string name ) const
{
	GLint location = -1;

	location = glGetAttribLocation( program, name.c_str( ) );
	if( useAssert )
		assert( location != -1 );

	return location;
}

GLint ShaderProgram::UniformLocationGet( const std::string name ) const
{
	GLint location = -1;

	location = glGetUniformLocation( program, name.c_str( ) );
	if( useAssert )
		assert( location != -1 );

	return location;
}

GLuint ShaderProgram::ProgramGet( void ) const
{
	return program;
}
