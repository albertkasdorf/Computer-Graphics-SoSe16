#include "Camera.h"
#include <GLFW/glfw3.h>
#include <glm/gtx/transform.hpp>
#include "ElapsedTime.h"


Camera::Camera( void )
	:
	position( 0.0, 0.0, 0.0 )
	, viewDirection( 0.0, 0.0, -1.0 )
	, up( 0.0, 1.0, 0.0 )
	, movementSpeed( 100.0f )
	, viewSpeed( 1.f )
	, mouseSensitivity( 0.002f )
	, fovy( 45.0 )
	, aspect( 1.0 )
	, znear( 0.1 )
	, zfar( 1000.0 )
{
}

Camera::~Camera( void )
{
}

void Camera::PositionSet( float x, float y, float z )
{
	position = glm::vec3( x, y, z );
}

void Camera::ViewDirectionSet( float x, float y, float z )
{
	viewDirection = glm::normalize( glm::vec3( x, y, z ) );
}

void Camera::Update( const ElapsedTime * const elapsedTime )
{
	const float velocity = movementSpeed * elapsedTime->Get( );
	const float velocityView = viewSpeed * elapsedTime->Get( );

	if( keyboardKeys[GLFW_KEY_W] )
	{
		position += velocity * viewDirection;
	}
	if( keyboardKeys[GLFW_KEY_S] )
	{
		position -= velocity * viewDirection;
	}
	if( keyboardKeys[GLFW_KEY_A] )
	{
		strafeDirection = glm::normalize( glm::cross( viewDirection, up ) );
		position -= velocity * strafeDirection;
	}
	if( keyboardKeys[GLFW_KEY_D] )
	{
		strafeDirection = glm::normalize( glm::cross( viewDirection, up ) );
		position += velocity * strafeDirection;
	}
	if( keyboardKeys[GLFW_KEY_R] )
	{
		position += velocity * up;
	}
	if( keyboardKeys[GLFW_KEY_F] )
	{
		position -= velocity * up;
	}
	if( keyboardKeys[GLFW_KEY_LEFT] )
	{
		strafeDirection = glm::normalize( glm::cross( viewDirection, up ) );

		const glm::mat4 rotator =
			glm::rotate( +velocityView, up ) *
			glm::rotate( 0.f, strafeDirection );

		viewDirection = glm::mat3( rotator ) * viewDirection;
	}
	if( keyboardKeys[GLFW_KEY_RIGHT] )
	{
		strafeDirection = glm::normalize( glm::cross( viewDirection, up ) );

		const glm::mat4 rotator =
			glm::rotate( -velocityView, up ) *
			glm::rotate( 0.f, strafeDirection );

		viewDirection = glm::mat3( rotator ) * viewDirection;
	}
	if( keyboardKeys[GLFW_KEY_UP] )
	{
		strafeDirection = glm::normalize( glm::cross( viewDirection, up ) );

		const glm::mat4 rotator =
			glm::rotate( 0.f, up ) *
			glm::rotate( +velocityView, strafeDirection );

		viewDirection = glm::mat3( rotator ) * viewDirection;
	}
	if( keyboardKeys[GLFW_KEY_DOWN] )
	{
		strafeDirection = glm::normalize( glm::cross( viewDirection, up ) );

		const glm::mat4 rotator =
			glm::rotate( 0.f, up ) *
			glm::rotate( -velocityView, strafeDirection );

		viewDirection = glm::mat3( rotator ) * viewDirection;
	}
}

glm::mat4 Camera::WorldToViewMatrixGet( void ) const
{
	return glm::lookAt( position, position + viewDirection, up );
}

glm::mat4 Camera::ViewToProjectionMatrixGet( void ) const
{
	return glm::perspective( glm::radians( fovy ), aspect, znear, zfar );
}

void Camera::CursorPosCallback( const double xpos, const double ypos )
{
	const glm::vec2 newMousePosition( xpos, ypos );
	const glm::vec2 mouseDelta = newMousePosition - oldMousePosition;

	if( mouseButtons[GLFW_MOUSE_BUTTON_LEFT] && glm::length( mouseDelta ) < 50.0f )
	{
		strafeDirection = glm::normalize( glm::cross( viewDirection, up ) );

		const glm::mat4 rotator =
			glm::rotate( -mouseDelta.x * mouseSensitivity, up ) *
			glm::rotate( -mouseDelta.y * mouseSensitivity, strafeDirection );

		viewDirection = glm::mat3( rotator ) * viewDirection;
	}

	oldMousePosition = newMousePosition;
}

void Camera::MouseButtonCallback( const int button, const int action, const int mods )
{
	if( action == GLFW_PRESS || action == GLFW_REPEAT )
		mouseButtons[button] = true;
	else if( action == GLFW_RELEASE )
		mouseButtons[button] = false;
}

void Camera::KeyCallback( const int key, const int scancode, const int action, const int mods )
{
	if( action == GLFW_PRESS || action == GLFW_REPEAT )
		keyboardKeys[key] = true;
	else if( action == GLFW_RELEASE )
		keyboardKeys[key] = false;
}

void Camera::FramebufferSizeCallback( const double width, const double height )
{
	aspect = width / height;

	glViewport( 0, 0, static_cast<GLsizei>( width ), static_cast<GLsizei>( height ) );
}
