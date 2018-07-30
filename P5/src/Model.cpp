#include "Model.h"
#include <set>
#include <queue>
#include <glm/gtx/matrix_decompose.hpp>
#include <assimp/LogStream.hpp>
#include <assimp/Logger.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/postprocess.h>
#include "GL/Buffer.h"
#include "GL/ShaderProgram.h"
#include "GL/Texture.h"
#include "GL/VertexArray.h"
#include "ElapsedTime.h"
#include "UI/PropertyWindow.h"
#include "IO/File.h"
#include "Rescale.h"
#include "Assimp/Assimp.h"


namespace Renderer
{

Model::Model( void )
{
}


Model::~Model( void )
{
}

void Model::Initialize(
	const std::string & meshFilePath,
	const std::vector<std::string> & animFilePaths )
{
	/*
	class myStream : public Assimp::LogStream
	{
	public:
	void write( const char* message )
	{
	std::cout << message;
	}
	};
	// Select the kinds of messages you want to receive on this log stream
	const unsigned int severity = 1 | 2 | 4 | 8;

	// Attaching it to the default logger
	Assimp::DefaultLogger::create( ASSIMP_DEFAULT_LOG_NAME, Assimp::Logger::VERBOSE );
	Assimp::DefaultLogger::get( )->attachStream( new myStream( ), severity );
	*/

	std::unique_ptr<Assimp::Importer> assimp;
	const std::uint32_t flags =
		aiProcess_GenSmoothNormals |
		aiProcess_LimitBoneWeights |
		aiProcess_FlipUVs;

	assimp = std::make_unique<Assimp::Importer>( );
	scene = assimp->ReadFile( meshFilePath, flags );
	assert( scene );
	importer.push_back( std::move( assimp ) );

	if( scene->HasAnimations( ) )
	{
		std::string fileName = basename(meshFilePath.c_str( ));

		animations.push_back(std::make_tuple(scene->mAnimations[0], fileName));
	}

	for( std::size_t a = 0; a < animFilePaths.size( ); a++ )
	{
		assimp = std::make_unique<Assimp::Importer>( );

		const aiScene * scene = assimp->ReadFile( animFilePaths[a], flags );
		assert( scene );
		if( scene->HasAnimations( ) )
		{
			std::string fileName = basename(animFilePaths[a].c_str( ));

			animations.push_back(std::make_tuple(scene->mAnimations[0], fileName));
		}

		importer.push_back( std::move( assimp ) );
	}

	AnimationIndexSet( 5 );
	AnimationBlendingIndexSet( 17 ); 

	InitializeMeshData( scene );
	InitializeSkeleton( scene );
	InitializeMaterial( scene );
	InitializeShader( );
}

void Model::Update( const ElapsedTime * elapsedTime )
{
	auto animation = AnimationGet( );

	if( animPlay )
	{
		/*animSample +=
			static_cast<float>(animation->mTicksPerSecond / animation->mDuration) *
			elapsedTime->Get( ) *
			animSpeed;*/

		if( blend )
		{
			blendWeight += elapsedTime->Get( ) * blendSpeed;
			blendWeight = std::min( blendWeight, 1.f );
		}

		const auto ipFloat = Assimp::Interpolator<double>( );
		double blendedDuration;
		ipFloat(
			blendedDuration,
			AnimationGet( animIndex )->mDuration,
			AnimationGet( blendIndex )->mDuration,
			blendWeight );
		
		animSample +=
			static_cast<float>(animation->mTicksPerSecond / blendedDuration) *
			elapsedTime->Get( ) *
			animSpeed;
	}

	if( animRepeat )
	{
		// fmod: Compute remainder of division
		animSample = std::fmod( animSample, 1.f );
	}
	else
	{
		animSample = std::min( animSample, 1.f );
	}

	/*std::cout.width( 15 );
	std::cout << std::left << elapsedTime->Get( ) << " ";
	std::cout.width( 15 );
	std::cout << animSample << " ";
	std::cout.width( 15 ); 
	std::cout << blendWeight << "\n";*/

	//----
	ComputeBoneTransformation2(
		AnimationGet( animIndex ), AnimationGet( blendIndex ), animSample );

	//ComputeBoneTransformation( animation, animSample );
	ComputeBoneFinalTransformation( );
	ComputeBoneDataUpdate( );
	ComputeMuscleActivity( );

	if( !IsGpuInUse( ) )
	{
		ComputeMeshDataUpdate( );
		ComputeSkeletonDataUpdate( );
	}
}


void Model::Render( glm::mat4 & projection, glm::mat4 & view, glm::mat4 & model )
{
	shader->ProgramEnable( );
	{
		shader->ProjectionMatrixSet( projection );
		shader->ViewMatrixSet( view );
		shader->ModelMatrixSet( model );

		shader->UniformSet( "useGPU", useGPU );
		if( useGPU )
			shader->UniformMatrixSet( "boneMatrix", jointMatrices );

		RenderMesh( );
		RenderSkeleton( );
	}
	shader->ProgramDisable( );
}

void Model::RenderMesh( void )
{
	shader->UniformSet( "primitiveColor", wireframeColor );
	for( const auto & mesh : meshes )
	{
		if( wireframeShow )
		{
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			shader->UniformSet( "useTexture", static_cast<GLboolean>(GL_FALSE) );
		}
		else
		{
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			
			shader->UniformSet( "useTexture", static_cast<GLboolean>(mesh.diffuseTexture ? GL_TRUE : GL_FALSE) );
			if( mesh.diffuseTexture )
			{
				glBindTexture( GL_TEXTURE_2D, mesh.diffuseTexture->IdGet( ) );
				shader->UniformSet( "diffuseTexture", 0 );
			}
		}

		glBindVertexArray( mesh.vao->IdGet( ) );
		{
			glDrawElements(
				GL_TRIANGLES,
				static_cast<GLsizei>(mesh.indices.size( ) * 3),
				GL_UNSIGNED_INT,
				nullptr );
		}
		glBindVertexArray( 0 );
	}
}

void Model::RenderSkeleton( void )
{
	if( !skeletonShow )
		return;

	GLint depthTest = GL_FALSE;

	glGetIntegerv( GL_DEPTH_TEST, &depthTest );
	glDisable( GL_DEPTH_TEST );

	shader->UniformSet( "useTexture", static_cast<GLboolean>(GL_FALSE) );

	glBindVertexArray( skeletonVAO->IdGet( ) );
	{
		// Draw the joint positions
		glEnable( GL_PROGRAM_POINT_SIZE );
		{
			shader->UniformSet( "pointSize", jointSize );
			//shader->UniformSet( "primitiveColor", jointColor );
			shader->UniformSet( "useColorMatrix", static_cast<GLboolean>(GL_TRUE) );
			shader->UniformSet( "colorMatrix", jointsColor );

			glDrawArrays( GL_POINTS, 0, static_cast<GLsizei>(skeletonVertices.size( )) );

			shader->UniformSet( "useColorMatrix", static_cast<GLboolean>(GL_FALSE) );
		}
		glDisable( GL_PROGRAM_POINT_SIZE );

		// Draw the bones
		shader->UniformSet( "primitiveColor", boneColor );
		glDrawElements( GL_LINES, static_cast<GLsizei>(skeletonIndices.size( ) * 2), GL_UNSIGNED_INT, 0 );
	}
	glBindVertexArray( 0 );

	if( depthTest == GL_FALSE )
		glDisable( GL_DEPTH_TEST );
	else
		glEnable( GL_DEPTH_TEST );
}

//
// Erzeugen der fixen Mesh-Daten für die GPU.
//
void Model::InitializeMeshData( const aiScene * scene )
{
	std::uint32_t numBones = 0;
	std::uint32_t offsetBone = 0;

	for( std::uint32_t m = 0; m < scene->mNumMeshes; m++ )
	{
		numBones += scene->mMeshes[m]->mNumBones;
	}

	if( meshes.size( ) != scene->mNumMeshes )
	{
		meshes.resize( scene->mNumMeshes );
	}

	jointMatrices.clear( );
	jointMatrices.resize( numBones );
	jointMatricesPrev.clear( );
	jointMatricesPrev.resize( numBones );
	jointsColor.clear( );
	jointsColor.resize( numBones );

	for( std::uint32_t m = 0; m < scene->mNumMeshes; m++ )
	{
		const aiMesh * srcMesh = scene->mMeshes[m];
		Mesh * const dstMesh = &meshes[m];

		dstMesh->vertices.clear( );
		dstMesh->vertices.resize( srcMesh->mNumVertices );

		dstMesh->indices.clear( );
		dstMesh->indices.resize( srcMesh->mNumFaces );

		for( std::uint32_t v = 0; v < srcMesh->mNumVertices; v++ )
		{
			VertexData * const vertexData = &(dstMesh->vertices[v]);

			vertexData->vertex[0] = srcMesh->mVertices[v][0];
			vertexData->vertex[1] = srcMesh->mVertices[v][1];
			vertexData->vertex[2] = srcMesh->mVertices[v][2];

			vertexData->normal[0] = srcMesh->mNormals[v][0];
			vertexData->normal[1] = srcMesh->mNormals[v][1];
			vertexData->normal[2] = srcMesh->mNormals[v][2];

			vertexData->uv[0] = srcMesh->mTextureCoords[0][v][0];
			vertexData->uv[1] = srcMesh->mTextureCoords[0][v][1];
		}

		for( std::uint32_t f = 0; f < srcMesh->mNumFaces; f++ )
		{
			const aiFace * face = &(srcMesh->mFaces[f]);

			dstMesh->indices[f][0] = face->mIndices[0];
			dstMesh->indices[f][1] = face->mIndices[1];
			dstMesh->indices[f][2] = face->mIndices[2];
		}

		for( std::uint32_t b = 0; b < srcMesh->mNumBones; b++ )
		{
			const aiBone * bone = srcMesh->mBones[b];
			const std::uint32_t indexBone = offsetBone + b;

			for( std::uint32_t w = 0; w < bone->mNumWeights; w++ )
			{
				const aiVertexWeight * weight = &(bone->mWeights[w]);
				VertexData * const vertexData = &(dstMesh->vertices[weight->mVertexId]);

				for( glm::length_t l = 0; l < vertexData->boneWeight.length( ); l++ )
				{
					if( vertexData->boneWeight[l] != 0.0f )
					{
						continue;
					}

					vertexData->boneIndex[l] = indexBone;
					vertexData->boneWeight[l] = weight->mWeight;
					break;
				}
			}

			jointMatrices[indexBone] = Assimp::ConvertToGLM( bone->mOffsetMatrix );
			jointLookup[std::string( bone->mName.C_Str( ) )] = indexBone;
		}

		InitializeVBO( dstMesh->vertices, dstMesh->vbo, useGPU ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW );
		InitializeEBO( dstMesh->ebo, dstMesh->indices );
		InitializeVAO( dstMesh->vao, dstMesh->vbo, dstMesh->ebo );

		offsetBone += srcMesh->mNumBones;
	}
}

void Model::InitializeSkeleton( const aiScene * scene )
{
	std::uint32_t numBones = 0;

	for( std::uint32_t m = 0; m < scene->mNumMeshes; m++ )
	{
		numBones += scene->mMeshes[m]->mNumBones;
	}

	skeletonVertices.clear( );
	skeletonVertices.resize( numBones );

	skeletonIndices.clear( );

	std::uint32_t offsetBone = 0;

	for( std::uint32_t m = 0; m < scene->mNumMeshes; m++ )
	{
		const aiMesh * mesh = scene->mMeshes[m];

		for( std::uint32_t b = 0; b < mesh->mNumBones; b++ )
		{
			const aiBone * bone = mesh->mBones[b];
			const std::uint32_t indexBone = offsetBone + b;

			skeletonVertices[indexBone].vertex =
				glm::vec3( glm::inverse( Assimp::ConvertToGLM( bone->mOffsetMatrix ) ) * glm::vec4( 0.f, 0.f, 0.f, 1.f ) );
			skeletonVertices[indexBone].normal = glm::vec3( 0.f );
			skeletonVertices[indexBone].uv = glm::vec2( 0.f );
			skeletonVertices[indexBone].boneIndex = glm::uvec4( indexBone, 0, 0, 0 );
			skeletonVertices[indexBone].boneWeight = glm::vec4( 1.f, 0.f, 0.f, 0.f );

			// Find parent bone (if exist) and add both to indices list
			const aiNode * childNode = scene->mRootNode->FindNode( bone->mName );
			aiNode * parentNode = childNode->mParent;

			while( parentNode != nullptr )
			{
				const std::string keyParent = std::string( parentNode->mName.C_Str( ) );

				if( jointLookup.find( keyParent ) != jointLookup.end( ) )
				{
					skeletonIndices.push_back(
						glm::uvec2( jointLookup[keyParent], indexBone ) );
					break;
				}

				parentNode = parentNode->mParent;
			}
		}
		offsetBone += mesh->mNumBones;
	}

	InitializeVBO( skeletonVertices, skeletonVBO, useGPU ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW );
	InitializeEBO( skeletonEBO, skeletonIndices );
	InitializeVAO( skeletonVAO, skeletonVBO, skeletonEBO );
}

void Model::InitializeVBO(
	const std::vector<VertexData> & vertextData,
	std::unique_ptr<GL::Buffer> & vbo,
	const GLenum usage )
{
	const GLsizeiptr size = sizeof( VertexData ) * vertextData.size( );
	const GLvoid * const data = &vertextData[0];

	vbo = std::make_unique<GL::Buffer>( );
	glBindBuffer( GL_ARRAY_BUFFER, vbo->IdGet( ) );
	glBufferData( GL_ARRAY_BUFFER, size, data, usage );
}

void Model::InitializeEBO(
	std::unique_ptr<GL::Buffer> & ebo,
	const std::vector<glm::uvec3> & indices )
{
	const GLsizeiptr size = sizeof( glm::uvec3 ) * indices.size( );
	const GLvoid * data = &indices[0];

	ebo = std::make_unique<GL::Buffer>( );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo->IdGet( ) );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW );
}

