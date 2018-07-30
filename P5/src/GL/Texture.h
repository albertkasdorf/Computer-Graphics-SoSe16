#pragma once
#include <string>
#include <GL/glew.h>
#include <GL/gl.h>

namespace GL
{
	class Texture
	{
	public:
		Texture( std::string texturePath );
		~Texture( void );

	private:
		std::string texturePath;
		GLuint textureId;

	public:
		void Create( void );

		GLuint IdGet( void );
	};
}
