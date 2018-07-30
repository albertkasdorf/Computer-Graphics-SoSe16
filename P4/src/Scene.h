#pragma once
#include <memory>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include "IScene.h"

class ShaderProgram;
class PhongShaderProgram;
class SunShaderProgram;
class Texture;
class ModelMatrix;
class VertexArrayObject;
class Camera;
class SolarSystemObject;
struct RenderContext;
class SkyBox;

class Scene : public IScene
{
public:
	Scene( );
	virtual ~Scene( );

private:
	GLdouble deltaTime = 0.0f;
	GLdouble lastFrame = 0.0f;
	
	/* Camera */
	std::unique_ptr<Camera> camera;
	GLdouble fov = 45.f;
	GLdouble aspectRatio;
	GLboolean perspective = GL_TRUE;
	glm::mat4 projection, view;

	/* SkyBox */
	std::unique_ptr<SkyBox> skyBox;

	/* Solar System */
	std::shared_ptr<SolarSystemObject> root;

	std::shared_ptr<SunShaderProgram> sunSP;
	float sunCounter;
	GLint sunBlendMode;

	std::shared_ptr<PhongShaderProgram> phongSP;

	GLboolean topViewAktiv;

	GLdouble movementSpeed;
	GLdouble rotateOriginStart[11];
	
	std::shared_ptr<VertexArrayObject> sphere;
	std::shared_ptr<VertexArrayObject> phobos;
	std::shared_ptr<VertexArrayObject> deimos;
	
	std::shared_ptr<Texture> sunTexture;
	std::shared_ptr<Texture> sunDistortion;
	std::shared_ptr<Texture> mercuryTexture, venusTexture;
	std::shared_ptr<Texture> earthTexture, moonTexture;
	std::shared_ptr<Texture> marsTexture, phobosTexture, deimosTexture;
	std::shared_ptr<Texture> jupiterTexture;
	std::shared_ptr<Texture> saturnTexture;
	std::shared_ptr<Texture> uranusTexture;
	std::shared_ptr<Texture> neptuneTexture;

	GLboolean animate;

	/* Konstanten */
	const GLboolean showAllPlanets;
	const GLboolean fixStartOrigin;

	/* SkyBox */
	//GLuint cubemapTextureId;


public:
	/* IScene interface implementation */
	virtual void Initialize( const GLint width, const GLint height ) override;
	virtual void Terminate( void ) override;
	virtual void Render( void ) override;
	virtual void Resized( const GLint width, const GLint height ) override;

	virtual void Keyboard( const int key, const int scancode, const int action, const int mods ) override;
	virtual void MouseMovement( const double xpos, const double ypos ) override;
	virtual void MouseScroll( const double xoffset, const double yoffset ) override;

private:
	void BufferObjectsCreate( void );
	void TextureCreate( void );
	void ShaderCreate( void );
	void SolarSystemCreate( void );
	//void SkyBoxCreate( void );

	void SolarSystemObjectRender( RenderContext& context, std::shared_ptr<SolarSystemObject> object );

	void GLStringPrint( void );
};

