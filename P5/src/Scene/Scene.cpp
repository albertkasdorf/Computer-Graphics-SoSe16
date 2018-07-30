#include "Scene.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "../StopWatch.h"
#include "../Renderer/CoordinateAxis.h"
#include "../Model.h"
#include "../Camera.h"
#include "../ElapsedTime.h"
#include "../FPSCounter.h"
#include "../GL/ShaderProgram.h"
#include "../UI/PropertyWindow.h"

namespace Scene
{

Scene::Scene( void ) :
	clearColor( 0.2, 0.3, 0.3, 1.0 )
{
}

Scene::~Scene( void )
{
}

void Scene::Initialize( const GLint width, const GLint height )
{
	StopWatch sw( "Scene::Initialize" );
	GLStringPrint( );
	BufferObjectsCreate( );
	ShaderCreate( );
	LoadAnimation( );

	camera = std::make_unique<Camera>( );
	camera->PositionSet( 0.0, 50.0, 200.0 );
	camera->ViewDirectionSet( 0.0, 0.0, -1.0 );

	elapsedTime = std::make_unique<ElapsedTime>( );
	fpsCounter = std::make_unique<FPSCounter>( );

	coordinateAxis = std::make_unique<Renderer::CoordinateAxis>( );
	coordinateAxis->Initialize( );

	propertyWindow = std::make_unique<UI::PropertyWindow>( );
	propertyWindow->Initialize( width, height );

	this->Inspect( propertyWindow.get( ) );
	playerModel->Inspect( propertyWindow.get( ) );

	//glEnable( GL_CULL_FACE );

	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );

	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	// Activate Wireframe Mode
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	glClearDepth( 1.0 );

	FramebufferSizeCallback( width, height );
}

void Scene::Terminate( void )
{
	propertyWindow->Terminate( );
}

void Scene::FramebufferSizeCallback( const GLint width, const GLint height )
{
	camera->FramebufferSizeCallback( width, height );
	propertyWindow->FramebufferSizeCallback( width, height );
}

void Scene::KeyCallback( const int key, const int scancode, const int action, const int mods )
{
	camera->KeyCallback( key, scancode, action, mods );
	propertyWindow->KeyCallback( key, scancode, action, mods );

	if( key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT) )
	{
		//playerModel->SampleSet( 1 );
		//skeleton->FrameSet( 1 );
	}
	else if( key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT) )
	{
		//playerModel->SampleSet( -1 );
		//skeleton->FrameSet( -1 );
	}
}

void Scene::CursorPosCallback( const double xpos, const double ypos )
{
	bool handled = false;

	if( !handled )
		handled = propertyWindow->CursorPosCallback( xpos, ypos );

	if( !handled )
		camera->CursorPosCallback( xpos, ypos );
}

void Scene::ScrollCallback( const double xoffset, const double yoffset )
{
	bool handled = false;

	if( !handled )
		handled = propertyWindow->ScrollCallback( xoffset, yoffset );
}

void Scene::MouseButtonCallback( const int button, const int action, const int mods )
{
	bool handled = false;

	if( !handled )
		handled = propertyWindow->MouseButtonCallback( button, action, mods );

	if( !handled )
		camera->MouseButtonCallback( button, action, mods );
}

void Scene::CharCallback( const unsigned int codepoint )
{
	propertyWindow->CharCallback( codepoint );
}

void Scene::Render( void )
{
	// Update
	elapsedTime->Update( );
	camera->Update( elapsedTime.get( ) );
	playerModel->Update( elapsedTime.get( ) );
	fpsCounter->Update( elapsedTime.get( ) );

	// Clear buffer to set color and alpha
	glClearColor( clearColor.r, clearColor.g, clearColor.b, clearColor.a );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	projection = camera->ViewToProjectionMatrixGet( );
	view = camera->WorldToViewMatrixGet( );
	model = glm::mat4( 1.0 );

	model = glm::rotate( model, glm::radians<float>( 90.0 ), glm::vec3( -1.0, 0.0, 0.0 ) );

	//floor->Render( projection, view, model );
	coordinateAxis->Render( projection, view, model );
	playerModel->Render( projection, view, model );

	propertyWindow->Render( );
}

