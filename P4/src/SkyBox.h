#pragma once
#include <memory>
#include "RenderContext.h"

class ShaderProgram;

class SkyBox
{
public:
	SkyBox( void );
	~SkyBox( void );

private:
	GLuint textureId;
	GLuint vaoId, vboId;
	std::unique_ptr<ShaderProgram> shader;

public:
	void Create( void );
	void Render( RenderContext context );

private:
	void TextureLoad( void );
	void VAOCreate( void );
	void ShaderCreate( void );
};

