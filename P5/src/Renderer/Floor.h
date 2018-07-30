#pragma once
#include <memory>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>

namespace GL
{
	class Buffer;
	class ShaderProgram;
	class Texture;
	class VertexArray;
}

namespace Renderer
{
	class Floor
	{
	public:
		Floor( void );
		~Floor( void );

	private:
		std::unique_ptr<GL::ShaderProgram> shader;
		std::unique_ptr<GL::Texture> texture;
		std::unique_ptr<GL::Buffer> vbo;
		std::unique_ptr<GL::VertexArray> vao;
		GLsizei count;

	public:
		void Initialize( void );

		void Render( 
			const glm::mat4 & projection,
			const glm::mat4 & view,
			const glm::mat4 & model ) const;

	};
}
