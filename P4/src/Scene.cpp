#include "Scene.h"
#include <random>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "File.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "ModelMatrix.h"
#include "VertexArrayObject.h"
#include "Camera.h"
#include "PhongShaderProgram.h"
#include "SunShaderProgram.h"
#include "RenderContext.h"
#include "SolarSystemObject.h"
#include "SunSolarSystemObject.h"
#include "PlanetSolarSystemObject.h"
#include "SkyBox.h"

Scene::Scene( void ) :
	showAllPlanets( GL_TRUE ),
	fixStartOrigin( GL_TRUE ),
	movementSpeed( 0.1 ),
	animate( GL_TRUE ),
	topViewAktiv( GL_FALSE ),
	sunBlendMode( 0 )
{
}

Scene::~Scene( void )
{
}

void Scene::Initialize( const GLint width, const GLint height )
{
	GLStringPrint( );

	camera = std::make_unique<Camera>(
		glm::vec3( 0.0, 0.0, 12.0 ),
		glm::vec3( 0.0, 0.0, -1.0 ),
		glm::vec3( 0.0, 1.0, 0.0 ) );

	BufferObjectsCreate( );
	TextureCreate( );
	ShaderCreate( );
	SolarSystemCreate( );
	skyBox = std::make_unique<SkyBox>( );
	skyBox->Create( );

	glEnable( GL_CULL_FACE );

	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );

	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	// Activate Wireframe Mode
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	glClearDepth( 1.0 );

	// Hintergrundfarbe ist Schwarz.
	glClearColor( 0.0, 0.0, 0.0, 1.0 );

	//----
	Resized( width, height );

	/* Random Planet Start Position */
	std::default_random_engine generator;
	std::uniform_real_distribution<GLdouble> distribution( 0.0, 356.0 );

	for( int i = 0; i < sizeof( rotateOriginStart ) / sizeof( GLdouble ); i++ )
	{
		rotateOriginStart[i] = fixStartOrigin ? 0.0 : distribution( generator );
	}
}

void Scene::Terminate( void )
{
}

void Scene::Resized( const GLint width, const GLint height )
{
	aspectRatio = static_cast<GLfloat>(width) / static_cast<GLfloat>(height);
	glViewport( 0, 0, width, height );
}

void Scene::Keyboard( const int key, const int scancode, const int action, const int mods )
{
	if( key == GLFW_KEY_P && action == GLFW_PRESS )
	{
		perspective = !perspective;
	}

	/* Camera */
	camera->KeyboardProcess( key, scancode, action, mods );

	if( key == GLFW_KEY_V && action == GLFW_PRESS )
		topViewAktiv = !topViewAktiv;

	/* Planet Moment */
	if( key == GLFW_KEY_R && action == GLFW_PRESS )
		animate = !animate;

	if( ( key == GLFW_KEY_I || key == GLFW_KEY_O ) && action == GLFW_PRESS )
	{
		GLdouble factor = std::log10( movementSpeed );
		factor = std::round( factor );
		if( key == GLFW_KEY_I )
			factor++;
		else
			factor--;
		std::cout << "Factor=" << factor << std::endl;
		movementSpeed = std::pow( 10, factor );
	}

	/* Sun Blend Mode */
	if( key == GLFW_KEY_M && action == GLFW_PRESS && mods == 0 )
	{
		sunBlendMode = (sunBlendMode + 1) % 9;
		std::cout << "Sun blend mode=" << sunBlendMode << std::endl;
	}
	if( key == GLFW_KEY_M && action == GLFW_PRESS &&mods == GLFW_MOD_SHIFT )
	{
		sunBlendMode = (sunBlendMode + 8) % 9;
		std::cout << "Sun blend mode=" << sunBlendMode << std::endl;
	}
}

void Scene::MouseMovement( const double xpos, const double ypos )
{
}

void Scene::MouseScroll( const double xoffset, const double yoffset )
{
	if( fov >= 1.0 && fov <= 45.0 )
		fov -= yoffset;
	if( fov <= 1.0 )
		fov = 1.0;
	if( fov >= 45.0 )
		fov = 45.0;
}

