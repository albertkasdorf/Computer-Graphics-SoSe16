#pragma once
#include <memory>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>

namespace GL
{
	class Buffer;
	class ShaderProgram;
	class VertexArray;
}

namespace Renderer
{

class CoordinateAxis
{
public:
	CoordinateAxis( void );
	~CoordinateAxis( void );

private:
	const float axisLenght;

	const std::unique_ptr<GL::Buffer> vbo;
	const std::unique_ptr<GL::VertexArray> vao;
	GLsizei count;

	std::unique_ptr<GL::ShaderProgram> shader;

public:
	void Initialize( void );
	void Render( glm::mat4 & projection, glm::mat4 & view, glm::mat4 & model );

private:
	void InitializeGeometry( void );
	void InitializeShader( void );
};

}
