#include "Manager.h"
#include <GLFW/glfw3.h>
#include "Scene.h"
#include "Simple.h"

namespace Scene
{

Manager::Manager( void )
{
}

Manager::~Manager( void )
{
}

void Manager::SceneInitialize( int index )
{
	switch( index )
	{
	case 1:
	default:
		scene = std::make_unique<Scene>( );
		break;
	case 2:
		scene = std::make_unique<Simple>( );
		break;
	}
}

void Manager::Initialize( const GLint width, const GLint height )
{
	this->width = width;
	this->height = height;

	glPushAttrib( GL_ALL_ATTRIB_BITS );

	SceneInitialize( 1 );
	scene->Initialize( width, height );
}

void Manager::Terminate( void )
{
	scene->Terminate( );
}

void Manager::Render( void )
{
	scene->Render( );
}

void Manager::FramebufferSizeCallback( const GLint width, const GLint height )
{
	this->width = width;
	this->height = height;

	scene->FramebufferSizeCallback( width, height );
}

void Manager::KeyCallback( const int key, const int scancode, const int action, const int mods )
{
	if( key >= GLFW_KEY_0 && key <= GLFW_KEY_9 && mods == GLFW_MOD_CONTROL && action == GLFW_PRESS )
	{
		scene->Terminate( );
		scene.reset( nullptr );

		glPopAttrib( );
		glPushAttrib( GL_ALL_ATTRIB_BITS );

		SceneInitialize( key - GLFW_KEY_0 );

		scene->Initialize( width, height );
	}
	else
	{
		scene->KeyCallback( key, scancode, action, mods );
	}
}

void Manager::CursorPosCallback( const double xpos, const double ypos )
{
	scene->CursorPosCallback( xpos, ypos );
}

void Manager::ScrollCallback( const double xoffset, const double yoffset )
{
	scene->ScrollCallback( xoffset, yoffset );
}

void Manager::MouseButtonCallback( const int button, const int action, const int mods )
{
	scene->MouseButtonCallback( button, action, mods );
}

void Manager::CharCallback( const unsigned int codepoint )
{
	scene->CharCallback( codepoint );
}

void Manager::CharModsCallback( const unsigned int codepoint, const int mods )
{
	scene->CharModsCallback( codepoint, mods );
}

}
