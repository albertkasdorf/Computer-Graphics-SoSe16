#pragma once
#include <string>
#include <chrono>

class StopWatch
{
public:
	StopWatch( std::string text );
	~StopWatch( );

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start, stop;
	std::string text;
};