void Model::InitializeEBO(
	std::unique_ptr<GL::Buffer> & ebo,
	const std::vector<glm::uvec2> & indices )
{
	const GLsizeiptr size = sizeof( glm::uvec2 ) * indices.size( );
	const GLvoid * data = &indices[0];

	ebo = std::make_unique<GL::Buffer>( );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo->IdGet( ) );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW );
}

void Model::InitializeVAO(
	std::unique_ptr<GL::VertexArray> & vao,
	const std::unique_ptr<GL::Buffer> & vbo,
	const std::unique_ptr<GL::Buffer> & ebo )
{
	vao = std::make_unique<GL::VertexArray>( );
	glBindVertexArray( vao->IdGet( ) );
	{
		glBindBuffer( GL_ARRAY_BUFFER, vbo->IdGet( ) );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo->IdGet( ) );

		GLuint index = 0;
		GLint size = sizeof( glm::vec3 ) / sizeof( float );
		GLsizei stride = sizeof( VertexData );
		GLvoid * pointer = reinterpret_cast<GLvoid*>(0);

		glEnableVertexAttribArray( index );
		glVertexAttribPointer( index, size, GL_FLOAT, GL_FALSE, stride, pointer );

		index = 1;
		size = sizeof( glm::vec3 ) / sizeof( float );
		pointer = reinterpret_cast<GLvoid*>(reinterpret_cast<std::size_t>(pointer) + sizeof( glm::vec3 ));
		glEnableVertexAttribArray( index );
		glVertexAttribPointer( index, size, GL_FLOAT, GL_FALSE, stride, pointer );

		index = 2;
		size = sizeof( glm::vec2 ) / sizeof( float );
		pointer = reinterpret_cast<GLvoid*>(reinterpret_cast<std::size_t>(pointer) + sizeof( glm::vec3 ));
		glEnableVertexAttribArray( index );
		glVertexAttribPointer( index, size, GL_FLOAT, GL_FALSE, stride, pointer );

		// Passing uint attribute to GLSL
		// http://stackoverflow.com/questions/18919927/passing-uint-attribute-to-glsl
		// https://www.opengl.org/sdk/docs/man/html/glVertexAttribPointer.xhtml
		// For glVertexAttribIPointer, only the integer types GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, GL_UNSIGNED_INT are accepted. Values are always left as integer values.
		index = 3;
		size = sizeof( glm::uvec4 ) / sizeof( std::uint32_t );
		pointer = reinterpret_cast<GLvoid*>(reinterpret_cast<std::size_t>(pointer) + sizeof( glm::vec2 ));
		glEnableVertexAttribArray( index );
		glVertexAttribIPointer( index, size, GL_UNSIGNED_INT, stride, pointer );

		index = 4;
		size = sizeof( glm::vec4 ) / sizeof( float );
		pointer = reinterpret_cast<GLvoid*>(reinterpret_cast<std::size_t>(pointer) + sizeof( glm::uvec4 ));
		glEnableVertexAttribArray( index );
		glVertexAttribPointer( index, size, GL_FLOAT, GL_FALSE, stride, pointer );
	}
	glBindVertexArray( 0 );
}

