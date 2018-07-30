#include "Directory.h"

namespace IO
{
	Directory::Directory( void )
	{
	}

	Directory::~Directory( void )
	{
	}

	std::vector<std::string> Directory::FilesGet( const std::string & path )
	{
		// TODO: Win32
//		WIN32_FIND_DATAA ffd = { };
//		HANDLE handle = INVALID_HANDLE_VALUE;
		std::vector<std::string> result;

//		handle = FindFirstFileA( path.c_str( ), &ffd );
//		if( handle != INVALID_HANDLE_VALUE )
//		{
//			do
//			{
//				if( ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
//				{
//					continue;
//				}
//				result.push_back( std::string( ffd.cFileName ) );
//			}
//			while( FindNextFileA( handle, &ffd ) != 0 );

//			FindClose( handle );
//		}

		return result;
	}
}
