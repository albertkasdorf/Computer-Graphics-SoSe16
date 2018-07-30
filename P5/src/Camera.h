#pragma once
#include <map>
#include <glm/glm.hpp>

class ElapsedTime;

class Camera
{
public:
	Camera( void );
	~Camera( void );

private:
	glm::vec3 position;

	glm::vec3 viewDirection;
	glm::vec3 strafeDirection;
	glm::vec3 up;

	glm::vec2 oldMousePosition;

	std::map<int, bool> keyboardKeys;
	std::map<int, bool> mouseButtons;

	/* Settings */
	float movementSpeed;
	float mouseSensitivity;
	float viewSpeed;

	/* Projection */
	double fovy, aspect, znear, zfar;

public:
	void PositionSet( float x, float y, float z );
	void ViewDirectionSet( float x, float y, float z );
	//void Reset( void );
	//void AxisInvert( bool x, bool y );

	void Update( const ElapsedTime * const elapsedTime );

	glm::mat4 WorldToViewMatrixGet( void ) const;
	glm::mat4 ViewToProjectionMatrixGet( void ) const;

	void MouseButtonCallback( const int button, const int action, const int mods );
	void CursorPosCallback( const double xpos, const double ypos );
	void KeyCallback( const int key, const int scancode, const int action, const int mods );
	void FramebufferSizeCallback( const double width, const double height );
};