void Scene::GLStringPrint( void )
{
	StopWatch sw( "Scene::GLStringPrint" );
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

	GLint glIntData = 0;
	glGetIntegerv( GL_MAX_UNIFORM_BLOCK_SIZE, &glIntData );
	std::cout << "GL_MAX_UNIFORM_BLOCK_SIZE=" << glIntData << " Bytes" << std::endl;

	glGetIntegerv( GL_MAX_UNIFORM_LOCATIONS, &glIntData );
	std::cout << "GL_MAX_UNIFORM_LOCATIONS=" << glIntData << std::endl;
	

	std::cout << std::endl;
}

void Scene::LoadAnimation( void )
{
	StopWatch sw( "Scene::LoadAnimation" );

	/**/
	playerModel = std::make_unique<Renderer::Model>( );
	//playerModel->Initialize( "./Assets/Blender/untitled6.dae", {} );
	playerModel->Initialize( "./Assets/BobLampClean/boblampclean.md5mesh", {} );
	//playerModel->Initialize( "./Assets/Blender/NormalPose.dae", {} );
	/**/

	/**
	playerModel = std::make_unique<Renderer::Model>( );
	playerModel->Initialize(
		"./Assets/imp/imp.md5mesh",
		{
			//"./Assets/imp/a2_elevator.md5anim",
			//"./Assets/imp/cabinet_leap.md5anim",
			//"./Assets/imp/ceilingimp.md5anim",
			//"./Assets/imp/ceilingimpidle.md5anim",
			//"./Assets/imp/crouched_range1.md5anim",
			//"./Assets/imp/d1corner.md5anim",
			//"./Assets/imp/d1corneridle.md5anim",
			"./Assets/imp/evade_left.md5anim",
			"./Assets/imp/evade_right.md5anim",
			"./Assets/imp/evade_left_on4.md5anim",
			"./Assets/imp/evade_right_on4.md5anim",
			"./Assets/imp/faster_walk.md5anim",
			//"./Assets/imp/hangonceiling.md5anim",
			"./Assets/imp/idle1.md5anim",
			//"./Assets/imp/idlecages.md5anim",
			//"./Assets/imp/idlecolumn.md5anim",
			//"./Assets/imp/imp1doorbreak.md5anim",
			//"./Assets/imp/imp1wallcrawl.md5anim",
			//"./Assets/imp/imp2doorbreak.md5anim",
			//"./Assets/imp/imp2wallcrawl.md5anim",
			"./Assets/imp/impcyberwakeup.md5anim",
			"./Assets/imp/imptankidle.md5anim",
			"./Assets/imp/imptanktwitch.md5anim",
			//"./Assets/imp/initial.md5anim",
			//"./Assets/imp/jump_end.md5anim",
			//"./Assets/imp/jump_loop.md5anim",
			//"./Assets/imp/jump_loop2.md5anim",
			//"./Assets/imp/jump_loop3.md5anim",
			//"./Assets/imp/jump_start.md5anim",
			//"./Assets/imp/leapdowncages.md5anim",
			//"./Assets/imp/leapdowncolumn.md5anim",
			//"./Assets/imp/offwall.md5anim",
			//"./Assets/imp/offwallidle.md5anim",
			"./Assets/imp/on4_idle.md5anim",
			"./Assets/imp/on4_melee1.md5anim",
			"./Assets/imp/on4_melee2.md5anim",
			"./Assets/imp/on4_sight.md5anim",
			"./Assets/imp/outofhole.md5anim",
			//"./Assets/imp/outofholeidle.md5anim",
			//"./Assets/imp/pain_chest.md5anim",
			//"./Assets/imp/pain_chest_on4.md5anim",
			//"./Assets/imp/pain_head.md5anim",
			//"./Assets/imp/pain_head_on4.md5anim",
			//"./Assets/imp/pain_luparm.md5anim",
			//"./Assets/imp/pain_luparm_on4.md5anim",
			//"./Assets/imp/pain_ruparm.md5anim",
			//"./Assets/imp/pain_ruparm_on4.md5anim",
			//"./Assets/imp/pain3.md5anim",
			"./Assets/imp/range1.md5anim",
			"./Assets/imp/range3.md5anim",
			"./Assets/imp/range4.md5anim",
			"./Assets/imp/scurry_leap.md5anim",
			"./Assets/imp/scurry2.md5anim",
			"./Assets/imp/severedidle.md5anim",
			"./Assets/imp/severedwake.md5anim",
			"./Assets/imp/sight.md5anim",
			"./Assets/imp/sight2.md5anim",
			"./Assets/imp/slash1.md5anim",
			"./Assets/imp/slash2.md5anim",
			"./Assets/imp/slash3.md5anim",
			"./Assets/imp/teleportin.md5anim",
			"./Assets/imp/turret_attack1.md5anim",
			"./Assets/imp/turret_idle.md5anim",
			"./Assets/imp/walk1.md5anim",
		} );
	/**/

	/**
	playerModel = std::make_unique<Renderer::Model>( );
	playerModel->Initialize(
		"./Assets/hellknight/hellknight.md5mesh",
		{
			"./Assets/hellknight/headpain.md5anim",
			"./Assets/hellknight/idle2.md5anim",
			//"./Assets/hellknight/ik_pose.md5anim",
			"./Assets/hellknight/initial.md5anim",
			"./Assets/hellknight/leftslash.md5anim",
			"./Assets/hellknight/pain_luparm.md5anim",
			"./Assets/hellknight/pain_ruparm.md5anim",
			"./Assets/hellknight/pain1.md5anim",
			"./Assets/hellknight/range_attack2.md5anim",
			"./Assets/hellknight/roar1.md5anim",
			"./Assets/hellknight/stand.md5anim",
			"./Assets/hellknight/turret_attack.md5anim",
			"./Assets/hellknight/walk7.md5anim",
			"./Assets/hellknight/walk7_left.md5anim",
			"./Assets/hellknight/attack2.md5anim",
			"./Assets/hellknight/attack3.md5anim",
			"./Assets/hellknight/chest.md5anim",
		} );
	/**/

	/**
	playerModel = std::make_unique<Renderer::Model>( );
	playerModel->Initialize(
		"./Assets/cyberdemon/cyberdemon.md5mesh",
		{
			"./Assets/cyberdemon/pain_big1.md5anim",
			"./Assets/cyberdemon/sight.md5anim",
			"./Assets/cyberdemon/turret3.md5anim",
			"./Assets/cyberdemon/twoshot.md5anim",
			"./Assets/cyberdemon/walk3.md5anim",
			"./Assets/cyberdemon/idle.md5anim",
			"./Assets/cyberdemon/oneshot.md5anim"
		} );
	/**/

	/**
	playerModel = std::make_unique<Renderer::Model>( );
	playerModel->Initialize(
		"./Assets/guardian/guardian.md5mesh",
		{
			//"./Assets/guardian/walk_attack_R.md5anim",
			//"./Assets/guardian/walk1_L.md5anim",
			//"./Assets/guardian/walk1_R.md5anim",
			"./Assets/guardian/death.md5anim",
			//"./Assets/guardian/death_pose.md5anim",
			//"./Assets/guardian/guardian_intro.md5anim",
			"./Assets/guardian/attack1.md5anim",
			"./Assets/guardian/attack2.md5anim",
			"./Assets/guardian/attack3.md5anim",
			"./Assets/guardian/idle1.md5anim",
			"./Assets/guardian/idle2.md5anim",
			//"./Assets/guardian/pain_head.md5anim",
			"./Assets/guardian/range_attack1.md5anim",
			//"./Assets/guardian/run_L.md5anim",
			//"./Assets/guardian/run_L_headbutt.md5anim",
			//"./Assets/guardian/run_R.md5anim",
			//"./Assets/guardian/run_R_headbutt.md5anim",
			"./Assets/guardian/sight1.md5anim",
			"./Assets/guardian/walk_attack_L.md5anim",
		} );
	/**/

	return;
}

void Scene::BufferObjectsCreate( void )
{

}

void Scene::TextureCreate( void )
{
}

void Scene::ShaderCreate( void )
{
	defaultSP = std::make_shared<GL::ShaderProgram>(
		"./Shader/Default.vert", "./Shader/Default.frag" );
	defaultSP->CompileAndLink( );
}

void Scene::Inspect( UI::PropertyWindow * const propertyWindow )
{
	propertyWindow->ReadWriteVariableAdd(
		"Scene::clearColor", "group=Scene label='Background'",
		&clearColor );

	fpsCounter->Inspect( propertyWindow, "Scene" );
}

}
