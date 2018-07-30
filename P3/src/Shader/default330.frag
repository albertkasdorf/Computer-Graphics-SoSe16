#version 330

layout(location = 0) in vec2 fragmentUV;

uniform sampler2D tex;

void main()
{
    gl_FragColor = vec4( 1.0, 0.0, 0.0, 1.0 );
    gl_FragColor = texture(tex, fragmentUV);
}
