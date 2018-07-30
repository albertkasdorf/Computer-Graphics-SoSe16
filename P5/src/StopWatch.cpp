#include "StopWatch.h"
#include <iostream>


StopWatch::StopWatch( std::string text )
	:
	text( text )
{
	start = std::chrono::high_resolution_clock::now( );
}


StopWatch::~StopWatch( )
{
	stop = std::chrono::high_resolution_clock::now( );

	std::cout << text << ": ";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count( );
	std::cout << " ms" << std::endl;
}