void Model::InitializeMaterial( const aiScene * scene )
{
	// Jedes Mesh muss eine Material besitzen.
	assert( scene->mNumMeshes == scene->mNumMaterials );

	for( std::uint32_t m = 0; m < scene->mNumMaterials; m++ )
	{
		const aiMaterial * material = scene->mMaterials[m];

		//AI_MATKEY_TEXTURE_NORMALS
		std::uint32_t diffuseCount = material->GetTextureCount( aiTextureType_DIFFUSE );

		aiReturn result;
		aiString path;

		result = material->GetTexture( aiTextureType_DIFFUSE, 0, &path );
		if( result == aiReturn_SUCCESS )
		{
			std::string filePath = std::string( path.C_Str( ) );

			if( IO::File::Exists( filePath ) )
			{
				meshes[m].diffuseTexture = std::make_unique<GL::Texture>( filePath );
				meshes[m].diffuseTexture->Create( );
			}
		}

		/*material->GetTexture( aiTextureType_SPECULAR, 0, &path );
		material->GetTexture( aiTextureType_NORMALS, 0, &path );
		material->GetTexture( aiTextureType_HEIGHT, 0, &path );
		material->GetTexture( aiTextureType_AMBIENT, 0, &path );*/

		continue;
	}
}

void Model::InitializeShader( void )
{
	shader = std::make_unique<GL::ShaderProgram>(
		"./Shader/Model.vs", "./Shader/Model.fs" );
	shader->CompileAndLink( );
}

