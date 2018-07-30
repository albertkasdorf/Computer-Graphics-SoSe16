#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Vertex-Fragment-Exchange
out vec4 vfxColor;

void main( )
{
	gl_Position = projection * view * model * vec4( position, 1.0 );
	vfxColor = vec4( color, 1.0 );
}