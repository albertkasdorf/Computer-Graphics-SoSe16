#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include "Scene.h"

int width = 800, height = 600;
GLFWwindow* window = nullptr;
std::unique_ptr<IScene> scene;

int main( void )
{
	/* Initialize the library */
	if( !glfwInit( ) )
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow( width, height, "Computergrafik Praktikum 3", nullptr, nullptr );
	if( window == nullptr )
	{
		glfwTerminate( );
		return -1;
	}

	/* Initialize Callbacks */
	glfwSetFramebufferSizeCallback( window, []( GLFWwindow* window, int width, int height )
	{
		scene->Resized( width, height );
	} );
	glfwSetKeyCallback( window, []( GLFWwindow* window, int key, int scancode, int action, int mods )
	{
		if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
		{
			glfwSetWindowShouldClose( window, GL_TRUE );
		}
		scene->Keyboard( key, scancode, action, mods );
	} );
	glfwSetScrollCallback( window, []( GLFWwindow* window, double xoffset, double yoffset )
	{
		scene->MouseScroll( xoffset, yoffset );
	} );
	glfwSetCursorPosCallback( window, []( GLFWwindow* window, double xpos, double ypos )
	{
		scene->MouseMovement( xpos, ypos );
	} );
	//glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

	/* Make the window's context current */
	glfwMakeContextCurrent( window );

	/* Initialize GLEW */
	glewExperimental = true;
	GLenum err = glewInit( );
	if( GLEW_OK != err )
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cout << "Error: " << glewGetErrorString( err ) << std::endl;

		glfwTerminate( );
		return -1;
	}

	/* Create and initialize scene*/
	glfwGetFramebufferSize( window, &width, &height );
	scene = std::make_unique<Scene>( );
	scene->Initialize( width, height );

	/* Loop until the user closes the window */
	while( !glfwWindowShouldClose( window ) )
	{
		/* Render here */
		scene->Render( );

		/* Swap front and back buffers */
		glfwSwapBuffers( window );

		/* Poll for and process events */
		glfwPollEvents( );
	}

	/* Cleanup */
	scene->Terminate( );
	scene.reset( nullptr );

	glfwTerminate( );

	return 0;
}

