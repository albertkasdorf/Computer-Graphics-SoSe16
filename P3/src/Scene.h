#pragma once
#include <memory>
#include <glm/mat4x4.hpp>
#include "IScene.h"

class ShaderProgram;
class Texture;
class ModelMatrix;
class VertexArrayObject;
class Camera;

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

	/* Solar System */
	std::unique_ptr<ModelMatrix> model;

	std::unique_ptr<ShaderProgram> modelSP;
	GLuint modelModelLoc, modelViewLoc, modelProjLoc;

	GLboolean topViewAktiv;

	GLdouble timex;
	GLdouble movementSpeed;
	GLdouble rotateOriginStart[11];
	
	std::unique_ptr<VertexArrayObject> sphere;
	std::unique_ptr<VertexArrayObject> phobos;
	std::unique_ptr<VertexArrayObject> deimos;
	
	std::unique_ptr<Texture> sunTexture;
	std::unique_ptr<Texture> mercuryTexture, venusTexture;
	std::unique_ptr<Texture> earthTexture, moonTexture;
	std::unique_ptr<Texture> marsTexture, phobosTexture, deimosTexture;
	std::unique_ptr<Texture> jupiterTexture;
	std::unique_ptr<Texture> saturnTexture;
	std::unique_ptr<Texture> uranusTexture;
	std::unique_ptr<Texture> neptuneTexture;

	GLboolean animate;

	/* Konstanten */
	const GLboolean showAllPlanets;
	const GLboolean fixStartOrigin;


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
	void CreateGeometry( void );
	void CreateBufferObjects( void );
	void CreateTexture( void );
	void CreateShader( void );

	void RenderSun( GLdouble );
	void RenderMercury( GLdouble );
	void RenderVenus( GLdouble );
	void RenderEarthMoon( GLdouble );
	void RenderMars( GLdouble );
	void RenderJupiter( GLdouble );
	void RenderSaturn( GLdouble );
	void RenderUranus( GLdouble );
	void RenderNeptune( GLdouble );
};

