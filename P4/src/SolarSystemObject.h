#pragma once
#include <vector>
#include <memory>
#include "RenderContext.h"


class SolarSystemObject
{
public:
	SolarSystemObject(
		GLdouble distanceOrigin,
		GLdouble diameter,
		GLdouble rotationSelf,
		GLdouble rotationOrigin );
	virtual ~SolarSystemObject( void );

protected:
	GLdouble distanceOrigin;
	GLdouble diameter;
	GLdouble rotationSelf;
	GLdouble rotationOrigin;

	std::vector<std::shared_ptr<SolarSystemObject>> objects;

public:
	void SubObjectAdd( std::shared_ptr<SolarSystemObject> object );
	std::shared_ptr<SolarSystemObject> SubObjectGet( GLsizei index );
	GLsizei SubObjectCountGet( void );

public:
	virtual void Render( RenderContext& context ) = 0;
};

