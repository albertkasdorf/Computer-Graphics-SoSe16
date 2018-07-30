#pragma once
#include <string>
#include <GL/glew.h>
#include <GL/gl.h>

class VertexArrayObject
{
public:
	VertexArrayObject( std::string modelPath );
	~VertexArrayObject( void );

private:
	std::string modelPath;
	
	GLuint meshVAO;
	GLuint vertexVBO;
	GLuint indexEBO;
	GLuint textureCoordVBO;
	GLsizei indexCount;

public:
	void Create( void );
	void Render( void );
};

