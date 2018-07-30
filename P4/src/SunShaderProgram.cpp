#include "SunShaderProgram.h"


SunShaderProgram::SunShaderProgram( std::string vertexShaderPath, std::string fragmentShaderPath ) :
	ShaderProgram( vertexShaderPath, fragmentShaderPath )
{
}


SunShaderProgram::~SunShaderProgram( void )
{
}

bool SunShaderProgram::CompileAndLink( void )
{
	bool result = ShaderProgram::CompileAndLink( );
	if( result )
	{
		locDiffuseMap = this->UniformLocationGet( "diffuseMap" );
		locDistortionMap = this->UniformLocationGet( "distortionMap" );

		locTime = this->UniformLocationGet( "time" );
		locBlendMode = this->UniformLocationGet( "blendMode" );
	}
	return result;
}

void SunShaderProgram::DiffuseMapTextureUnitSet( GLint unit )
{
	glUniform1i( locDiffuseMap, unit );
}

void SunShaderProgram::DistortionMapTextureUnitSet( GLint unit )
{
	glUniform1i( locDistortionMap, unit );
}

void SunShaderProgram::AnimationCounterSet( GLfloat time )
{
	glUniform1f( locTime, time );
}

void SunShaderProgram::BlendModeSet( GLint mode )
{
	glUniform1i( locBlendMode, mode );
}

