#pragma once
#include <GL/glew.h>
#include <GL/gl.h>

namespace GL
{

class Buffer
{
public:
	Buffer( void );
	~Buffer( void );

private:
	GLuint id;

public:
	GLuint IdGet( void ) const;
};

}
