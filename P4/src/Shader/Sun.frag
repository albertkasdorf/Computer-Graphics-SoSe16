#version 330 core

in vec2 diffuseMapCoord;
in vec3 vfColor;

out vec4 Color;

uniform sampler2D diffuseMap;
uniform sampler2D distortionMap;

uniform float time;
uniform int blendMode;

void main()
{
	// Koordinaten zum Bestimmen der auszulesenden Stelle in der Verzerrungstextur
	vec2 distortionMapCoord = diffuseMapCoord;

	// Verschiebe die Verzerrungskoordinaten abhängig von der Zeit nach oben
	// Die Konstante beeinflusst die Geschwindigkeit
	distortionMapCoord.y -= time * 0.2;

	// Lese Werte aus der Verzerrungstextur aus
	vec4 distortionValue = texture( distortionMap, vec2( distortionMapCoord.x, 1.0f - distortionMapCoord.y ) );

	// Verwende die ersten zwei Komponenten als Verschiebung
	vec2 distortionOffset = distortionValue.xy;

	// Von OpenGL normalisierte Werte liegen im Intervall [0,1]
	// Wir wollen eine Verschiebung in beide Richtungen, also im Intervall [-1,1]
	distortionOffset -= vec2( 0.5, 0.5 );
	distortionOffset *= 2;

	// Skaliere die Verschiebung und bestimme damit die Stärke der Verzerrung
	distortionOffset *= 0.05;

	// Mildere den Effekt im oberen Bereich der Textur (optional)
	distortionOffset *= (1 - diffuseMapCoord.y);

	// Berechne die verschobenen Texturkoordinaten
	vec2 distortedCoord = diffuseMapCoord + distortionOffset;

	// Lese Farben aus Texturen aus
	vec4 color1 = texture( diffuseMap, vec2( diffuseMapCoord.x, 1.0f - diffuseMapCoord.y ) );
	vec4 color2 = texture( diffuseMap, vec2( distortedCoord.x, 1.0f - distortedCoord.y ) );

	switch( blendMode )
	{
		// Multiplikatives Überblenden der zwei Farben (blend mode multiply)
		// Die Multiplikation zwischen zwei Vektoren mit * erfolgt komponentenweise,
		// also (a, b, c, d) * (e, f , g, h) = (a*e, b*f, c*g, d*h)
		default:
		case 0:
			Color = color1 * color2;
			break;

		// screen
		case 1:
			Color = vec4(1.0) - ((vec4(1.0) - color2) * (vec4(1.0) - color1));
			break;

		// darken
		case 2:
			Color = min( color2, color1 );
			break;

		// lighten
		case 3:
			Color = max( color2, color1 );
			break;

		// difference
		case 4:
			Color = abs( color1 - color2 );
			break;

		// negation
		case 5:
			Color = vec4(1.0) - abs( vec4(1.0) - color1 - color2 );
			break;

		// exclusion
		case 6:
			Color = color1 + color2 - (2.0*color1*color2);
			break;

		// dodge:
		case 7:
			Color = color1 / (vec4(1.0) - color2);
			break;

		// burn:
		case 8:
			Color = vec4(1.0) - (vec4(1.0) - color1) / color2;
			break;
	}
}