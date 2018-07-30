#pragma once
#include "SolarSystemObject.h"

class PhongShaderProgram;
class Texture;
class VertexArrayObject;
struct RenderContext;

class PlanetSolarSystemObject : public SolarSystemObject
{
public:
	PlanetSolarSystemObject( 
		GLdouble distanceOrigin,
		GLdouble diameter,
		GLdouble rotationSelf,
		GLdouble rotationOrigin,
		GLdouble rotationOriginStart,
		std::shared_ptr<VertexArrayObject> mesh,
		std::shared_ptr<Texture> diffuseMap,
		std::shared_ptr<PhongShaderProgram> shader );
	virtual ~PlanetSolarSystemObject( void );

protected:
	std::shared_ptr<PhongShaderProgram> shader;
	std::shared_ptr<Texture> diffuseMap;
	std::shared_ptr<VertexArrayObject> mesh;
	GLdouble rotationOriginStart;

public:
	virtual void Render( RenderContext& context ) override;
};

