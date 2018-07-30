#pragma once
#include <string>

class File
{
private:
	File( );
	~File( );

public:
	static std::string ReadAllText( std::string path );
};

