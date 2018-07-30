#include "FPSCounter.h"
#include "ElapsedTime.h"
#include "UI/PropertyWindow.h"


FPSCounter::FPSCounter( void )
{
}

FPSCounter::~FPSCounter( void )
{
}

void FPSCounter::Update( const ElapsedTime * elapsedTime )
{
	const float duration = elapsedTime->TimeSinceStart( ) - lastTime;
	
	frames++;
	if( duration > 1.f )
	{
		millisecondsPerFrame = 1000.f / static_cast<float>(frames);
		framesPerSecond = frames;

		frames = 0;
		lastTime = elapsedTime->TimeSinceStart( );
	}
}

void FPSCounter::Inspect(
	UI::PropertyWindow * const propertyWindow,
	const std::string & groupName )
{
	propertyWindow->ReadWriteVariableAdd(
		"FPSCounter::millisecondsPerFrame",
		"group='" + groupName + "' label='Millisecond/Frame' readonly=true",
		&millisecondsPerFrame );

	propertyWindow->ReadWriteVariableAdd(
		"FPSCounter::framesPerSecond",
		"group='" + groupName + "' label='Frames/Second' readonly=true",
		&framesPerSecond );
}
