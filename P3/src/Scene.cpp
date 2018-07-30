#include "Scene.h"

#include <random>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "File.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "ModelMatrix.h"
#include "VertexArrayObject.h"
#include "Camera.h"

Scene::Scene( void ) :
	showAllPlanets( GL_TRUE ),
	fixStartOrigin( GL_FALSE ),
	movementSpeed( 0.1 ),
	animate( GL_TRUE ),
	topViewAktiv( GL_FALSE )
{
}

Scene::~Scene( void )
{
}

void Scene::Initialize( const GLint width, const GLint height )
{
	model = std::make_unique<ModelMatrix>( );
	camera = std::make_unique<Camera>(
		glm::vec3( 0.0, 0.0, 12.0 ),
		glm::vec3( 0.0, 0.0, -1.0 ),
		glm::vec3( 0.0, 1.0, 0.0 ) );

	CreateGeometry( );
	CreateBufferObjects( );
	CreateTexture( );
	CreateShader( );

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
	GLdouble currentFrame = glfwGetTime( );
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	// Clear buffer to set color and alpha
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// setup projection matrix
	if( perspective )
	{
		projection = glm::perspective(
			glm::radians( fov ),
			aspectRatio,
			0.1, 1000.0 );
	}
	else
	{
		projection = glm::ortho( -3.0, 3.0, -3.0, 3.0, 20.0, -20.0 );
	}

	/* Camera */
	view = camera->ViewMatrixGet( deltaTime );
	
	/* Solar System */
	glUseProgram( modelSP->ProgramGet( ) );
	glUniformMatrix4fv( modelProjLoc, 1, GL_FALSE, glm::value_ptr( projection ) );
	glUniformMatrix4fv( modelViewLoc, 1, GL_FALSE, glm::value_ptr( view ) );

	// Reset Model
	model->LoadIdentity( );
	
	// TopView
	if( topViewAktiv )
	{
		model->Rotate( 90.0, 1.0, 0.0, 0.0 );
	}
	
	//----
	GLdouble time = glfwGetTime( );
	if( animate )
	{
		timex = time * movementSpeed;
		//std::cout << "Time=" << time << " TimeX=" << timex << std::endl;
	}

	RenderSun( timex );
	RenderMercury( timex );
	if( showAllPlanets )
	{
		RenderVenus( timex );
	}
	RenderEarthMoon( timex );
	if( showAllPlanets )
	{
		RenderMars( timex );
		RenderJupiter( timex );
		RenderSaturn( timex );
		RenderUranus( timex );
		RenderNeptune( timex );
	}
}

void Scene::RenderSun( GLdouble time )
{
	// http://nssdc.gsfc.nasa.gov/planetary/factsheet/
	const GLdouble diameter = 1.0;
	const GLdouble rotateSelf = 360.0 / 25.38;	// days

	model->PushMatrix( );
	{
		model->Translate( 0.0, 0.0, 0.0 );
		model->Rotate( rotateSelf * time, 0.0, 1.0, 0.0 );
		model->Scale( diameter );

		glUniformMatrix4fv( modelModelLoc, 1, GL_FALSE, model->PtrGet( ) );
		glBindTexture( GL_TEXTURE_2D, sunTexture->TextureIdGet( ) );

		sphere->Render( );
	}
	model->PopMatrix( );
}

void Scene::RenderMercury( GLdouble time )
{
	const GLdouble diameter = 0.3;
	const GLdouble rotateSelf = 360.0 / 58.65;	// days
	const GLdouble rotateOrigin = 360.0 / 88.0;	// days

	model->PushMatrix( );
	{
		model->Rotate( rotateOrigin * time + rotateOriginStart[0], 0.0, 1.0, 0.0 );

		model->Translate( 2.0, 0.0, 0.0 );
		model->Rotate( rotateSelf * time, 0.0, 1.0, 0.0 );
		model->Scale( diameter );

		glUniformMatrix4fv( modelModelLoc, 1, GL_FALSE, model->PtrGet( ) );
		glBindTexture( GL_TEXTURE_2D, mercuryTexture->TextureIdGet( ) );

		sphere->Render( );
	}
	model->PopMatrix( );
}

