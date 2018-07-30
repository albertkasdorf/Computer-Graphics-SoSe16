#pragma once
#include <string>
#include <map>
#include <vector>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>

namespace GL
{

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
	GLuint previousProgram;

	GLint locModel, locView, locProjection;

	std::map<std::string, GLint> uniformLocationLookUp;

public:
	// ShaderFileAdd( ShaderType::Fragment, const std::string filePath );
	// ShaderTextAdd( ShaderType::Vertex, const std::string text );
	// ShaderFileAdd( const std::string vertexShaderPath, const std::string fragmentShaderPath );
	virtual bool CompileAndLink( void );

	GLint AttribLocationGet( const std::string name );
	GLint UniformLocationGet( const std::string name );

	GLuint ProgramGet( void ) const;
	void ProgramUse( bool enable = true );
	void ProgramEnable( void );
	void ProgramDisable( void );

	void ModelMatrixSet( const glm::mat4 & model );
	void ViewMatrixSet( const glm::mat4 & view );
	void ProjectionMatrixSet( const glm::mat4 & projection );

	void UniformSet( const std::string name, GLfloat value );
	void UniformSet( const std::string name, GLboolean value );
	void UniformSet( const std::string name, GLint value );

	void UniformSet( const std::string name, GLfloat v0, GLfloat v1, GLfloat v2 );
	void UniformSet( const std::string name, glm::vec3 & value );
	void UniformSet( const std::string name, std::vector<float> & value );

	void UniformMatrixSet( const std::string name, std::vector<glm::mat4> & value );

protected:
	GLuint ShaderCreate( GLenum shaderType, std::string shaderText );

	std::string ShaderInfoLogGet( const GLuint shaderId );
	std::string ProgramInfoLogGet( const GLuint programId );
};

}
