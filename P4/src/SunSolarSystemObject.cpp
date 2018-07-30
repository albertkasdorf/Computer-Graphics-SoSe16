#include "SunSolarSystemObject.h"
#include "SunShaderProgram.h"
#include "Texture.h"
#include "VertexArrayObject.h"



SunSolarSystemObject::SunSolarSystemObject(
	std::shared_ptr<VertexArrayObject> sphere,
	std::shared_ptr<Texture> diffuseMap,
	std::shared_ptr<Texture> distortionMap,
	std::shared_ptr<SunShaderProgram> shader )
	:
	SolarSystemObject( 0.0, 1.0, 360.0 / 25.38, 0.0 ),
	animationCounter( 0.0 ),
	sphere( sphere ),
	diffuseMap( diffuseMap ),
	distortionMap( distortionMap ),
	shader( shader )
{
}


SunSolarSystemObject::~SunSolarSystemObject( void )
{
}

void SunSolarSystemObject::Render( RenderContext& context )
{
	glUseProgram( shader->ProgramGet( ) );

	shader->ProjectionMatrixSet( context.projection );
	shader->ViewMatrixSet( context.view );

	animationCounter += 0.02f;

	shader->AnimationCounterSet( animationCounter );
	shader->BlendModeSet( blendMode );

	context.model.PushMatrix( );
	{
		context.model.Translate( distanceOrigin, 0.0, 0.0 );
		context.model.Rotate( rotationSelf * context.time, 0.0, 1.0, 0.0 );
		context.model.Scale( diameter );

		shader->ModelMatrixSet( context.model.MatrixGet( ) );

		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, diffuseMap->TextureIdGet( ) );
		shader->DiffuseMapTextureUnitSet( 0 );

		glActiveTexture( GL_TEXTURE1 );
		glBindTexture( GL_TEXTURE_2D, distortionMap->TextureIdGet( ) );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		shader->DistortionMapTextureUnitSet( 1 );

		sphere->Render( );
	}
	context.model.PopMatrix( );
}

void SunSolarSystemObject::BlendModeSet( GLint mode )
{
	blendMode = mode;
}