void Model::ComputeBoneTransformation(
	const aiAnimation * animation, const float sample )
{
	Rescale rescaler( 0.f, static_cast<float>(animation->mDuration), 0.f, 1.f );

	const float sampleUnscaled = rescaler.Unscale( sample );

	// mTransformation für alle Knoten (Joints) aktualisiert.
	for( std::uint32_t c = 0; c < animation->mNumChannels; c++ )
	{
		const aiNodeAnim * channel = animation->mChannels[c];
		aiNode * node = scene->mRootNode->FindNode( channel->mNodeName );

		const auto translation =
			Assimp::InterpolationGet<aiVector3D>(
				sampleUnscaled,
				channel->mPositionKeys,
				channel->mNumPositionKeys,
				interpolateAnimation );

		const auto rotation =
			Assimp::InterpolationGet<aiQuaternion>(
				sampleUnscaled,
				channel->mRotationKeys,
				channel->mNumRotationKeys,
				interpolateAnimation );

		aiMatrix4x4 transformation;
		aiMatrix4x4::Translation( translation, transformation );
		transformation = transformation * aiMatrix4x4( rotation.GetMatrix( ) );

		node->mTransformation = transformation;
	}
}

void Model::ComputeBoneTransformation2(
	const aiAnimation * animation1,
	const aiAnimation * animation2,
	const float sample )
{
	Rescale rescaler1( 0.f, static_cast<float>(animation1->mDuration), 0.f, 1.f );
	Rescale rescaler2( 0.f, static_cast<float>(animation2->mDuration), 0.f, 1.f );

	const float sample1Unscaled = rescaler1.Unscale( sample );
	const float sample2Unscaled = rescaler2.Unscale( sample );

	// mTransformation für alle Knoten (Joints) aktualisiert.
	for( std::uint32_t c = 0; c < animation1->mNumChannels; c++ )
	{
		const aiNodeAnim * channel1 = animation1->mChannels[c];
		const aiNodeAnim * channel2 = animation2->mChannels[c];
		aiNode * node = scene->mRootNode->FindNode( channel1->mNodeName );

		const auto translation1 =
			Assimp::InterpolationGet<aiVector3D>(
				sample1Unscaled,
				channel1->mPositionKeys,
				channel1->mNumPositionKeys,
				interpolateAnimation );

		const auto translation2 =
			Assimp::InterpolationGet<aiVector3D>(
				sample2Unscaled,
				channel2->mPositionKeys,
				channel2->mNumPositionKeys,
				interpolateAnimation );

		const auto rotation1 =
			Assimp::InterpolationGet<aiQuaternion>(
				sample1Unscaled,
				channel1->mRotationKeys,
				channel1->mNumRotationKeys,
				interpolateAnimation );

		const auto rotation2 =
			Assimp::InterpolationGet<aiQuaternion>(
				sample2Unscaled,
				channel2->mRotationKeys,
				channel2->mNumRotationKeys,
				interpolateAnimation );

		aiVector3D translation;
		aiQuaternion rotation;

		const auto ipVect = Assimp::Interpolator<aiVector3D>( );
		const auto ipQuat = Assimp::Interpolator<aiQuaternion>( );

		ipVect( translation, translation1, translation2, blendWeight );
		ipQuat( rotation, rotation1, rotation2, blendWeight );

		aiMatrix4x4 transformation;
		aiMatrix4x4::Translation( translation, transformation );
		transformation = transformation * aiMatrix4x4( rotation.GetMatrix( ) );

		node->mTransformation = transformation;
	}
}

