#pragma once
#include <map>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>

class Camera
{
public:
	Camera( glm::vec3 position, glm::vec3 front, glm::vec3 up );
	~Camera( void );

private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	std::map<int, bool> activeKeys;

public:
	glm::mat4 ViewMatrixGet( GLdouble deltaTime );

	void FrontViewSet( void );
	void TopViewSet( void );

	void KeyboardProcess( const int key, const int scancode, const int action, const int mods );
};

