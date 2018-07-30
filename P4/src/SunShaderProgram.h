#pragma once
#include "ShaderProgram.h"

class SunShaderProgram :
	public ShaderProgram
{
public:
	SunShaderProgram( std::string vertexShaderPath, std::string fragmentShaderPath );
	virtual ~SunShaderProgram( void );

private:
	GLint locDiffuseMap, locDistortionMap;
	GLint locTime, locBlendMode;
	GLfloat sunCounter;
	GLint sunBlendMode;

public:
	virtual bool CompileAndLink( void ) override;

public:
	void DiffuseMapTextureUnitSet( GLint unit );
	void DistortionMapTextureUnitSet( GLint unit );

	void AnimationCounterSet( GLfloat time );
	void BlendModeSet( GLint mode );
};

