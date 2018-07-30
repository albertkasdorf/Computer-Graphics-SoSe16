
#include "VertexArray.h"

namespace GL
{

VertexArray::VertexArray( void ) :
	id( 0 )
{
	glGenVertexArrays( 1, &id );
}

VertexArray::~VertexArray( void )
{
	glDeleteVertexArrays( 1, &id );
}

GLuint VertexArray::IdGet( void ) const
{
	return id;
}

}