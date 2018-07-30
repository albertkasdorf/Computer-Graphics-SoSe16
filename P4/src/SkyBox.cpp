#include "SkyBox.h"
#include <vector>
#include <SOIL/SOIL.h>
#include "ShaderProgram.h"


SkyBox::SkyBox( void )
{
}


SkyBox::~SkyBox( void )
{
}

void SkyBox::Create( void )
{
	ShaderCreate( );
	VAOCreate( );
	TextureLoad( );
}

void SkyBox::Render( RenderContext context )
{
	// Draw skybox first

	// Remember to turn depth writing off
	glDepthMask( GL_FALSE );
	{
		glUseProgram( shader->ProgramGet( ) );

		// Remove any translation component of the view matrix
		glm::mat4 view = glm::mat4( glm::mat3( context.view ) );
		glm::mat4 projection = context.projection;

		shader->ViewMatrixSet( view );
		shader->ProjectionMatrixSet( projection );

		// skybox cube
		glBindVertexArray( vaoId );
		{
			glActiveTexture( GL_TEXTURE0 );
			glUniform1i( shader->UniformLocationGet( "skybox" ), 0 );
			glBindTexture( GL_TEXTURE_CUBE_MAP, textureId );
			glDrawArrays( GL_TRIANGLES, 0, 36 );
		}
		glBindVertexArray( 0 );
	}
	glDepthMask( GL_TRUE );
}

void SkyBox::TextureLoad( void )
{
	std::vector<std::string> faces( {
		"./Assets/ame_starfield/starfield_bk.png",
		"./Assets/ame_starfield/starfield_dn.png",
		"./Assets/ame_starfield/starfield_ft.png",
		"./Assets/ame_starfield/starfield_lf.png",
		"./Assets/ame_starfield/starfield_rt.png",
		"./Assets/ame_starfield/starfield_up.png"
	} );

	glGenTextures( 1, &textureId );
	glActiveTexture( GL_TEXTURE0 );

	int width, height;
	unsigned char* image;

	glBindTexture( GL_TEXTURE_CUBE_MAP, textureId );
	for( GLuint i = 0; i < faces.size( ); i++ )
	{
		image = SOIL_load_image( faces[i].c_str( ), &width, &height, 0, SOIL_LOAD_RGB );
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
			GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
		);
	}
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
	glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
}

void SkyBox::VAOCreate( void )
{
	GLfloat vertices[] = {
		// Positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays( 1, &vaoId );
	glGenBuffers( 1, &vboId );
	glBindVertexArray( vaoId );
	{
		glBindBuffer( GL_ARRAY_BUFFER, vboId );
		glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), &vertices, GL_STATIC_DRAW );
		glEnableVertexAttribArray( 0 );
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), (GLvoid*)0 );
	}
	glBindVertexArray( 0 );
}

void SkyBox::ShaderCreate( void )
{
	shader = std::make_unique<ShaderProgram>(
		"./Shader/SkyBox.vert", "./Shader/SkyBox.frag", false );
	shader->CompileAndLink( );
}
