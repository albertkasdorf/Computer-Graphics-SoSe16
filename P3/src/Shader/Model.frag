#version 330 core

in vec2 TextureCoord;
out vec4 Color;
uniform sampler2D Texture;

void main()
{
    Color = texture( Texture, vec2( TextureCoord.x, 1.0f - TextureCoord.y ) );
} 