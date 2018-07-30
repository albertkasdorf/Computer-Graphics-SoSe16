#version 430

// Vertex-Fragment-Exchange
in vec3 vfxColor;
in vec4 vfxNormal;
in vec2 vfxUV;

//----
out vec4 color;

//----
uniform bool useTexture;
uniform sampler2D diffuseTexture;

// Main
void main( )
{
	if( useTexture )
	{
		color = texture( diffuseTexture, vfxUV );
	}
	else
	{
		color = vec4( vfxColor, 1.0 );
	}
}