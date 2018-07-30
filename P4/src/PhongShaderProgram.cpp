#include "PhongShaderProgram.h"
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

PhongShaderProgram::PhongShaderProgram( std::string vertexShaderPath, std::string fragmentShaderPath ) :
	ShaderProgram( vertexShaderPath, fragmentShaderPath )
{
}

PhongShaderProgram::~PhongShaderProgram( void )
{
}

bool PhongShaderProgram::CompileAndLink( void )
{
	bool result = ShaderProgram::CompileAndLink( );
	if( result )
	{
		locNormal = this->UniformLocationGet( "normal" );

		locLightPosition = this->UniformLocationGet( "lightPosition" );
		locLightIntensity = this->UniformLocationGet( "lightIntensity" );
		locDiffuse = this->UniformLocationGet( "kd" );
		locAmbient = this->UniformLocationGet( "ka" );
		locSpecular = this->UniformLocationGet( "ks" );
		locShininess = this->UniformLocationGet( "shininess" );
		locDiffuseMap = this->UniformLocationGet( "diffuseMap" );
	}
	return result;
}

void PhongShaderProgram::NormalMatrixSet( const glm::mat4& model, glm::mat4& view )
{
	glm::mat4 viewModelMatrix = view * model;
	glm::mat4 normalMatrix = glm::inverseTranspose( viewModelMatrix );

	glUniformMatrix4fv( locNormal, 1, GL_FALSE, glm::value_ptr( normalMatrix ) );
}

void PhongShaderProgram::LightPositionSet( GLdouble x, GLdouble y, GLdouble z )
{
	glUniform3f(
		locLightPosition,
		static_cast<GLfloat>(x), static_cast<GLfloat>(y), static_cast<GLfloat>(z) );
}

void PhongShaderProgram::LightColorSet( GLdouble r, GLdouble g, GLdouble b )
{
	glUniform3f(
		locLightIntensity,
		static_cast<GLfloat>(r), static_cast<GLfloat>(g), static_cast<GLfloat>(b) );
}

void PhongShaderProgram::DiffuseColorSet( GLdouble r, GLdouble g, GLdouble b )
{
	glUniform3f(
		locDiffuse,
		static_cast<GLfloat>(r), static_cast<GLfloat>(g), static_cast<GLfloat>(b) );
}

void PhongShaderProgram::AmbientColorSet( GLdouble r, GLdouble g, GLdouble b )
{
	glUniform3f(
		locAmbient,
		static_cast<GLfloat>(r), static_cast<GLfloat>(g), static_cast<GLfloat>(b) );
}

void PhongShaderProgram::SpecularColorSet( GLdouble r, GLdouble g, GLdouble b )
{
	glUniform3f(
		locSpecular, 
		static_cast<GLfloat>(r), static_cast<GLfloat>(g), static_cast<GLfloat>(b) );
}

void PhongShaderProgram::SpecularExponentSet( GLdouble n )
{
	glUniform1f( locShininess, static_cast<GLfloat>(n) );
}

void PhongShaderProgram::DiffuseMapTextureUnitSet( GLint unit )
{
	glUniform1i( locDiffuseMap, unit );
}
