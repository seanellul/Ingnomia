#version 410 core

in vec2 vUV;
out vec4 fColor;

uniform sampler2D uImage;
uniform bool uHorizontal;

// 9-tap gaussian weights (sigma ~4)
const float weight[5] = float[]( 0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216 );

void main()
{
	vec2 texelSize = 1.0 / vec2( textureSize( uImage, 0 ) );
	vec3 result = texture( uImage, vUV ).rgb * weight[0];

	if( uHorizontal )
	{
		for( int i = 1; i < 5; ++i )
		{
			result += texture( uImage, vUV + vec2( texelSize.x * float(i), 0.0 ) ).rgb * weight[i];
			result += texture( uImage, vUV - vec2( texelSize.x * float(i), 0.0 ) ).rgb * weight[i];
		}
	}
	else
	{
		for( int i = 1; i < 5; ++i )
		{
			result += texture( uImage, vUV + vec2( 0.0, texelSize.y * float(i) ) ).rgb * weight[i];
			result += texture( uImage, vUV - vec2( 0.0, texelSize.y * float(i) ) ).rgb * weight[i];
		}
	}

	fColor = vec4( result, 1.0 );
}
