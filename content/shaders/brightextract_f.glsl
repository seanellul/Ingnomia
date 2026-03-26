#version 410 core

in vec2 vUV;
out vec4 fColor;

uniform sampler2D uSceneColor;
uniform float uThreshold;

void main()
{
	vec3 color = texture( uSceneColor, vUV ).rgb;
	float brightness = dot( color, vec3( 0.2126, 0.7152, 0.0722 ) );

	if( brightness > uThreshold )
	{
		fColor = vec4( color * ( brightness - uThreshold ) / brightness, 1.0 );
	}
	else
	{
		fColor = vec4( 0.0, 0.0, 0.0, 1.0 );
	}
}
