#include "Texture.h"
#include <iostream>
#include <cassert>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

Texture::Texture( std::string texturePath ) :
	texturePath( texturePath )
{
}

Texture::~Texture( )
{
	glDeleteTextures( 1, &textureId );
}

void Texture::TextureCreate( void )
{
	std::cout << "Loading texture=" << texturePath;
	double start = glfwGetTime( );

	GLint width = 0, height = 0;
	GLubyte* image = nullptr;
		
	image = SOIL_load_image( texturePath.c_str( ), &width, &height, 0, SOIL_LOAD_RGB );
	assert( image != nullptr );
	assert( width != 0 );
	assert( height != 0 );

	glGenTextures( 1, &textureId );
	glBindTexture( GL_TEXTURE_2D, textureId );
	{
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
		glGenerateMipmap( GL_TEXTURE_2D );
	}
	glBindTexture( GL_TEXTURE_2D, 0 );

	SOIL_free_image_data( image );

	std::cout << " time=" << glfwGetTime( ) - start << " ms [ok]" << std::endl;
}

GLuint Texture::TextureIdGet( void )
{
	return textureId;
}
