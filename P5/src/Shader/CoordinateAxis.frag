#version 330 core

// Vertex-Fragment-Exchange
in vec4 vfxColor;

out vec4 color;

void main( )
{
	color = vfxColor;
} 