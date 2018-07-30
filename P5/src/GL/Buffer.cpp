
#include "Buffer.h"

namespace GL
{

Buffer::Buffer( void ) :
	id( 0 )
{
	glGenBuffers( 1, &id );
}

Buffer::~Buffer( void )
{
	glDeleteBuffers( 1, &id );
}

GLuint Buffer::IdGet( void ) const
{
	return id;
}

}