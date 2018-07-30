#version 430 core
// Vertex shader with phong lighting

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexTextureCoord;
layout (location = 2) in vec3 vertexNormal;

// Uniform input
layout (location = 3) uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normal;

// Vertex-Fragment exchange
flat out vec3 vfNormal;
//out vec3 vfNormal;
out vec3 vfPosition;
out vec2 vfTextureCoord;

// Main start point
void main()
{
	vfNormal = normalize( vec3( normal * vec4( vertexNormal, 0.0 ) ) );

	// Do not look at me, look at the sun^^
	//vfPosition = vec3( view * model * vec4( vertexPosition, 1.0 ) );
	vfPosition = vec3( model * vec4( vertexPosition, 1.0 ) );

	vfTextureCoord = vertexTextureCoord;

	gl_Position = projection * view * model * vec4( vertexPosition, 1.0 );
}
