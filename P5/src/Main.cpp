#include "StopWatch.h"
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include "Scene/Manager.h"

int width = 800, height = 600;
GLFWwindow* window = nullptr;
std::unique_ptr<Scene::IScene> scene;

static void APIENTRY glDebugOutput(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar *message,
	const void *userParam )
{
	// ignore non-significant error/warning codes
	if( id == 131169 || id == 131185 || id == 131218 || id == 131204 )
		return;

	if( type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR )
		return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch( source )
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch( type )
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch( severity )
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}

int main( void )
{
	int width = 800, height = 600;
	GLFWwindow* window = nullptr;

	/* Initialize the library */
	{
		StopWatch sw( "glfwInit" );
		if( !glfwInit( ) )
			return -1;

		//glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
		//glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
		//glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
		//glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
		glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE );
	}

	/* Create a windowed mode window and its OpenGL context */
	{
		StopWatch sw( "glfwCreateWindow" );
		window = glfwCreateWindow( width, height, "Computergrafik Abschlussprojekt", nullptr, nullptr );
		if( window == nullptr )
		{
			glfwTerminate( );
			return -1;
		}
	}

	/* Initialize Callbacks */
	glfwSetFramebufferSizeCallback( window, []( GLFWwindow* window, int width, int height )
	{
		scene->FramebufferSizeCallback( width, height );
	} );
	glfwSetKeyCallback( window, []( GLFWwindow* window, int key, int scancode, int action, int mods )
	{
		if( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
		{
			glfwSetWindowShouldClose( window, GL_TRUE );
		}
		scene->KeyCallback( key, scancode, action, mods );
	} );
	glfwSetScrollCallback( window, []( GLFWwindow* window, double xoffset, double yoffset )
	{
		scene->ScrollCallback( xoffset, yoffset );
	} );
	glfwSetCursorPosCallback( window, []( GLFWwindow* window, double xpos, double ypos )
	{
		scene->CursorPosCallback( xpos, ypos );
	} );
	glfwSetMouseButtonCallback( window, []( GLFWwindow* window, int button, int action, int mods )
	{
		scene->MouseButtonCallback( button, action, mods );
	} );
	glfwSetCharCallback( window, []( GLFWwindow * window, unsigned int codepoint )
	{
		scene->CharCallback( codepoint );
	} );
	glfwSetCharModsCallback( window, []( GLFWwindow * window, unsigned int codepoint, int mods )
	{
		scene->CharModsCallback( codepoint, mods );
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

	GLint flags;
	glGetIntegerv( GL_CONTEXT_FLAGS, &flags );
	if( flags & GL_CONTEXT_FLAG_DEBUG_BIT )
	{
		glEnable( GL_DEBUG_OUTPUT );
		glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );

		glDebugMessageCallback( glDebugOutput, nullptr );
		glDebugMessageControl( GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE );
	}

	/* Create and initialize scene*/
	glfwGetFramebufferSize( window, &width, &height );
	scene = std::make_unique<Scene::Manager>( );
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

