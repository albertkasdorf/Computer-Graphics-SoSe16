#pragma once
#include <vector>
#include <string>

namespace IO
{
	class Directory
	{
	public:
		Directory( void );
		~Directory( void );

	public:
		static std::vector<std::string> FilesGet( const std::string & path );
	};
}
