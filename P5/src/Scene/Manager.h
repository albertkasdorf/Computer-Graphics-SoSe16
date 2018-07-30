#pragma once
#include <memory>
#include "IScene.h"

namespace Scene
{

class Manager : public IScene
{
public:
	Manager( void );
	virtual ~Manager( void );

private:
	std::unique_ptr<IScene> scene;
	GLint width, height;

private:
	void SceneInitialize( int index );

public:
	virtual void Initialize( const GLint width, const GLint height ) override;
	virtual void Terminate( void ) override;
	virtual void Render( void ) override;

	virtual void FramebufferSizeCallback( const GLint width, const GLint height ) override;
	virtual void KeyCallback( const int key, const int scancode, const int action, const int mods ) override;
	virtual void CursorPosCallback( const double xpos, const double ypos ) override;
	virtual void ScrollCallback( const double xoffset, const double yoffset ) override;
	virtual void MouseButtonCallback( const int button, const int action, const int mods ) override;
	virtual void CharCallback( const unsigned int codepoint ) override;
	virtual void CharModsCallback( const unsigned int codepoint, const int mods ) override;
};

}
