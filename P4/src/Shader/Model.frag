#version 330 core

in vec2 TextureCoord;
in vec3 vfColor;

out vec4 Color;
uniform sampler2D Texture;

void main()
{
    Color = texture( Texture, vec2( TextureCoord.x, 1.0f - TextureCoord.y ) );
	//Color = vec4( vfColor, 1.0 );
}