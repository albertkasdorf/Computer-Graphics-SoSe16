#include "PlanetSolarSystemObject.h"
#include "PhongShaderProgram.h"
#include "RenderContext.h"
#include "Texture.h"
#include "VertexArrayObject.h"


PlanetSolarSystemObject::PlanetSolarSystemObject(
	GLdouble distanceOrigin,
	GLdouble diameter,
	GLdouble rotationSelf,
	GLdouble rotationOrigin,
	GLdouble rotationOriginStart,
	std::shared_ptr<VertexArrayObject> mesh,
	std::shared_ptr<Texture> diffuseMap,
	std::shared_ptr<PhongShaderProgram> shader )
	:
	SolarSystemObject( distanceOrigin, diameter, rotationSelf, rotationOrigin ),
	rotationOriginStart( rotationOriginStart ),
	mesh( mesh ),
	diffuseMap( diffuseMap ),
	shader( shader )
{
}


PlanetSolarSystemObject::~PlanetSolarSystemObject( void )
{
}

void PlanetSolarSystemObject::Render( RenderContext& context )
{
	const GLdouble diffuse = 0.9;
	const GLdouble ambient = 0.05;		// nahezu schwarzes Umgebungslicht
	const GLdouble specular = 0.0;
	

	glUseProgram( shader->ProgramGet( ) );

	shader->ProjectionMatrixSet( context.projection );
	shader->ViewMatrixSet( context.view );
	shader->DiffuseMapTextureUnitSet( 0 );

	shader->LightPositionSet( 0.0, 0.0, 0.0 );
	shader->LightColorSet( 1.0, 1.0, 1.0 );
	shader->DiffuseColorSet( diffuse, diffuse, diffuse );
	shader->AmbientColorSet( ambient, ambient, ambient );
	shader->SpecularColorSet( specular, specular, specular );
	shader->SpecularExponentSet( 8.0 );

	context.model.Rotate( rotationOrigin * context.time + rotationOriginStart, 0.0, 1.0, 0.0 );

	context.model.Translate( distanceOrigin, 0.0, 0.0 );
	context.model.Rotate( rotationSelf * context.time, 0.0, 1.0, 0.0 );
	context.model.Scale( diameter );

	shader->ModelMatrixSet( context.model.MatrixGet( ) );
	shader->NormalMatrixSet( context.model.MatrixGet( ), context.view );

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, diffuseMap->TextureIdGet( ) );

	mesh->Render( );
}
