#pragma once
#include <GL/glew.h>
#include <GL/gl.h>

namespace GL
{

class VertexArray
{
public:
	VertexArray( void );
	~VertexArray( void );

private:
	GLuint id;

public:
	GLuint IdGet( void ) const;
};

}