void Model::ComputeBoneFinalTransformation( void )
{
	const aiAnimation * animation = AnimationGet( );
	std::set<std::string> lookup;

	for( std::uint32_t c = 0; c < animation->mNumChannels; c++ )
	{
		const aiNodeAnim * channel = animation->mChannels[c];
		const std::string key = Assimp::ConvertToSTD( channel->mNodeName );

		if( lookup.find( key ) != lookup.end( ) )
			continue;

		lookup.insert( key );
	}

	std::queue<aiNode*> queue;

	queue.push( scene->mRootNode );
	while( !queue.empty( ) )
	{
		aiNode * node = queue.front( );
		queue.pop( );

		for( std::uint32_t c = 0; c < node->mNumChildren; c++ )
		{
			queue.push( node->mChildren[c] );
		}

		if( node->mParent == nullptr )
			continue;

		if( lookup.find( Assimp::ConvertToSTD( node->mName ) ) == lookup.end( ) )
			continue;

		node->mTransformation = node->mParent->mTransformation * node->mTransformation;
	}
}

void Model::ComputeBoneDataUpdate( void )
{
	std::uint32_t offsetBone = 0;

	for( std::uint32_t m = 0; m < scene->mNumMeshes; m++ )
	{
		const aiMesh * mesh = scene->mMeshes[m];

		for( std::uint32_t b = 0; b < mesh->mNumBones; b++ )
		{
			const aiBone * bone = mesh->mBones[b];
			const aiNode * node = scene->mRootNode->FindNode( bone->mName );
			const std::uint32_t indexBone = offsetBone + b;

			jointMatrices[indexBone] =
				Assimp::ConvertToGLM(
					node->mTransformation * bone->mOffsetMatrix );
		}

		offsetBone += mesh->mNumBones;
	}
}

