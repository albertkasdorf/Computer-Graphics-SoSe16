#pragma once
#include <vector>
#include <memory>
#include <map>
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

class ElapsedTime;
namespace GL
{
	class Buffer;
	class ShaderProgram;
	class Texture;
	class VertexArray;
}
namespace UI
{
	class PropertyWindow;
}

namespace Renderer
{

struct VertexData
{
	glm::vec3 vertex;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::uvec4 boneIndex;
	glm::vec4 boneWeight;

	VertexData( void ) : boneIndex( 0 ), boneWeight( 0.0f ) {}
};

struct Mesh
{
	std::vector<VertexData> vertices;
	std::vector<glm::uvec3> indices;

	std::unique_ptr<GL::Buffer> vbo;
	std::unique_ptr<GL::Buffer> ebo;
	std::unique_ptr<GL::VertexArray> vao;

	std::unique_ptr<GL::Texture> diffuseTexture;
};

class Model
{
public:
	Model( void );
	~Model( void );

private:
	std::vector<std::unique_ptr<Assimp::Importer>> importer;
	const aiScene * scene;
	std::vector < std::tuple<const aiAnimation *, std::string>> animations;


	/* GPU Rendering */
	std::unique_ptr<GL::ShaderProgram> shader;

	std::vector<Mesh> meshes;

	std::vector<VertexData> skeletonVertices;
	std::vector<glm::uvec2> skeletonIndices;

	std::unique_ptr<GL::Buffer> skeletonVBO;
	std::unique_ptr<GL::Buffer> skeletonEBO;
	std::unique_ptr<GL::VertexArray> skeletonVAO;

	std::vector<glm::mat4> jointMatrices;
	std::vector<glm::mat4> jointMatricesPrev;
	std::map<std::string, std::uint32_t> jointLookup;

	/* Eigenschaften */
	
	// Verwenden der GPU f�r die Animationen
	GLboolean useGPU = GL_TRUE;
	bool interpolateAnimation = true;

	GLboolean wireframeShow = GL_FALSE;
	

	std::uint32_t animIndex = 0;
	float animSpeed = 1.f;
	float animSample = 0.f;
	bool animRepeat = true;
	bool animPlay = true;

	glm::vec3 wireframeColor = glm::vec3( 0.5f );

	std::uint32_t blendIndex = 0;
	float blendWeight = 0.f;
	float blendSpeed = 1.f;
	bool blend = false;

	GLboolean skeletonShow = GL_FALSE;
	std::vector<float> jointsColor;
	float jointSize = 10.f;
	glm::vec3 boneColor = glm::vec3( 0.f, 1.f, 0.f );


// Properties
public:
	const GLboolean IsGpuInUse( void ) const { return useGPU; };


// Methoden
public:

	void Initialize(
		const std::string & meshFilePath,
		const std::vector<std::string> & animFilePaths );

	void UseGPU( void );
	void UseCPU( void );



	void Update( const ElapsedTime * elapsedTime );

	/* Render */
	void Render( glm::mat4 & projection, glm::mat4 & view, glm::mat4 & model );




	/* Animation */
	const aiAnimation * AnimationGet( void ) const;
	const aiAnimation * AnimationGet( const std::uint32_t & index ) const;

	void AnimationStart( void );

	void AnimationStop( void );
	void AnimationPause( void );

	bool AnimationRepeatGet( void ) const;
	void AnimationRepeatSet( bool value );

	std::string AnimationNameGet( void ) const;

	std::uint32_t AnimationIndexGet( void ) const;
	void AnimationIndexSet( const std::uint32_t & index );

	float AnimationDurationGet( void ) const;
	float AnimationTicksPerSecondGet( void ) const;

	std::string AnimationBlendingNameGet( void ) const;
	std::uint32_t AnimationBlendingIndexGet( void ) const;
	void AnimationBlendingIndexSet( const std::uint32_t & index );

	void AnimationBlendingRestart( void );

	void Inspect( UI::PropertyWindow * const propertyWindow );

private:
	void InitializeMeshData( const aiScene * scene );
	void InitializeSkeleton( const aiScene * scene );

	void InitializeVBO(
		const std::vector<VertexData> & vertextData,
		std::unique_ptr<GL::Buffer> & vbo,
		const GLenum usage );

	void InitializeEBO(
		std::unique_ptr<GL::Buffer> & ebo,
		const std::vector<glm::uvec3> & indices );

	void InitializeEBO(
		std::unique_ptr<GL::Buffer> & ebo,
		const std::vector<glm::uvec2> & indices );

	void InitializeVAO(
		std::unique_ptr<GL::VertexArray> & vao,
		const std::unique_ptr<GL::Buffer> & vbo,
		const std::unique_ptr<GL::Buffer> & ebo );

	void InitializeMaterial( const aiScene * scene );

	void InitializeShader( void );

	void RenderMesh( void );
	void RenderSkeleton( void );



	void ComputeBoneTransformation(
		const aiAnimation * animation, const float sample );
	void ComputeBoneTransformation2(
		const aiAnimation * animation1,
		const aiAnimation * animation2,
		const float sample );
	void ComputeBoneFinalTransformation( void );
	void ComputeBoneDataUpdate( void );

	void ComputeMuscleActivity( void );

	void ComputeMeshDataUpdate( void );
	void ComputeSkeletonDataUpdate( void );

	void UpdateVBO(
		const std::vector<VertexData> & vertextData,
		const std::unique_ptr<GL::Buffer> & vbo );
};

}
