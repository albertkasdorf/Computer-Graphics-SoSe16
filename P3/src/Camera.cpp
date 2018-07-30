#include "Camera.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera( glm::vec3 position, glm::vec3 front, glm::vec3 up ) :
	position( position ),
	front( front ),
	up( up )
{
}

Camera::~Camera( void )
{
}

glm::mat4 Camera::ViewMatrixGet( GLdouble deltaTime )
{
	GLfloat speed = 5.0f * deltaTime;
	
	if( activeKeys[GLFW_KEY_W] )
	{
		position += front * speed;
	}
	if( activeKeys[GLFW_KEY_S] )
	{
		position -= front * speed;
	}
	if( activeKeys[GLFW_KEY_A] )
	{
		glm::vec3 right = glm::normalize( glm::cross( front, up ) );
		position -= right * speed;
	}
	if( activeKeys[GLFW_KEY_D] )
	{
		glm::vec3 right = glm::normalize( glm::cross( front, up ) );
		position += right * speed;
	}
	
	return glm::lookAt( position, position + front, up );
}

void Camera::KeyboardProcess( const int key, const int scancode, const int action, const int mods )
{
	if( action == GLFW_PRESS )
		activeKeys[key] = true;
	else if( action == GLFW_RELEASE )
		activeKeys[key] = false;
}
