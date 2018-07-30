#pragma once
#include <string>
#include <GL/glew.h>

class ShaderProgram
{
public:
	ShaderProgram( std::string vertexShaderPath, std::string fragmentShaderPath, bool useAssert = true );
	virtual ~ShaderProgram( );

private:
	std::string vertexShaderPath;
	std::string fragmentShaderPath;

	const bool useAssert;

	GLuint program;

public:
	bool CompileAndLink( void );

	GLint AttribLocationGet( const std::string name ) const;
	GLint UniformLocationGet( const std::string name ) const;

	GLuint ProgramGet( void ) const;

protected:
	GLuint ShaderCreate( GLenum shaderType, std::string shaderText );

	std::string ShaderInfoLogGet( const GLuint shaderId );
	std::string ProgramInfoLogGet( const GLuint programId );
}; 

