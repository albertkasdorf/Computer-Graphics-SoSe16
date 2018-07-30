#pragma once
#include "SolarSystemObject.h"

class SunShaderProgram;
class Texture;
class VertexArrayObject;

class SunSolarSystemObject : public SolarSystemObject
{
public:
	SunSolarSystemObject(
		std::shared_ptr<VertexArrayObject> sphere,
		std::shared_ptr<Texture> diffuseMap,
		std::shared_ptr<Texture> distortionMap,
		std::shared_ptr<SunShaderProgram> shader );
	virtual ~SunSolarSystemObject( void );

protected:
	std::shared_ptr<SunShaderProgram> shader;
	std::shared_ptr<Texture> diffuseMap;
	std::shared_ptr<Texture> distortionMap;
	std::shared_ptr<VertexArrayObject> sphere;

	GLfloat animationCounter;
	GLint blendMode;

public:
	virtual void Render( RenderContext& context ) override;

public:
	void BlendModeSet( GLint mode );
};

