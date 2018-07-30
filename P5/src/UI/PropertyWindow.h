#pragma once
#include <AntTweakBar/AntTweakBar.h>
#include <string>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>

namespace UI
{

class PropertyWindow
{
public:
	PropertyWindow( void );
	~PropertyWindow( void );

private:
	TwBar * twBar;
	int mouseWheelScroll;
	std::string windowName = "Properties";

public:
	void Initialize( const GLint width, const GLint height );
	void Terminate( void );
	void Render( void );

	void FramebufferSizeCallback( const GLint width, const GLint height );
	void KeyCallback( const int key, const int scancode, const int action, const int mods );
	bool CursorPosCallback( const double xpos, const double ypos );
	bool ScrollCallback( const double xoffset, const double yoffset );
	bool MouseButtonCallback( const int button, const int action, const int mods );
	bool CharCallback( const unsigned int codepoint );

public:
	std::string NameGet( void ) const;

public:
	void ReadWriteVariableAdd(
		const std::string name, const std::string definition, glm::vec4 * variable );
	void ReadWriteVariableAdd(
		const std::string name, const std::string definition, glm::vec3 * variable );
	void ReadWriteVariableAdd(
		const std::string name, const std::string definition, float * variable );
	void ReadWriteVariableAdd(
		const std::string name, const std::string definition, GLboolean * variable );
	void ReadWriteVariableAdd(
		const std::string name, const std::string definition, std::int32_t * variable );
	void ReadWriteVariableAdd(
		const std::string & name,
		const std::string & definition,
		bool * variable );
	void ReadWriteVariableAdd(
		const std::string & name,
		const std::string & definition,
		std::string * variable );

	void DefineAdd( const std::string definition );

	void ButtonAdd(
		const std::string name,
		const std::string definition,
		void * clientData,
		TwButtonCallback callback );

	void CallbackVariableAdd(
		const std::string & name,
		const std::string & definition,
		const TwType & type,
		void *clientData,
		TwGetVarCallback getCallback,
		TwSetVarCallback setCallback );

};

}
