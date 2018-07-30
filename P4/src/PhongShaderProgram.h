#pragma once
#include "ShaderProgram.h"

class PhongShaderProgram : public ShaderProgram
{
public:
	PhongShaderProgram( std::string vertexShaderPath, std::string fragmentShaderPath );
	virtual ~PhongShaderProgram( void );

private:
	GLint locNormal;
	GLint locLightPosition, locLightIntensity;
	GLint locDiffuse, locAmbient, locSpecular, locShininess;
	GLint locDiffuseMap;

	glm::mat4 normalMatrix;

public:
	virtual bool CompileAndLink( void ) override;

public:
	void NormalMatrixSet( const glm::mat4& model, glm::mat4& view );

	void LightPositionSet( GLdouble x, GLdouble y, GLdouble z );
	void LightColorSet( GLdouble r, GLdouble g, GLdouble b );
	void DiffuseColorSet( GLdouble r, GLdouble g, GLdouble b );
	void AmbientColorSet( GLdouble r, GLdouble g, GLdouble b );
	void SpecularColorSet( GLdouble r, GLdouble g, GLdouble b );
	void SpecularExponentSet( GLdouble n );

	void DiffuseMapTextureUnitSet( GLint unit );
};

