#pragma once
#include <string>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>

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

	GLint locModel, locView, locProjection;

public:
	virtual bool CompileAndLink( void );

	GLint AttribLocationGet( const std::string name ) const;
	GLint UniformLocationGet( const std::string name ) const;

	GLuint ProgramGet( void ) const;

	void ModelMatrixSet( const glm::mat4& model );
	void ViewMatrixSet( glm::mat4& view );
	void ProjectionMatrixSet( glm::mat4& projection );

protected:
	GLuint ShaderCreate( GLenum shaderType, std::string shaderText );

	std::string ShaderInfoLogGet( const GLuint shaderId );
	std::string ProgramInfoLogGet( const GLuint programId );
}; 

