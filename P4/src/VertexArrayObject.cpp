#include "VertexArrayObject.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


VertexArrayObject::VertexArrayObject( std::string modelPath ) :
	modelPath( modelPath )
{
}


VertexArrayObject::~VertexArrayObject( void )
{
	glDeleteBuffers( 1, &vertexVBO );
	glDeleteBuffers( 1, &normalVBO );
	glDeleteBuffers( 1, &indexEBO );
	glDeleteBuffers( 1, &textureCoordVBO );
	glDeleteVertexArrays( 1, &meshVAO );
}

void VertexArrayObject::Create( void )
{
	std::cout << "Creating VertexArrayObject=" << modelPath;
	double start = glfwGetTime( );

	Assimp::Importer importer;
	const aiScene* scene;
	std::vector<GLfloat> vertex;
	std::vector<GLfloat> normal;
	std::vector<GLuint> index;
	std::vector<GLfloat> textureCoord;

	scene = importer.ReadFile(
		modelPath,
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals );
	assert( scene );

	vertex.resize( scene->mMeshes[0]->mNumVertices * 3 );
	normal.resize( scene->mMeshes[0]->mNumVertices * 3 );
	textureCoord.resize( scene->mMeshes[0]->mNumVertices * 2 );
	for( GLuint i = 0; i < scene->mMeshes[0]->mNumVertices; i++ )
	{
		vertex[i * 3 + 0] = scene->mMeshes[0]->mVertices[i].x;
		vertex[i * 3 + 1] = scene->mMeshes[0]->mVertices[i].y;
		vertex[i * 3 + 2] = scene->mMeshes[0]->mVertices[i].z;

		normal[i * 3 + 0] = scene->mMeshes[0]->mNormals[i].x;
		normal[i * 3 + 1] = scene->mMeshes[0]->mNormals[i].y;
		normal[i * 3 + 2] = scene->mMeshes[0]->mNormals[i].z;

		textureCoord[i * 2 + 0] = scene->mMeshes[0]->mTextureCoords[0][i].x;
		textureCoord[i * 2 + 1] = scene->mMeshes[0]->mTextureCoords[0][i].y;
	}

	index.resize( scene->mMeshes[0]->mNumFaces * 3 );
	for( GLuint i = 0; i < scene->mMeshes[0]->mNumFaces; i++ )
	{
		index[i * 3 + 0] = scene->mMeshes[0]->mFaces[i].mIndices[0];
		index[i * 3 + 1] = scene->mMeshes[0]->mFaces[i].mIndices[1];
		index[i * 3 + 2] = scene->mMeshes[0]->mFaces[i].mIndices[2];
	}
	indexCount = index.size( );

	importer.FreeScene( );

	glGenVertexArrays( 1, &meshVAO );
	glGenBuffers( 1, &vertexVBO );
	glGenBuffers( 1, &normalVBO );
	glGenBuffers( 1, &indexEBO );
	glGenBuffers( 1, &textureCoordVBO );
	glBindVertexArray( meshVAO );
	{
		const auto vertexSizeInBytes = vertex.size( ) * sizeof( decltype(vertex)::value_type );
		const auto normalSizeInBytes = normal.size( ) * sizeof( decltype(normal)::value_type );
		const auto indexSizeInBytes = index.size( ) * sizeof( decltype(index)::value_type );
		const auto textureCoordSizeInBytes = textureCoord.size( ) * sizeof( decltype(textureCoord)::value_type );

		glBindBuffer( GL_ARRAY_BUFFER, vertexVBO );
		glBufferData( GL_ARRAY_BUFFER, vertexSizeInBytes, &vertex[0], GL_STATIC_DRAW );
		glEnableVertexAttribArray( 0 );
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), reinterpret_cast<GLvoid*>(0) );

		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexEBO );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, indexSizeInBytes, &index[0], GL_STATIC_DRAW );

		glBindBuffer( GL_ARRAY_BUFFER, textureCoordVBO );
		glBufferData( GL_ARRAY_BUFFER, textureCoordSizeInBytes, &textureCoord[0], GL_STATIC_DRAW );
		glEnableVertexAttribArray( 1 );
		glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof( GLfloat ), reinterpret_cast<GLvoid*>(0) );

		glBindBuffer( GL_ARRAY_BUFFER, normalVBO );
		glBufferData( GL_ARRAY_BUFFER, normalSizeInBytes, &normal[0], GL_STATIC_DRAW );
		glEnableVertexAttribArray( 2 );
		glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), reinterpret_cast<GLvoid*>(0) );

	}
	glBindVertexArray( 0 );

	std::cout << " time=" << glfwGetTime( ) - start << " ms [ok]" << std::endl;
}

void VertexArrayObject::Render( void )
{
	glBindVertexArray( meshVAO );
		glDrawElements( GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0 );
	glBindVertexArray( 0 );
}
