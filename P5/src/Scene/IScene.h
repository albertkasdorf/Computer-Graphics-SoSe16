#pragma once
#include <GL/glew.h>
#include <GL/gl.h>

namespace Scene
{

struct IScene
{
	virtual ~IScene( void ) {}

	virtual void Initialize( const GLint width, const GLint height ) = 0;
	virtual void Terminate( void ) = 0;
	virtual void Render( void ) = 0;

	virtual void FramebufferSizeCallback( const GLint width, const GLint height ) = 0;
	virtual void KeyCallback( const int key, const int scancode, const int action, const int mods ) = 0;
	virtual void CursorPosCallback( const double xpos, const double ypos ) = 0;
	virtual void ScrollCallback( const double xoffset, const double yoffset ) = 0;
	virtual void MouseButtonCallback( const int button, const int action, const int mods ) = 0;

	virtual void CharCallback( const unsigned int codepoint ) { }
	virtual void CharModsCallback( const unsigned int codepoint, const int mods ) { }
};

}
