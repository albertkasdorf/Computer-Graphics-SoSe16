#include "SolarSystemObject.h"


SolarSystemObject::SolarSystemObject(
	GLdouble distanceOrigin,
	GLdouble diameter,
	GLdouble rotationSelf,
	GLdouble rotationOrigin )
	:
	distanceOrigin( distanceOrigin ),
	diameter( diameter ),
	rotationSelf( rotationSelf ),
	rotationOrigin( rotationOrigin )
{
}


SolarSystemObject::~SolarSystemObject( void )
{
}

void SolarSystemObject::SubObjectAdd( std::shared_ptr<SolarSystemObject> object )
{
	objects.push_back( object );
}

std::shared_ptr<SolarSystemObject> SolarSystemObject::SubObjectGet( GLsizei index )
{
	return objects[index];
}

GLsizei SolarSystemObject::SubObjectCountGet( void )
{
	return objects.size( );
}
