#pragma once
#include <string>

namespace IO
{
	class File
	{
	private:
		File( );
		~File( );

	public:
		static std::string ReadAllText( std::string path );
		static bool Exists( const std::string & path );
	};
}
