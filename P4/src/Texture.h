#pragma once
#include <string>
#include <GL/glew.h>
#include <GL/gl.h>

class Texture
{
public:
	Texture( std::string texturePath );
	~Texture( );

private:
	std::string texturePath;
	GLuint textureId;

public:
	void TextureCreate( void );

	GLuint TextureIdGet( void );
};

