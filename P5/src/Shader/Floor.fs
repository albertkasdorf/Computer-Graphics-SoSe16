#version 430

// Vertex-Fragment-Exchange
in vec2 vfxUV;

//----
out vec4 color;

//----
uniform sampler2D diffuseTexture;


// Main
void main( )
{
	color = texture( diffuseTexture, vfxUV );
}