void Model::ComputeMuscleActivity( void )
{
	glm::vec3 scale, skew;
	glm::vec4 perspective;
	glm::quat rotCur, rotPrev;
	glm::vec3 transCur, transPrev;
	
	bool result;
	float maxValue = 0.f;

	for( std::size_t j = 0; j < jointMatrices.size( ); j++ )
	{
		result = glm::decompose(
			jointMatrices[j], scale, rotCur, transCur, skew, perspective );
		//assert( result );

		result = glm::decompose(
			jointMatricesPrev[j], scale, rotPrev, transPrev, skew, perspective );
		//assert( result );

		const auto angleCur = glm::eulerAngles( rotCur );
		const auto anglePrev = glm::eulerAngles( rotPrev );
		const auto angleDiff = angleCur - anglePrev;
		const auto angleLen = glm::length( angleDiff );

		const auto transDiff = transCur - transPrev;
		const auto transLen = glm::length( transDiff );

		jointsColor[j] =
			log10f( angleLen + 1.f ) +
			//( angleLen + 0.f ) +
			log10f( transLen + 1.f );
		maxValue = std::max( maxValue, jointsColor[j] );
	}

	const Rescale rs( 0.f, maxValue, 0.0f, 1.f );
	for( auto & value : jointsColor )
	{
		value = rs.Scale( value );
	}

	//----
	jointMatricesPrev = jointMatrices;
}

//
// Alle Vertices aktualisieren
//
void Model::ComputeMeshDataUpdate( void )
{
	for( std::uint32_t m = 0; m < scene->mNumMeshes; m++ )
	{
		const aiMesh * mesh = scene->mMeshes[m];

		for( std::uint32_t v = 0; v < mesh->mNumVertices; v++ )
		{
			VertexData * const vertexData = &(meshes[m].vertices[v]);

			const glm::mat4 skinning = 
				jointMatrices[vertexData->boneIndex[0]] * vertexData->boneWeight[0] +
				jointMatrices[vertexData->boneIndex[1]] * vertexData->boneWeight[1] +
				jointMatrices[vertexData->boneIndex[2]] * vertexData->boneWeight[2] +
				jointMatrices[vertexData->boneIndex[3]] * vertexData->boneWeight[3];

			const glm::vec4 vertex = Assimp::ConvertToGLM( mesh->mVertices[v], 1.f );
			const glm::vec4 normal = Assimp::ConvertToGLM( mesh->mNormals[v], 0.f );

			vertexData->vertex = glm::vec3( skinning * vertex );
			vertexData->normal= glm::vec3( skinning * normal );
		}

		// Vertexdaten an die Grafikkarte schicken
		UpdateVBO( meshes[m].vertices, meshes[m].vbo );
	}
}

void Model::ComputeSkeletonDataUpdate( void )
{
	std::uint32_t offsetBone = 0;

	for( std::uint32_t m = 0; m < scene->mNumMeshes; m++ )
	{
		const aiMesh * mesh = scene->mMeshes[m];

		for( std::uint32_t b = 0; b < mesh->mNumBones; b++ )
		{
			const aiBone * bone = mesh->mBones[b];
			const std::uint32_t index = offsetBone + b;
			VertexData * vd = &(skeletonVertices[index]);

			// Joints werden nur von einer Matrix beeinflusst.
			const glm::mat4 skinning = jointMatrices[vd->boneIndex[0]];
			const glm::vec4 vertex =
				glm::inverse( Assimp::ConvertToGLM( bone->mOffsetMatrix ) ) * glm::vec4( 0.f, 0.f, 0.f, 1.f );

			skeletonVertices[index].vertex = glm::vec3( skinning * vertex );
		}
		offsetBone += mesh->mNumBones;
	}

	// Vertexdaten an die Grafikkarte schicken
	UpdateVBO( skeletonVertices, skeletonVBO );
}

void Model::UpdateVBO(
	const std::vector<VertexData> & vertextData,
	const std::unique_ptr<GL::Buffer> & vbo )
{
	const GLsizeiptr size = sizeof( VertexData ) * vertextData.size( );
	const GLvoid * data = &vertextData[0];

	glBindBuffer( GL_ARRAY_BUFFER, vbo->IdGet( ) );
	glBufferSubData( GL_ARRAY_BUFFER, 0, size, data );
}