void Scene::RenderVenus( GLdouble delta )
{
	const GLdouble diameter = 0.4;
	const GLdouble rotateSelf = 360.0 / -243.0208;
	const GLdouble rotateOrigin = 360.0 / 224.7;

	model->PushMatrix( );
	{
		model->Rotate( rotateOrigin * delta + rotateOriginStart[1], 0.0, 1.0, 0.0 );

		model->Translate( 4.0, 0.0, 0.0 );
		model->Rotate( rotateSelf * delta, 0.0, 1.0, 0.0 );
		model->Scale( diameter );

		glUniformMatrix4fv( modelModelLoc, 1, GL_FALSE, model->PtrGet( ) );
		glBindTexture( GL_TEXTURE_2D, venusTexture->TextureIdGet( ) );

		sphere->Render( );
	}
	model->PopMatrix( );
}

void Scene::RenderEarthMoon( GLdouble delta )
{
	const GLdouble earthDiameter = 0.4;
	const GLdouble earthRotateSelf = 360.0 / 0.99;
	const GLdouble earthRotateOrigin = 360.0 / 365.2;

	const GLdouble moonDiameter = 0.1;
	const GLdouble moonRotateSelf = 0.0;
	const GLdouble moonRotateOrigin = 360.0 / 27.322;

	model->PushMatrix( );
	{
		/* Rotation um die Y Achse im Ursprung*/
		model->Rotate( earthRotateOrigin * delta + rotateOriginStart[2], 0.0, 1.0, 0.0 );

		/* SRT um sich selbst */
		model->Translate( 6.0, 0.0, 0.0 );
		model->Rotate( earthRotateSelf * delta, 0.0, 1.0, 0.0 );
		model->Scale( earthDiameter );

		glUniformMatrix4fv( modelModelLoc, 1, GL_FALSE, model->PtrGet( ) );
		glBindTexture( GL_TEXTURE_2D, earthTexture->TextureIdGet( ) );

		sphere->Render( );

		model->PushMatrix( );
		{
			model->Rotate( moonRotateOrigin * delta + rotateOriginStart[3], 0.0, 1.0, 0.0 );

			model->Translate( 2.0, 0.0, 0.0 );
			model->Rotate( moonRotateSelf * delta, 0.0, 1.0, 0.0 );
			model->Scale( moonDiameter );

			glUniformMatrix4fv( modelModelLoc, 1, GL_FALSE, model->PtrGet( ) );
			glBindTexture( GL_TEXTURE_2D, moonTexture->TextureIdGet( ) );

			sphere->Render( );
		}
		model->PopMatrix( );
	}
	model->PopMatrix( );
}

void Scene::RenderMars( GLdouble delta )
{
	const GLdouble marsDiameter = 0.3;
	const GLdouble marsRotateSelf = 360.0 / 1.025;
	const GLdouble marsRotateOrigin = 360.0 / 687.0;

	const GLdouble phobosDiameter = 0.01;
	const GLdouble phobosRotateSelf = 0.0;	// Gebundene Rotation
	const GLdouble phobosRotateOrigin = 360.0 / 0.3189;

	const GLdouble deimosDiameter = 0.01;
	const GLdouble deimosRotateSelf = 0.0;
	const GLdouble deimosRotateOrigin = 360.0 / 1.2624;

	model->PushMatrix( );
	{
		model->Rotate( marsRotateOrigin * delta + rotateOriginStart[4], 0.0, 1.0, 0.0 );

		model->Translate( 8.0, 0.0, 0.0 );
		model->Rotate( marsRotateSelf * delta, 0.0, 1.0, 0.0 );
		model->Scale( marsDiameter );

		glUniformMatrix4fv( modelModelLoc, 1, GL_FALSE, model->PtrGet( ) );
		glBindTexture( GL_TEXTURE_2D, marsTexture->TextureIdGet( ) );

		sphere->Render( );

		model->PushMatrix( );
		{
			model->Rotate( phobosRotateOrigin * delta + rotateOriginStart[5], 0.0, 1.0, 0.0 );

			model->Translate( 1.5, 0.0, 0.0 );
			model->Rotate( phobosRotateSelf * delta, 0.0, 1.0, 0.0 );
			model->Scale( phobosDiameter );

			glUniformMatrix4fv( modelModelLoc, 1, GL_FALSE, model->PtrGet( ) );
			glBindTexture( GL_TEXTURE_2D, phobosTexture->TextureIdGet( ) );

			phobos->Render( );
		}
		model->PopMatrix( );

		model->PushMatrix( );
		{
			model->Rotate( deimosRotateOrigin * delta + rotateOriginStart[6], 0.0, 1.0, 0.0 );

			model->Translate( 2.0, 0.0, 0.0 );
			model->Rotate( deimosRotateSelf * delta, 0.0, 1.0, 0.0 );
			model->Scale( deimosDiameter );

			glUniformMatrix4fv( modelModelLoc, 1, GL_FALSE, model->PtrGet( ) );
			glBindTexture( GL_TEXTURE_2D, deimosTexture->TextureIdGet( ) );

			deimos->Render( );
		}
		model->PopMatrix( );
	}
	model->PopMatrix( );
}

