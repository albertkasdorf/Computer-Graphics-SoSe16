#pragma once
#include <memory>
#include "IScene.h"

class Camera;
class ElapsedTime;
namespace GL
{
	class Buffer;
	class ShaderProgram;
	class Texture;
	class VertexArray;
}

namespace Scene
{

class Simple : public IScene
{
public:
	Simple( );
	~Simple( );

private:
	const std::unique_ptr<GL::Buffer> vbo;
	const std::unique_ptr<GL::Buffer> ebo;
	const std::unique_ptr<GL::VertexArray> vao;

	std::unique_ptr<GL::ShaderProgram> shader;

	std::unique_ptr<Camera> camera;
	std::unique_ptr<ElapsedTime> elapsedTime;

private:
	void InitializeGeometry( void );
	void InitializeShader( void );

public:
	virtual void Initialize( const GLint width, const GLint height ) override;
	virtual void Terminate( void ) override;
	virtual void Render( void ) override;

	virtual void FramebufferSizeCallback( const GLint width, const GLint height ) override;
	virtual void KeyCallback( const int key, const int scancode, const int action, const int mods ) override;
	virtual void CursorPosCallback( const double xpos, const double ypos ) override;
	virtual void ScrollCallback( const double xoffset, const double yoffset ) override;
	virtual void MouseButtonCallback( const int button, const int action, const int mods ) override;
};

}