void Model::Inspect( UI::PropertyWindow * const propertyWindow )
{
	//
	// Animation
	//
	propertyWindow->CallbackVariableAdd(
		"Model::AnimationNameGet", "group=Animation label=Name", 
		TwType::TW_TYPE_STDSTRING, this,
		[]( void * value, void * clientData )
		{
			auto model = static_cast<const Model*>(clientData);
			auto name = static_cast<std::string*>(value);

			TwCopyStdStringToLibrary( *name, model->AnimationNameGet( ) );
		},
		nullptr );

	propertyWindow->CallbackVariableAdd(
		"Model::AnimationIndexGet/Set",
		"group=Animation label=Index",
		TwType::TW_TYPE_UINT32,
		this,
		[]( void * value, void * clientData )
		{
			auto model = static_cast<const Model*>(clientData);
			auto animIndex = static_cast<std::uint32_t*>(value);

			*animIndex = model->AnimationIndexGet( );
		},
		[]( const void * value, void * clientData )
		{
			auto model = static_cast<Model*>(clientData);
			auto animIndex = static_cast<const std::uint32_t*>(value);

			model->AnimationIndexSet( *animIndex );
		} );

	propertyWindow->CallbackVariableAdd(
		"Model::AnimationDurationGet", "group=Animation label=Duration",
		TwType::TW_TYPE_FLOAT, this,
		[]( void * value, void * clientData )
		{
			auto model = static_cast<const Model*>(clientData);
			auto duration = static_cast<float*>(value);

			*duration = model->AnimationDurationGet( );
		},
		nullptr );

	propertyWindow->CallbackVariableAdd(
		"Model::AnimationTicksPerSecondGet",
		"group=Animation label='Ticks Per Second'",
		TwType::TW_TYPE_FLOAT, this,
		[]( void * value, void * clientData )
		{
			auto model = static_cast<const Model*>(clientData);
			auto tps = static_cast<float*>(value);

			*tps = model->AnimationTicksPerSecondGet( );
		},
		nullptr );

	propertyWindow->ReadWriteVariableAdd(
		"Model::animSpeed", "group=Animation label=Speed step=0.1 min=0.0",
		&animSpeed );

	propertyWindow->ReadWriteVariableAdd(
		"Model::interpolateAnimation", "group=Animation label=Interpolate",
		&interpolateAnimation );

	propertyWindow->CallbackVariableAdd(
		"Model::useGPU", "group=Animation label='Use GPU'",
		TwType::TW_TYPE_BOOLCPP,
		this,
		[]( void * value, void * clientData )
		{
			auto model = static_cast<const Model*>(clientData);
			auto gpuUsed = static_cast<bool*>(value);

			*gpuUsed = model->IsGpuInUse( ) == GL_FALSE ? false : true;
		},
		[]( const void * value, void * clientData )
		{
			auto model = static_cast<Model*>(clientData);
			if( model->IsGpuInUse( ) )
				model->UseCPU( );
			else
				model->UseGPU( );
		} );

	propertyWindow->CallbackVariableAdd(
		"Model::animRepeat", "group=Animation label=Repeat",
		TwType::TW_TYPE_BOOLCPP,
		this,
		[]( void * value, void * clientData )
		{
			auto model = static_cast<const Model*>(clientData);
			auto animRepeat = static_cast<bool*>(value);

			*animRepeat = model->AnimationRepeatGet( );
		},
		[]( const void * value, void * clientData )
		{
			auto model = static_cast<Model*>(clientData);
			auto animRepeat = static_cast<const bool*>(value);

			model->AnimationRepeatSet( *animRepeat );
		} );

	propertyWindow->ButtonAdd(
		"Model::AnimationStart", "group=Animation label=Start", this,
		[]( void * clientData )
		{
			Model * model = static_cast<Model*>(clientData);
			model->AnimationStart( );
		} );
	
	propertyWindow->ButtonAdd(
		"Model::AnimationPause", "group=Animation label=Pause", this,
		[]( void * clientData )
		{
			Model * model = static_cast<Model*>(clientData);
			model->AnimationPause( );
		} );

	propertyWindow->ButtonAdd(
		"Model::AnimationStop", "group=Animation label=Stop", this,
		[]( void * clientData )
		{
			Model * model = static_cast<Model*>(clientData);
			model->AnimationStop( );
		} );

	//
	// Blending, will it blend? ;)
	//
	propertyWindow->CallbackVariableAdd(
		"Model::AnimationBlendingNameGet", "group=Blending label=Name",
		TwType::TW_TYPE_STDSTRING, this,
		[]( void * value, void * clientData )
		{
			auto model = static_cast<const Model*>(clientData);
			auto name = static_cast<std::string*>(value);

			TwCopyStdStringToLibrary( *name, model->AnimationBlendingNameGet( ) );
		},
		nullptr );

	propertyWindow->CallbackVariableAdd(
		"Model::AnimationBlendingIndexGet/Set", "group=Blending label=Index",
		TwType::TW_TYPE_UINT32,
		this,
		[]( void * value, void * clientData )
		{
			auto model = static_cast<const Model*>(clientData);
			auto blendIndex = static_cast<std::uint32_t*>(value);

			*blendIndex = model->AnimationBlendingIndexGet( );
		},
		[]( const void * value, void * clientData )
		{
			auto model = static_cast<Model*>(clientData);
			auto blendIndex = static_cast<const std::uint32_t*>(value);

			model->AnimationBlendingIndexSet( *blendIndex );
		} );

	propertyWindow->ReadWriteVariableAdd(
		"Model::blendWeight",
		"group=Blending label=Weight min=0 max=1 step=0.1",
		&blendWeight );

	propertyWindow->ReadWriteVariableAdd(
		"Model::blendSpeed",
		"group=Blending label=Speed min=0 step=0.1",
		&blendSpeed );

	propertyWindow->ButtonAdd(
		"Model::AnimationBlendingRestart", "group=Blending label=Restart", this,
		[]( void * clientData )
		{
			Model * model = static_cast<Model*>(clientData);
			model->AnimationBlendingRestart( );
		} );

	//
	// Skeleton
	//
	propertyWindow->ReadWriteVariableAdd(
		"Model::skeletonShow", "group=Skeleton label='Show'",
		&skeletonShow );

	propertyWindow->ReadWriteVariableAdd(
		"Model::jointSize", "group=Skeleton label='Joint Size'",
		&jointSize );

	propertyWindow->ReadWriteVariableAdd(
		"Model::boneColor", "group=Skeleton label='Bone Color'",
		&boneColor );

	//
	// Wireframe
	//
	propertyWindow->ReadWriteVariableAdd(
		"Model::wireframeShow", "group=Wireframe label='Show'",
		&wireframeShow );

	propertyWindow->ReadWriteVariableAdd(
		"Model::wireframeColor", "group=Wireframe label='Color'",
		&wireframeColor );

	// Create sub groups
	propertyWindow->DefineAdd(
		propertyWindow->NameGet( ) + "/Animation group=Model opened=true" );

	propertyWindow->DefineAdd(
		propertyWindow->NameGet( ) + "/Blending group=Model opened=true" );

	propertyWindow->DefineAdd(
		propertyWindow->NameGet( ) + "/Skeleton group=Model opened=true" );

	propertyWindow->DefineAdd(
		propertyWindow->NameGet( ) + "/Wireframe group=Model opened=true" );
}

