#version 430

//----
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

//----
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Vertex-Fragment-Exchange
out vec2 vfxUV;

// Main
void main( void )
{
	vfxUV = uv;
	gl_Position = projection * view * model * vec4( position, 1.0 );
}