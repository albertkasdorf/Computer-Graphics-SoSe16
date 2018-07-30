#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in uvec4 boneIndex;
layout (location = 4) in vec4 boneWeight;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform bool useGPU;
uniform float pointSize;
uniform vec3 primitiveColor;
uniform mat4 boneMatrix[128];
uniform bool useColorMatrix;
uniform float colorMatrix[128];

// Vertex-Fragment-Exchange
out vec3 vfxColor;
out vec4 vfxNormal;
out vec2 vfxUV;

// Main
void main( void )
{
	mat4 skinning = mat4( 1.0 );

	//----
	if( useGPU )
	{
		skinning =
			boneMatrix[boneIndex[0]] * boneWeight[0] +
			boneMatrix[boneIndex[1]] * boneWeight[1] +
			boneMatrix[boneIndex[2]] * boneWeight[2] +
			boneMatrix[boneIndex[3]] * boneWeight[3];
	}

	//----
	if( useColorMatrix )
	{
		vfxColor = vec3( colorMatrix[boneIndex[0]], 0, 0 );
	}
	else
	{
		vfxColor = primitiveColor;
	}
	vfxNormal = skinning * vec4( normal, 0.0 );
	vfxUV = uv;

	//----
	gl_PointSize = pointSize;
	gl_Position = projection * view * model * skinning * vec4( position, 1.0 );
}