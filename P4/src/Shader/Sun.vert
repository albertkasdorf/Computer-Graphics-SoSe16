#version 330 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 textureCoord;
layout (location = 2) in vec3 normal;

out vec2 diffuseMapCoord;
out vec3 vfColor;			// vf=vertex-fragment

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4( position, 1.0 );
	diffuseMapCoord = textureCoord;
	vfColor = normal;
}