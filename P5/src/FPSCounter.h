#pragma once
#include <cstdint>
#include <cmath>
#include <string>

class ElapsedTime;
namespace UI
{
class PropertyWindow;
}

class FPSCounter
{
public:
	FPSCounter( void );
	~FPSCounter( void );

private:
	std::int32_t frames = 0;
	std::float_t lastTime = 0;

	std::float_t millisecondsPerFrame = 0.f;
	std::int32_t framesPerSecond = 0;

public:
	void Update( const ElapsedTime * elapsedTime );
	void Inspect(
		UI::PropertyWindow * const propertyWindow,
		const std::string & groupName );
};
