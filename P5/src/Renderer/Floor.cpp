#include "Floor.h"
#include "../GL/Buffer.h"
#include "../GL/ShaderProgram.h"
#include "../GL/Texture.h"
#include "../GL/VertexArray.h"

namespace Renderer
{
	Floor::Floor( void )
	{
	}

	Floor::~Floor( void )
	{
	}

	void Floor::Initialize( void )
	{
		//
		// Shader
		//
		shader = std::make_unique<GL::ShaderProgram>(
			"./Shader/Floor.vs",
			"./Shader/Floor.fs" );
		shader->CompileAndLink( );

		//
		// Texture
		//
		texture = std::make_unique<GL::Texture>(
			"./Assets/texture/ytrim_d.tga" );
			//"./Assets/hell/breakage1.tga" );
		texture->Create( );
		glBindTexture( GL_TEXTURE_2D, texture->IdGet( ) );
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		}
		glBindTexture( GL_TEXTURE_2D, 0 );

		//
		// Geometry
		//
		const float sizeFloor = 500.f;
		const float sizeUV = 32.f;
		const std::vector<glm::vec3> geometry{
			glm::vec3( +sizeFloor, +sizeFloor, 0.f ),
			glm::vec3( sizeUV, sizeUV, 0.f ),

			glm::vec3( -sizeFloor, +sizeFloor, 0.f ),
			glm::vec3( 0.f, sizeUV, 0.f ),

			glm::vec3( -sizeFloor, -sizeFloor, 0.f ),
			glm::vec3( 0.f, 0.f, 0.f ),

			glm::vec3( +sizeFloor, -sizeFloor, 0.f ),
			glm::vec3( sizeUV, 0.f, 0.f ),
		};
		count = static_cast<GLsizei>(geometry.size( ) / 2);

		const GLsizeiptr size = sizeof( glm::vec3 ) * geometry.size( );
		const GLvoid * data = &geometry[0];

		vbo = std::make_unique<GL::Buffer>( );
		glBindBuffer( GL_ARRAY_BUFFER, vbo->IdGet( ) );
		glBufferData( GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW );

		vao = std::make_unique<GL::VertexArray>( );
		glBindVertexArray( vao->IdGet( ) );
		{
			glBindBuffer( GL_ARRAY_BUFFER, vbo->IdGet( ) );

			GLuint index;
			GLint size;
			GLvoid * pointer;
			GLsizei stride;

			index = 0;
			size = sizeof( glm::vec3 ) / sizeof( float );
			stride = sizeof( glm::vec3 ) * 2;
			pointer = reinterpret_cast<GLvoid*>(0);
			glEnableVertexAttribArray( index );
			glVertexAttribPointer( index, size, GL_FLOAT, GL_FALSE, stride, pointer );

			index = 1;
			size = sizeof( glm::vec2 ) / sizeof( float );
			pointer = reinterpret_cast<GLvoid*>(reinterpret_cast<std::size_t>(pointer) + sizeof( glm::vec3 ));
			glEnableVertexAttribArray( index );
			glVertexAttribPointer( index, size, GL_FLOAT, GL_FALSE, stride, pointer );
		}
		glBindVertexArray( 0 );
	}

	void Floor::Render(
		const glm::mat4 & projection,
		const glm::mat4 & view,
		const glm::mat4 & model ) const
	{
		return;
		shader->ProgramUse( );
		shader->ProjectionMatrixSet( projection );
		shader->ViewMatrixSet( view );
		shader->ModelMatrixSet( model );

		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, texture->IdGet( ) );
		shader->UniformSet( "diffuseTexture", 0 );

		glBindVertexArray( vao->IdGet( ) );
		{
			glDrawArrays( GL_QUADS, 0, count );
		}
		glBindVertexArray( 0 );

		shader->ProgramUse( false );
	}
}
