#version 330 core
// Fragment shader with phong lighting

// Vertex-Fragment exchange
flat in vec3 vfNormal;
//in vec3 vfNormal;
in vec3 vfPosition;
in vec2 vfTextureCoord;

// Uniform input
uniform vec3 lightPosition;		// light position in world coordinates
uniform vec3 lightIntensity;	// light 'color '
uniform vec3 kd;				// diffuse-reflection coefficient [0,1]
uniform vec3 ka;				// ambient-reflection coefficient [0,1]
uniform vec3 ks;				// specular
uniform float shininess;		// controls specular effect
uniform sampler2D diffuseMap;

void main( )
{
	vec3 n = normalize( vfNormal );
	vec3 s = normalize( lightPosition - vfPosition );
	vec3 v = normalize( -vfPosition );
	vec3 r = reflect( -s, n );
	
	vec3 outputColor = lightIntensity * (
		ka +
		kd * max( dot( s, n ), 0.0 ) +
		ks * pow( max( dot( r, v ), 0.0 ), shininess )
	);
		
	vec4 textureColor = texture( diffuseMap, vec2( vfTextureCoord.x, 1.0f - vfTextureCoord.y ) );

	gl_FragColor = vec4( outputColor, 1.0 ) * textureColor;
}