void Scene::RenderJupiter( GLdouble delta )
{
	const GLdouble diameter = 0.8;
	const GLdouble rotateSelf = 360.0 / 0.4125;
	const GLdouble rotateOrigin = 360.0 / 4331.0;
	
	model->PushMatrix( );
	{
		model->Rotate( rotateOrigin * delta + rotateOriginStart[7], 0.0, 1.0, 0.0 );

		model->Translate( 10.0, 0.0, 0.0 );
		model->Rotate( rotateSelf * delta, 0.0, 1.0, 0.0 );
		model->Scale( diameter );

		glUniformMatrix4fv( modelModelLoc, 1, GL_FALSE, model->PtrGet( ) );
		glBindTexture( GL_TEXTURE_2D, jupiterTexture->TextureIdGet( ) );

		sphere->Render( );
	}
	model->PopMatrix( );
}

void Scene::RenderSaturn( GLdouble delta )
{
	const GLdouble diameter = 0.8;
	const GLdouble rotateSelf = 360.0 / 0.4458;
	const GLdouble rotateOrigin = 360.0 / 10747.0;

	model->PushMatrix( );
	{
		model->Rotate( rotateOrigin * delta + rotateOriginStart[8], 0.0, 1.0, 0.0 );

		model->Translate( 12.0, 0.0, 0.0 );
		model->Rotate( rotateSelf * delta, 0.0, 1.0, 0.0 );
		model->Scale( diameter );

		glUniformMatrix4fv( modelModelLoc, 1, GL_FALSE, model->PtrGet( ) );
		glBindTexture( GL_TEXTURE_2D, saturnTexture->TextureIdGet( ) );

		sphere->Render( );
	}
	model->PopMatrix( );
}

void Scene::RenderUranus( GLdouble delta )
{
	const GLdouble diameter = 0.6;
	const GLdouble rotateSelf = 360.0 / -0.716;
	const GLdouble rotateOrigin = 360.0 / 30589.0;
	
	model->PushMatrix( );
	{
		model->Rotate( rotateOrigin * delta + rotateOriginStart[9], 0.0, 1.0, 0.0 );

		model->Translate( 14.0, 0.0, 0.0 );
		model->Rotate( rotateSelf * delta, 0.0, 1.0, 0.0 );
		model->Scale( diameter );

		glUniformMatrix4fv( modelModelLoc, 1, GL_FALSE, model->PtrGet( ) );
		glBindTexture( GL_TEXTURE_2D, uranusTexture->TextureIdGet( ) );

		sphere->Render( );
	}
	model->PopMatrix( );
}

