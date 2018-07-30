#include "ElapsedTime.h"
#include <algorithm>
#include <GLFW/glfw3.h>


ElapsedTime::ElapsedTime( void ) :
	max( 1.0 ),
	timeSinceStart( glfwGetTime( ) )
{
}

ElapsedTime::~ElapsedTime( void )
{
}

void ElapsedTime::Update( void )
{
	const double timeSinceStart = glfwGetTime( );

	elapsedTime = timeSinceStart - this->timeSinceStart;
	elapsedTime = std::min( elapsedTime, max );

	this->timeSinceStart = timeSinceStart;
}

float ElapsedTime::Get( void ) const
{
	return static_cast<float>( elapsedTime );
}

float ElapsedTime::TimeSinceStart( void ) const
{
	return static_cast<float>( timeSinceStart );
}
