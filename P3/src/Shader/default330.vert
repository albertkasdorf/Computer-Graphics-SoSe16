#version 330

layout(location = 0) in  vec4 vertex;
layout(location = 1) in  vec2 vertexUV;
layout(location = 0) out vec2 fragmentUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vertex;
    fragmentUV = vertexUV;
    //fragmentUV = vec2(vertexUV.x, 1.0f - vertexUV.y);
}