void Scene::Render( void )
{
	RenderContext context;

	GLdouble currentFrame = glfwGetTime( );
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	// Clear buffer to set color and alpha
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// setup projection matrix
	if( perspective )
	{
		context.projection = projection = glm::perspective(
			glm::radians( fov ),
			aspectRatio,
			0.1, 1000.0 );
	}
	else
	{
		context.projection = projection = glm::ortho( -3.0, 3.0, -3.0, 3.0, 20.0, -20.0 );
	}

	/* Camera */
	context.view = view = camera->ViewMatrixGet( deltaTime );
	
	/* SkyBox */
	skyBox->Render( context );

	/* Solar System */
	
	// Reset Model
	context.model.LoadIdentity( );
	
	// TopView
	if( topViewAktiv )
	{
		context.model.Rotate( 90.0, 1.0, 0.0, 0.0 );
	}
	
	//----
	if( animate )
	{
		context.time = glfwGetTime( ) * movementSpeed;
	}

	/* Solar System */
	reinterpret_cast<SunSolarSystemObject*>(root.get( ))->BlendModeSet( sunBlendMode );
	SolarSystemObjectRender( context, root );
}

void Scene::SolarSystemObjectRender( RenderContext& context, std::shared_ptr<SolarSystemObject> object )
{
	context.model.PushMatrix( );

	object->Render( context );

	for( GLsizei i = 0; i < object->SubObjectCountGet( ); i++ )
	{
		SolarSystemObjectRender( context, object->SubObjectGet( i ) );
	}

	context.model.PopMatrix( );
}

void Scene::GLStringPrint( void )
{
	const GLubyte* glString = nullptr;

	glString = glGetString( GL_VENDOR );
	std::cout << "GL_VENDOR=" << glString << std::endl;

	glString = glGetString( GL_RENDERER );
	std::cout << "GL_RENDERER=" << glString << std::endl;

	glString = glGetString( GL_VERSION );
	std::cout << "GL_VERSION=" << glString << std::endl;

	glString = glGetString( GL_SHADING_LANGUAGE_VERSION );
	std::cout << "GL_SHADING_LANGUAGE_VERSION=" << glString << std::endl;

	/*glString = glGetString( GL_EXTENSIONS );
	std::cout << "GL_EXTENSIONS=" << glString << std::endl;*/

	std::cout << std::endl;
}

void Scene::BufferObjectsCreate( void )
{
	/* Solar System */
	sphere = std::make_shared<VertexArrayObject>( "./Assets/sphere_low.obj" );
	sphere->Create( );

	if( showAllPlanets )
	{
		phobos = std::make_shared<VertexArrayObject>( "./Assets/Phobos.3ds" );
		phobos->Create( );

		deimos = std::make_shared<VertexArrayObject>( "./Assets/Deimos.3ds" );
		deimos->Create( );
	}
}

void Scene::TextureCreate( void )
{
	/* Solar System */
	sunTexture = std::make_shared<Texture>( "./Assets/Sun_ColorMap2.jpg" );
	sunTexture->TextureCreate( );

	// Distortion3_800x800, Distortion1_512x512
	sunDistortion = std::make_shared<Texture>( "./Assets/Distortion3_800x800.jpg" );
	sunDistortion->TextureCreate( );

	mercuryTexture = std::make_shared<Texture>( "./Assets/Mercury_ColorMap.jpg" );
	mercuryTexture->TextureCreate( );

	if( showAllPlanets )
	{
		venusTexture = std::make_shared<Texture>( "./Assets/Venus_ColorMap.jpg" );
		venusTexture->TextureCreate( );
	}

	earthTexture = std::make_shared<Texture>( "./Assets/Earth_ColorMap.jpg" );
	earthTexture->TextureCreate( );

	moonTexture = std::make_shared<Texture>( "./Assets/Moon_ColorMap.jpg" );
	moonTexture->TextureCreate( );
	
	if( showAllPlanets )
	{
		marsTexture = std::make_shared<Texture>( "./Assets/Mars_ColorMap1.jpg" );
		marsTexture->TextureCreate( );

		phobosTexture = std::make_shared<Texture>( "./Assets/Phobos_ColorMap.png" );
		phobosTexture->TextureCreate( );

		deimosTexture = std::make_shared<Texture>( "./Assets/Deimos_ColorMap.jpg" );
		deimosTexture->TextureCreate( );

		jupiterTexture = std::make_shared<Texture>( "./Assets/Jupiter_ColorMap1.jpg" );
		jupiterTexture->TextureCreate( );

		saturnTexture = std::make_shared<Texture>( "./Assets/Saturn_ColorMap.jpg" );
		saturnTexture->TextureCreate( );

		uranusTexture = std::make_shared<Texture>( "./Assets/Uranus_ColorMap.jpg" );
		uranusTexture->TextureCreate( );

		neptuneTexture = std::make_shared<Texture>( "./Assets/Neptune_ColorMap.jpg" );
		neptuneTexture->TextureCreate( );
	}
}