void Scene::RenderNeptune( GLdouble delta )
{
	const GLdouble diameter = 0.6;
	const GLdouble rotateSelf = 360.0 / 0.67083;
	const GLdouble rotateOrigin = 360.0 / 59800.0;

	model->PushMatrix( );
	{
		model->Rotate( rotateOrigin * delta + rotateOriginStart[10], 0.0, 1.0, 0.0 );

		model->Translate( 16.0, 0.0, 0.0 );
		model->Rotate( rotateSelf * delta, 0.0, 1.0, 0.0 );
		model->Scale( diameter );

		glUniformMatrix4fv( modelModelLoc, 1, GL_FALSE, model->PtrGet( ) );
		glBindTexture( GL_TEXTURE_2D, neptuneTexture->TextureIdGet( ) );

		sphere->Render( );
	}
	model->PopMatrix( );
}

void Scene::CreateGeometry( void )
{
	/* Coord */
	/*coordVertexData.assign( {
		0.f, 0.f, 0.f,	100.f, 0.f, 0.f,
		0.f, 0.f, 0.f,	0.f, 100.f, 0.f,
		0.f, 0.f, 0.f,	0.f, 0.f, 100.f,
	} );
	coordColorData.assign( {
		1.f, 0.f, 0.f, 1.f,	1.f, 0.f, 0.f, 1.f,
		0.f, 1.f, 0.f, 1.f,	0.f, 1.f, 0.f, 1.f,
		0.f, 0.f, 1.f, 1.f,	0.f, 0.f, 1.f, 1.f
	} );*/
}

void Scene::CreateBufferObjects( void )
{
	// sphere_high, sphere_low

	/* Solar System */
	sphere = std::make_unique<VertexArrayObject>( "./Assets/sphere_low.obj" );
	sphere->Create( );

	if( showAllPlanets )
	{
		phobos = std::make_unique<VertexArrayObject>( "./Assets/Phobos.3ds" );
		phobos->Create( );

		deimos = std::make_unique<VertexArrayObject>( "./Assets/Deimos.3ds" );
		deimos->Create( );
	}
}

void Scene::CreateTexture( void )
{
	/* Solar System */
	sunTexture = std::make_unique<Texture>( "./Assets/Sun_ColorMap.jpg" );
	sunTexture->TextureCreate( );

	mercuryTexture = std::make_unique<Texture>( "./Assets/Mercury_ColorMap.jpg" );
	mercuryTexture->TextureCreate( );

	if( showAllPlanets )
	{
		venusTexture = std::make_unique<Texture>( "./Assets/Venus_ColorMap.jpg" );
		venusTexture->TextureCreate( );
	}

	earthTexture = std::make_unique<Texture>( "./Assets/Earth_ColorMap.jpg" );
	earthTexture->TextureCreate( );

	moonTexture = std::make_unique<Texture>( "./Assets/Moon_ColorMap.jpg" );
	moonTexture->TextureCreate( );
	
	if( showAllPlanets )
	{
		marsTexture = std::make_unique<Texture>( "./Assets/Mars_ColorMap1.jpg" );
		marsTexture->TextureCreate( );

		phobosTexture = std::make_unique<Texture>( "./Assets/Phobos_ColorMap.png" );
		phobosTexture->TextureCreate( );

		deimosTexture = std::make_unique<Texture>( "./Assets/Deimos_ColorMap.jpg" );
		deimosTexture->TextureCreate( );

		jupiterTexture = std::make_unique<Texture>( "./Assets/Jupiter_ColorMap1.jpg" );
		jupiterTexture->TextureCreate( );

		saturnTexture = std::make_unique<Texture>( "./Assets/Saturn_ColorMap.jpg" );
		saturnTexture->TextureCreate( );

		uranusTexture = std::make_unique<Texture>( "./Assets/Uranus_ColorMap.jpg" );
		uranusTexture->TextureCreate( );

		neptuneTexture = std::make_unique<Texture>( "./Assets/Neptune_ColorMap.jpg" );
		neptuneTexture->TextureCreate( );
	}
}

void Scene::CreateShader( void )
{
	/* Model */
	modelSP = std::make_unique<ShaderProgram>(
		"./Shader/Model.vert",
		"./Shader/Model.frag" );
	modelSP->CompileAndLink( );

	modelProjLoc = modelSP->UniformLocationGet( "projection" );
	modelViewLoc = modelSP->UniformLocationGet( "view" );
	modelModelLoc = modelSP->UniformLocationGet( "model" );
}
