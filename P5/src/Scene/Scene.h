#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "IScene.h"

class Camera;
class ElapsedTime;
class FPSCounter;
namespace GL
{
	class ShaderProgram;
}
namespace Renderer
{
	class Model;
	class CoordinateAxis;
}
namespace UI
{
	class PropertyWindow;
}

namespace Scene
{

class Scene : public IScene
{
public:
	Scene( );
	virtual ~Scene( );

private:
	glm::mat4 projection, view, model;

	std::shared_ptr<GL::ShaderProgram> defaultSP;

	std::unique_ptr<Camera> camera;

	std::unique_ptr<Renderer::CoordinateAxis> coordinateAxis;
	std::unique_ptr<Renderer::Model> playerModel;

	std::unique_ptr<ElapsedTime> elapsedTime;
	std::unique_ptr<FPSCounter> fpsCounter;
	std::unique_ptr<UI::PropertyWindow> propertyWindow;

	glm::vec4 clearColor;

	

public:
	/* IScene interface implementation */
	virtual void Initialize( const GLint width, const GLint height ) override;
	virtual void Terminate( void ) override;
	virtual void Render( void ) override;
	virtual void FramebufferSizeCallback( const GLint width, const GLint height ) override;

	virtual void KeyCallback( const int key, const int scancode, const int action, const int mods ) override;
	virtual void CursorPosCallback( const double xpos, const double ypos ) override;
	virtual void ScrollCallback( const double xoffset, const double yoffset ) override;
	virtual void MouseButtonCallback( const int button, const int action, const int mods ) override;

	virtual void CharCallback( const unsigned int codepoint ) override;

private:
	void BufferObjectsCreate( void );
	void TextureCreate( void );
	void ShaderCreate( void );
	void GLStringPrint( void );
	void LoadAnimation( void );

	void Inspect( UI::PropertyWindow * const propertyWindow );
};

}
