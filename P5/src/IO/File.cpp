
#include "File.h"
#include <fstream>

namespace IO
{
	File::File( )
	{
	}

	File::~File( )
	{
	}

	std::string File::ReadAllText( std::string path )
	{
		std::string text;
		std::ifstream in( path );

		in.seekg( 0, std::ios::end );
		text.reserve( static_cast<size_t>(in.tellg( )) );
		in.seekg( 0, std::ios::beg );

		text.assign(
			std::istreambuf_iterator<char>( in ),
			std::istreambuf_iterator<char>( ) );

		return text;
	}

	bool File::Exists( const std::string & path )
	{
		std::fstream fs( path );
		return fs.good( );
	}
}
