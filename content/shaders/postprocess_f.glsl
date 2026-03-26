#version 410 core

in vec2 vUV;
out vec4 fColor;

uniform sampler2D uSceneColor;
uniform sampler2D uBloomBlur;
uniform float uBloomStrength;
uniform float uVignetteStrength;
uniform float uGrainStrength;
uniform float uTime;

void main()
{
	vec3 color = texture( uSceneColor, vUV ).rgb;

	// Bloom — add blurred bright areas
	vec3 bloom = texture( uBloomBlur, vUV ).rgb;
	color += bloom * uBloomStrength;

	// Vignette — darken screen edges
	vec2 uv = vUV - 0.5;
	float vignette = 1.0 - dot( uv, uv ) * uVignetteStrength;
	color *= clamp( vignette, 0.0, 1.0 );

	// Film grain — subtle dither to prevent banding
	float grain = fract( sin( dot( vUV * uTime, vec2( 12.9898, 78.233 ) ) ) * 43758.5453 );
	color += ( grain - 0.5 ) * uGrainStrength;

	fColor = vec4( color, 1.0 );
}