void Model::UseGPU( void )
{
	if( useGPU )
		return;

	useGPU = GL_TRUE;

	InitializeMeshData( scene );
	InitializeSkeleton( scene );
}

void Model::UseCPU( void )
{
	if( !useGPU )
		return;

	useGPU = GL_FALSE;

	InitializeMeshData( scene );
	InitializeSkeleton( scene );
}

std::uint32_t Model::AnimationIndexGet( void ) const
{
	return animIndex;
}

void Model::AnimationIndexSet( const std::uint32_t & index )
{
	animIndex = std::min(
		index,
		static_cast<std::uint32_t>(animations.size( ) - 1) );
	animSample = 0.f;
}

const aiAnimation * Model::AnimationGet( void ) const
{
	return std::get<0>( animations[animIndex] );
}

const aiAnimation * Model::AnimationGet( const std::uint32_t & index ) const
{
	std::uint32_t checkedIndex = std::min(
		index, static_cast<std::uint32_t>(animations.size( ) - 1) );

	return std::get<0>( animations[checkedIndex] );
}

void Model::AnimationStart( void )
{
	animPlay = true;
	if( !animRepeat )
	{
		animSample = 0.f;
	}
}

// Reset animSample to zero.
void Model::AnimationStop( void )
{
	animPlay = false;
	animSample = 0.f;
}

// animSample nicht weiter erhöhen.
void Model::AnimationPause( void )
{
	animPlay = false;
}

bool Model::AnimationRepeatGet( void ) const
{
	return animRepeat;
}

void Model::AnimationRepeatSet( bool value )
{
	animRepeat = value;
}

std::string Model::AnimationNameGet( void ) const
{
	return std::get<1>( animations[animIndex] );
}

float Model::AnimationDurationGet( void ) const
{
	auto animation = AnimationGet( );
	return static_cast<float>(animation->mDuration);
}

float Model::AnimationTicksPerSecondGet( void ) const
{
	auto animation = AnimationGet( );
	return static_cast<float>(animation->mTicksPerSecond);
}

std::string Model::AnimationBlendingNameGet( void ) const
{
	return std::get<1>( animations[blendIndex] );
}

std::uint32_t Model::AnimationBlendingIndexGet( void ) const
{
	return blendIndex;
}

void Model::AnimationBlendingIndexSet( const std::uint32_t & index )
{
	blendIndex = std::min(
		index,
		static_cast<std::uint32_t>(animations.size( ) - 1) );
}

void Model::AnimationBlendingRestart( void )
{
	blend = !blend;
	blendWeight = 0.f;
	if( blend )
		animSample = 0.f;
}

}
