#include "PropertyWindow.h"
#include <GLFW/glfw3.h>

namespace UI
{

PropertyWindow::PropertyWindow( void ) :
	twBar( nullptr ),
	mouseWheelScroll( 0 )
{
}

PropertyWindow::~PropertyWindow( void )
{
}

void PropertyWindow::Initialize( const GLint width, const GLint height )
{
	//const glm::uvec2 primaryDisplayMonitor(
	//	GetSystemMetrics( SM_CXSCREEN ), GetSystemMetrics( SM_CYSCREEN ) );
	// Notebook Monitor {x=3200 y=1800}
	// Extern Monitor   {x=1920 y=1080}

	//const glm::vec2 dpiScale(
	//	::GetDeviceCaps( ::GetDC( nullptr ), LOGPIXELSX ) / 96.f,
	//	::GetDeviceCaps( ::GetDC( nullptr ), LOGPIXELSY ) / 96.f);
	//// Notebook Monitor {x=192.000000 y=192.000000}
	//// Extern Monitor   {x = 96.0000000 y = 96.0000000}
	//if( dpiScale.x > 1.f || dpiScale.y > 1.f )
	//{
	//	TwDefine( "GLOBAL fontscaling=2" );
	//}

	TwInit( TW_OPENGL, nullptr );
	//TwInit( TW_OPENGL_CORE, nullptr );

	TwDeleteAllBars( );
	twBar = TwNewBar( windowName.c_str( ) );

	// TwBar parameters syntax
	// http://anttweakbar.sourceforge.net/doc/tools:anttweakbar:twbarparamsyntax
	TwDefine( "GLOBAL fontsize=3 contained=true" );
	//TwDefine( " Properties iconified=true " );
	//TwDefine( " Properties refresh=0 " );

	TwWindowSize( width, height );

	glm::uvec2 size( 300, 600 );
	TwSetParam( twBar, nullptr, "size", TW_PARAM_INT32, 2, &size[0] );
}

void PropertyWindow::Terminate( void )
{
	TwTerminate( );
}

void PropertyWindow::Render( void )
{
	TwDraw( );
}

void PropertyWindow::FramebufferSizeCallback( const GLint width, const GLint height )
{
	TwWindowSize( width, height );
}

void PropertyWindow::KeyCallback( const int key, const int scancode, const int action, const int mods )
{
	TwEventKeyGLFW( key, action );
}

bool PropertyWindow::CursorPosCallback( const double xpos, const double ypos )
{
	int handled =
		TwEventMousePosGLFW( static_cast<int>(xpos), static_cast<int>(ypos) );

	return handled ? true : false;
}

bool PropertyWindow::ScrollCallback( const double xoffset, const double yoffset )
{
	mouseWheelScroll += static_cast<int>(yoffset);

	int handled =
		TwEventMouseWheelGLFW( mouseWheelScroll );

	return handled ? true : false;
}

bool PropertyWindow::MouseButtonCallback( const int button, const int action, const int mods )
{
	int handled =
		TwEventMouseButtonGLFW( button, action );

	return handled ? true : false;
}

bool PropertyWindow::CharCallback( const unsigned int codepoint )
{
	int handled =
		TwEventCharGLFW( codepoint, GLFW_PRESS );

	return handled ? true : false;
}

void PropertyWindow::ReadWriteVariableAdd(
	const std::string name,
	const std::string definition,
	glm::vec4 * variable )
{
	// Variable parameters syntax
	// http://anttweakbar.sourceforge.net/doc/tools:anttweakbar:varparamsyntax
	TwAddVarRW( twBar, name.c_str( ), TW_TYPE_COLOR4F, variable, definition.c_str( ) );
}

void PropertyWindow::ReadWriteVariableAdd(
	const std::string name,
	const std::string definition,
	glm::vec3 * variable )
{
	TwAddVarRW( twBar, name.c_str( ), TW_TYPE_COLOR3F, variable, definition.c_str( ) );
}

void PropertyWindow::ReadWriteVariableAdd(
	const std::string name,
	const std::string definition,
	float * variable )
{
	TwAddVarRW( twBar, name.c_str( ), TW_TYPE_FLOAT, variable, definition.c_str( ) );
}

void PropertyWindow::ReadWriteVariableAdd(
	const std::string name,
	const std::string definition,
	GLboolean * variable )
{
	TwAddVarRW( twBar, name.c_str( ), TW_TYPE_BOOL8, variable, definition.c_str( ) );
}

void PropertyWindow::ReadWriteVariableAdd(
	const std::string name,
	const std::string definition,
	std::int32_t * variable )
{
	TwAddVarRW( twBar, name.c_str( ), TW_TYPE_INT32, variable, definition.c_str( ) );
}

void PropertyWindow::ReadWriteVariableAdd(
	const std::string & name,
	const std::string & definition,
	bool * variable )
{
	TwAddVarRW(
		twBar, name.c_str( ), TW_TYPE_BOOLCPP, variable, definition.c_str( ) );
}

void PropertyWindow::ReadWriteVariableAdd(
	const std::string & name,
	const std::string & definition,
	std::string * variable )
{
	TwAddVarRW(
		twBar, name.c_str( ), TW_TYPE_STDSTRING, variable, definition.c_str( ) );
}

void PropertyWindow::DefineAdd( const std::string definition )
{
	TwDefine( definition.c_str( ) );
}

std::string PropertyWindow::NameGet( void ) const
{
	return windowName;
}

void PropertyWindow::ButtonAdd(
	const std::string name,
	const std::string definition,
	void * clientData,
	TwButtonCallback callback )
{
	TwAddButton( twBar, name.c_str( ), callback, clientData, definition.c_str( ) );
}

void PropertyWindow::CallbackVariableAdd(
	const std::string & name,
	const std::string & definition,
	const TwType & type,
	void * clientData,
	TwGetVarCallback getCallback,
	TwSetVarCallback setCallback )
{
	TwAddVarCB( twBar, name.c_str( ), type, setCallback, getCallback, clientData, definition.c_str( ) );
}

}
