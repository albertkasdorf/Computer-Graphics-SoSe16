#pragma once

struct IScene
{
	virtual ~IScene( void ) {}

	virtual void Initialize( const GLint width, const GLint height ) = 0;
	virtual void Terminate( void ) = 0;
	virtual void Render( void ) = 0;
	virtual void Resized( const GLint width, const GLint height ) = 0;

	virtual void Keyboard( const int key, const int scancode, const int action, const int mods ) = 0;
	virtual void MouseMovement( const double xpos, const double ypos ) = 0;
	virtual void MouseScroll( const double xoffset, const double yoffset ) = 0;
};