void Scene::ShaderCreate( void )
{
	/* Sun */
	sunSP = std::make_shared<SunShaderProgram>( "./Shader/Sun.vert", "./Shader/Sun.frag" );
	sunSP->CompileAndLink( );
	
	/* Phong */
	phongSP = std::make_shared<PhongShaderProgram>( "./Shader/Phong.vert", "./Shader/Phong.frag" );
	phongSP->CompileAndLink( );
}

void Scene::SolarSystemCreate( void )
{
	root = std::make_shared<SunSolarSystemObject>( sphere, sunTexture, sunDistortion, sunSP );

	auto mercury = std::make_shared<PlanetSolarSystemObject>(
		2.0, 0.3, 360.0 / 58.65, 360.0 / 88.0, rotateOriginStart[0],
		sphere, mercuryTexture, phongSP );
	root->SubObjectAdd( mercury );

	if( showAllPlanets )
	{
		auto venus = std::make_shared<PlanetSolarSystemObject>(
			4.0, 0.4, 360.0 / -243.0208, 360.0 / 224.7, rotateOriginStart[1],
			sphere, venusTexture, phongSP );
		root->SubObjectAdd( venus );
	}
	
	auto earth = std::make_shared<PlanetSolarSystemObject>(
		6.0, 0.4, 360.0 / 0.99, 360.0 / 365.2, rotateOriginStart[2],
		sphere, earthTexture, phongSP );
	{
		auto moon = std::make_shared<PlanetSolarSystemObject>(
			2.0, 0.1, 0.0, 360.0 / 27.322, rotateOriginStart[3],
			sphere, moonTexture, phongSP );
		earth->SubObjectAdd( moon );
	}
	root->SubObjectAdd( earth );

	if( showAllPlanets )
	{
		auto mars = std::make_shared<PlanetSolarSystemObject>(
			8.0, 0.3, 360.0 / 1.025, 360.0 / 687.0, rotateOriginStart[4],
			sphere, marsTexture, phongSP );
		{
			auto phobos = std::make_shared<PlanetSolarSystemObject>(
				1.5, 0.01, 0.0, 360.0 / 0.3189, rotateOriginStart[5],
				this->phobos, phobosTexture, phongSP );
			mars->SubObjectAdd( phobos );

			auto deimos = std::make_shared<PlanetSolarSystemObject>(
				2.0, 0.01, 0.0, 360.0 / 1.2624, rotateOriginStart[6],
				this->deimos, deimosTexture, phongSP );
			mars->SubObjectAdd( deimos );
		}
		root->SubObjectAdd( mars );

		auto jupiter = std::make_shared<PlanetSolarSystemObject>(
			10.0, 0.8, 360.0 / 0.4125, 360.0 / 4331.0, rotateOriginStart[7],
			sphere, jupiterTexture, phongSP );
		root->SubObjectAdd( jupiter );

		auto saturn = std::make_shared<PlanetSolarSystemObject>(
			12.0, 0.8, 360.0 / 0.4458, 360.0 / 10747.0, rotateOriginStart[8],
			sphere, saturnTexture, phongSP );
		root->SubObjectAdd( saturn );

		auto uranus = std::make_shared<PlanetSolarSystemObject>(
			14.0, 0.6, 360.0 / -0.716, 360.0 / 30589.0, rotateOriginStart[9],
			sphere, uranusTexture, phongSP );
		root->SubObjectAdd( uranus );

		auto neptune = std::make_shared<PlanetSolarSystemObject>(
			16.0, 0.6, 360.0 / 0.67083, 360.0 / 59800.0, rotateOriginStart[10],
			sphere, neptuneTexture, phongSP );
		root->SubObjectAdd( neptune );
	}
}
