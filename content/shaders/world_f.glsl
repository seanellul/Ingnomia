#version 410 core

#define TF_NONE                 0x00000000u
#define TF_WALKABLE             0x00000001u
#define TF_UNDISCOVERED         0x00000002u
#define TF_SUNLIGHT             0x00000004u
#define TF_WET                  0x00000008u
#define TF_GRASS                0x00000010u
#define TF_NOPASS               0x00000020u
#define TF_BLOCKED              0x00000040u
#define TF_DOOR                 0x00000080u
#define TF_STOCKPILE            0x00000100u
#define TF_GROVE                0x00000200u
#define TF_FARM                 0x00000400u
#define TF_TILLED               0x00000800u
#define TF_WORKSHOP             0x00001000u
#define TF_ROOM                 0x00002000u
#define TF_LAVA                 0x00004000u
#define TF_WATER                0x00008000u
#define TF_JOB_FLOOR            0x00010000u
#define TF_JOB_WALL             0x00020000u
#define TF_JOB_BUSY_FLOOR       0x00040000u
#define TF_JOB_BUSY_WALL        0x00080000u
#define TF_MOUSEOVER            0x00100000u
#define TF_WALKABLEANIMALS      0x00200000u
#define TF_WALKABLEMONSTERS     0x00400000u
#define TF_PASTURE              0x00800000u
#define TF_INDIRECT_SUNLIGHT    0x01000000u
#define TF_TRANSPARENT          0x40000000u
#define TF_OVERSIZE             0x80000000u

#define WATER_TOP               0x01u
#define WATER_EDGE              0x02u
#define WATER_WALL              0x10u
#define WATER_FLOOR             0x20u
#define WATER_ONFLOOR           0x40u

#define CAT(x, y) CAT_(x, y)
#define CAT_(x, y) x ## y
#define UNPACKSPRITE(alias, src) uint CAT(alias, ID) = src & 0xffffu; uint CAT(alias, Flags) = src >> 16u;
layout(location = 0) noperspective in vec2 vTexCoords;
layout(location = 1) flat in uvec4  block1;
layout(location = 2) flat in uvec4  block2;
layout(location = 3) flat in uvec4  block3;
layout(location = 4) flat in uvec2  vTileExtra; // x=tileZ, y=aoFlags

layout(location = 0) out vec4 fColor;

uniform sampler2DArray uTexture[16];

uniform int uTickNumber;

uniform int uUndiscoveredTex;
uniform int uWaterTex;

uniform int uWorldRotation;
uniform bool uOverlay;
uniform bool uDebug;
uniform bool uWallsLowered;
uniform float uDaylight;
uniform float uLightMin;
uniform bool uPaintFrontToBack;

uniform bool uShowJobs;

uniform float uViewLevel;
uniform float uRenderDepth;
uniform int uSeason;
uniform int uWeather;          // 0=clear, 1=storm, 2=heatwave, 3=coldsnap
uniform float uWeatherIntensity; // 0-1 ramp

const float waterAlpha = 0.6;
const float flSize =  ( 1.0 / 32. );
const int rightWallOffset = 4;
const int leftWallOffset = 8;

const vec3 perceivedBrightness = vec3(0.299, 0.587, 0.114);

vec4 getTexel( uint spriteID, uint rot, uint animFrame )
{
	uint absoluteId = ( spriteID + animFrame ) * 4;
	uint tex = absoluteId / 2048;
	uint localBaseId = absoluteId % 2048;
	uint localID = localBaseId + rot;
	
	ivec3 samplePos = ivec3( vTexCoords.x * 32, vTexCoords.y * 64, localID);

	// Need to unroll each access to texelFetch with a different element from uTexture into a distinct instruction
	// Otherwise we are triggering a bug on AMD GPUs, where threads start sampling from the wrong texture
	#define B(X) case X: return texelFetch( uTexture[X], samplePos, 0);
	#define C(X) B(X) B(X+1) B(X+2) B(X+3)
	#define D(X) C(X) C(X+4) C(X+8) C(X+12)
	switch(tex)
	{
		D(0)
	}
	#undef D
	#undef C
	#undef B
}

void main()
{
	vec4 texel = vec4( 0,  0,  0, 0 );
	
	uint rot = 0;
	uint spriteID = 0;
	uint animFrame = 0;
	
	UNPACKSPRITE(floorSprite, block1.x);
	UNPACKSPRITE(jobFloorSprite, block1.y);
	UNPACKSPRITE(wallSprite, block1.z);
	UNPACKSPRITE(jobWallSprite, block1.w);
	UNPACKSPRITE(itemSprite, block2.x);
	UNPACKSPRITE(creatureSprite, block2.y);

	uint vFluidLevelPacked1 = block2.z;
	bool uIsWall = ( block2.w != 0 );

	uint vFlags = block3.x;
	uint vFlags2 = block3.y;

	uint vLightLevel = block3.z;
	uint vVegetationLevel = block3.w;

	uint vFluidLevel = (vFluidLevelPacked1 >> 0) & 0xffu;
	uint vFluidLevelLeft = (vFluidLevelPacked1 >> 8) & 0xffu;
	uint vFluidLevelRight = (vFluidLevelPacked1 >> 16) & 0xffu;
	uint vFluidFlags = (vFluidLevelPacked1 >> 24) & 0xffu;

	
	if( !uIsWall )
	{
		if( ( vFlags & TF_UNDISCOVERED ) != 0 && !uDebug )
		{
			if( !uWallsLowered )
			{
				vec4 tmpTexel = getTexel( uUndiscoveredTex / 4 + 2, 0, 0 );

				// Fully replace — don't show any content through fog of war
				texel.rgb = tmpTexel.rgb;
				texel.a = tmpTexel.a;
			}
			// Skip all further floor rendering for undiscovered tiles
		}
		else
		{
			spriteID = floorSpriteID;
			if( spriteID != 0 )
			{
				rot = floorSpriteFlags & 3u;
				rot = ( rot + uWorldRotation ) % 4;
				
				if( ( floorSpriteFlags & 4u ) == 4 )
				{
					animFrame = ( uTickNumber / 10 ) % 4;
				}
				
				vec4 tmpTexel = getTexel( spriteID, rot, animFrame );
				
				if( ( vFlags & TF_GRASS ) != 0 )
				{
					vec4 roughFloor = getTexel( uUndiscoveredTex / 4 + 3, 0, 0 );
					float interpol = 1.0 - ( float( vVegetationLevel ) / 100. );
					tmpTexel.rgb = mix( tmpTexel.rgb, roughFloor.rgb, interpol * roughFloor.a );
					texel.a = max(tmpTexel.a , roughFloor.a);
				}

				texel.rgb = mix( texel.rgb, tmpTexel.rgb, tmpTexel.a );
				texel.a = max(texel.a , tmpTexel.a);
			}
			
			spriteID = jobFloorSpriteID;
			animFrame = 0;
			if( uShowJobs && ( spriteID != 0 )  )
			{
				rot = jobFloorSpriteFlags & 3u;
				rot = ( rot + uWorldRotation ) % 4;
				
				vec4 tmpTexel = getTexel( spriteID, rot, animFrame );
				
				if( ( vFlags & TF_JOB_BUSY_FLOOR ) != 0 )
				{
					tmpTexel.r *= 0.3;
					tmpTexel.g *= 0.7;
					tmpTexel.b *= 0.3;
				}
				else
				{
					tmpTexel.r *= 0.7;
					tmpTexel.g *= 0.7;
					tmpTexel.b *= 0.3;
				}

				texel.rgba = tmpTexel.rgba;
			}

			if( uOverlay && 0 != ( vFlags & ( TF_STOCKPILE | TF_FARM | TF_GROVE | TF_PASTURE | TF_WORKSHOP | TF_ROOM | TF_NOPASS ) ) )
			{
				vec3 roomColor = vec3( 0.0 );
			
				if( ( vFlags & TF_STOCKPILE ) != 0 ) //stockpile
				{
					roomColor = vec3(1, 1, 0);
				}
				
				else if( ( vFlags & TF_FARM ) != 0 ) //farm
				{
					roomColor = vec3(0.5, 0, 1);
				}
				else if( ( vFlags & TF_GROVE ) != 0 ) //grove
				{
					roomColor = vec3(0, 1, 0.5);
				}
				else if( ( vFlags & TF_PASTURE ) != 0 ) 
				{
					roomColor = vec3(0, 0.9, 0.9);
				}
				else if( ( vFlags & TF_WORKSHOP ) != 0 ) //workshop
				{
					if( ( vFlags & TF_BLOCKED ) != 0 )
					{
						roomColor = vec3(1, 0, 0);
					}
					else
					{
						roomColor = vec3(1, 1, 0);
					}
				}
				else if( ( vFlags & TF_ROOM ) != 0 ) //room
				{
					roomColor = vec3(0, 0, 1);
				}
				else if( ( vFlags & TF_NOPASS ) != 0 ) //room
				{
					roomColor = vec3(1, 0, 0);
				}
				if( texel.a != 0 )
				{
					float brightness = dot(texel.rgb, perceivedBrightness.xyz);
					// Preserve perceived brightness of original pixel during tinting, but drop saturation partially
					texel.rgb = mix( roomColor, mix( texel.rgb, vec3(1,1,1) * brightness, 0.7), 0.7);
				}
				
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// water related calculations
		//
		////////////////////////////////////////////////////////////////////////////////////////////////////
		if( ( vFluidFlags & ( WATER_FLOOR | WATER_EDGE ) ) != 0 )
		{
			bool renderAboveFloor = ( vFluidFlags & WATER_ONFLOOR ) != 0;
			int startLevel =  renderAboveFloor ? 2 : int(min(vFluidLevel, 2));
			int referenceLevel = int(vTexCoords.x < 0.5 ? vFluidLevelLeft : vFluidLevelRight);
			int offset = vTexCoords.x < 0.5 ? leftWallOffset : rightWallOffset;

			float fl = float( startLevel - 2 ) * flSize;

			vec4 tmpTexel = vec4( 0, 0, 0, 0 );

			if( ( vFluidFlags & WATER_FLOOR ) != 0 )
			{
				float y = vTexCoords.y + fl;
				tmpTexel = texture( uTexture[0], vec3( vec2( vTexCoords.x, y ), uWaterTex ) );
			}

			if( ( vFluidFlags & WATER_EDGE ) != 0 )
			{
				float y = vTexCoords.y + fl;
				for(int i = startLevel; i > referenceLevel; --i)
				{
					y -= flSize;
					tmpTexel += texture( uTexture[0], vec3( vec2( vTexCoords.x, y ), uWaterTex + offset ) );
				}
			}

			// Turn into slight tint instead
			if( renderAboveFloor && vFluidLevel == 1 )
			{
				tmpTexel.a *= 0.5;
			}


			texel.rgb = mix( texel.rgb, tmpTexel.rgb, waterAlpha * tmpTexel.a );
			texel.a = max(texel.a , tmpTexel.a);

			// Water caustics — animated dappled light on shallow water
			if( vFluidLevel <= 4u && vFluidLevel >= 1u )
			{
				float time = float(uTickNumber) * 0.05;
				vec2 p = vTexCoords * 4.0 + vec2( float(vTileExtra.x) * 0.3, float(vTileExtra.x) * 0.7 );
				float c1 = sin( p.x * 3.0 + time ) * cos( p.y * 2.5 - time * 0.7 );
				float c2 = sin( p.x * 2.0 - time * 0.5 + 1.5 ) * cos( p.y * 3.5 + time * 0.3 );
				float caustic = ( c1 + c2 ) * 0.5 + 0.5;
				caustic = caustic * caustic;
				float shallowFactor = 1.0 - float( vFluidLevel ) / 5.0;
				texel.rgb += vec3( 0.15, 0.2, 0.25 ) * caustic * shallowFactor * 0.4;
			}
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// end water related calculations
		//
		////////////////////////////////////////////////////////////////////////////////////////////////////


	}
	else
	{
		if( ( vFlags & TF_UNDISCOVERED ) != 0 && !uDebug )
		{
			if( !uWallsLowered )
			{
				vec4 tmpTexel = getTexel( uUndiscoveredTex / 4, 0, 0 );

				// Fully replace — don't show any content through fog of war
				texel.rgb = tmpTexel.rgb;
				texel.a = tmpTexel.a;
			}
			// Skip all further wall rendering for undiscovered tiles
		}
		else
		{
			spriteID = wallSpriteID;
			if( spriteID != 0 )
			{
				rot = wallSpriteFlags & 3u;
				rot = ( rot + uWorldRotation ) % 4;

				if( ( wallSpriteFlags & 4u ) == 4 )
				{
					animFrame = ( uTickNumber / 3 ) % 4;
				}
				if( uWallsLowered )
				{
					animFrame = 0;
					if( ( wallSpriteFlags & 8u ) == 8 )
					{
						spriteID = spriteID + 1;
					}
				}
				vec4 tmpTexel = getTexel( spriteID, rot, animFrame );
				
				texel.rgb = mix( texel.rgb, tmpTexel.rgb, tmpTexel.a );
				texel.a = max(texel.a , tmpTexel.a);
			}
			
			spriteID = itemSpriteID;
			animFrame = 0;
			if( spriteID != 0 )
			{
				rot = itemSpriteID & 3u;
				rot = ( rot + uWorldRotation ) % 4;
				
				vec4 tmpTexel = getTexel( spriteID, rot, animFrame );
				
				texel.rgb = mix( texel.rgb, tmpTexel.rgb, tmpTexel.a );
				texel.a = max(texel.a , tmpTexel.a);
			}
		}
	
		spriteID = jobWallSpriteID;
		animFrame = 0;
		if( uShowJobs && spriteID != 0 )
		{
			rot = jobWallSpriteFlags & 3u;
			rot = ( rot + uWorldRotation ) % 4;
			
			vec4 tmpTexel = getTexel( spriteID, rot, animFrame );
			
			if( ( vFlags & TF_JOB_BUSY_WALL ) != 0 )
			{
				tmpTexel.r *= 0.3;
				tmpTexel.g *= 0.7;
				tmpTexel.b *= 0.3;
			}
			else
			{
				tmpTexel.r *= 0.7;
				tmpTexel.g *= 0.7;
				tmpTexel.b *= 0.3;
			}

			texel.rgba = tmpTexel.rgba;
		}

	
		// Only render creatures on discovered tiles
		if( ( vFlags & TF_UNDISCOVERED ) == 0 || uDebug )
		{
			spriteID = creatureSpriteID;
			animFrame = 0;
			if( spriteID != 0 )
			{
				rot = creatureSpriteFlags & 3u;
				rot = ( rot + uWorldRotation ) % 4;

				vec4 tmpTexel = getTexel( spriteID, rot, animFrame );

				texel.rgb = mix( texel.rgb, tmpTexel.rgb, tmpTexel.a );
				texel.a = max(texel.a , tmpTexel.a);
			}
		}
		
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// water related calculations
		//
		////////////////////////////////////////////////////////////////////////////////////////////////////
		if( ( vFluidFlags & ( WATER_TOP | WATER_WALL ) ) != 0 && vFluidLevel > 2 )
		{
			int startLevel = int(vFluidLevel - 2);
			int referenceLevel = int( vTexCoords.x < 0.5 ? max(2, vFluidLevelLeft) : max(2, vFluidLevelRight) ) - 2;
			int offset = vTexCoords.x < 0.5 ? leftWallOffset : rightWallOffset;

			float fl = float( startLevel ) * flSize;

			vec4 tmpTexel = vec4( 0, 0, 0, 0 );
			
			if( ( vFluidFlags & WATER_TOP ) != 0 )
			{
				float y = vTexCoords.y + fl;
				tmpTexel = texture( uTexture[0], vec3( vec2( vTexCoords.x, y ), uWaterTex ) );
			}

			if( ( vFluidFlags & WATER_WALL ) != 0)
			{
				float y = vTexCoords.y + fl;
				for(int i = startLevel; i > referenceLevel; --i)
				{
					y -= flSize;
					tmpTexel += texture( uTexture[0], vec3( vec2( vTexCoords.x, y ), uWaterTex + offset ) );
				}
			}
			
			texel.rgb = mix( texel.rgb, tmpTexel.rgb, waterAlpha * tmpTexel.a );
			texel.a = max(texel.a , tmpTexel.a);
		}
		////////////////////////////////////////////////////////////////////////////////////////////////////
		//
		// end water related calculations
		//
		////////////////////////////////////////////////////////////////////////////////////////////////////
	}

	if( texel.a <= 0 )
	{
		discard;
	}
	else if(uPaintFrontToBack)
	{
		// Flush to 1 in case of front-to-back rendering
		texel.a = 1;
	}
	
	
	if( !uDebug )
	{
		float torchLight = float( vLightLevel ) / 20.;
		float light = torchLight;
		bool hasSunlight = ( vFlags & ( TF_SUNLIGHT | TF_INDIRECT_SUNLIGHT ) ) != 0;

		if( hasSunlight )
		{
			light = max( light, uDaylight );
		}

		float brightness = dot(texel.rgb, perceivedBrightness.xyz);
		float lightMult = ( 1 - uLightMin ) * light + uLightMin;
		float minSaturation = 0.1;
		float saturation = ( 1 - minSaturation ) * light + minSaturation;
		// Desaturate, then darken
		texel.rgb = mix(brightness * vec3(1,1,1), texel.rgb, saturation) * lightMult;

		// Underground cave color — subtle purple tint in dark underground areas
		if( !hasSunlight && light < 0.2 )
		{
			vec3 caveColor = vec3( 0.12, 0.08, 0.15 );
			float caveBlend = ( 0.2 - light ) / 0.2 * 0.15;
			texel.rgb = mix( texel.rgb, caveColor, caveBlend );
		}

		// Night blue tint — shift outdoor areas toward cool blue at night
		float nightAmount = clamp( 1.0 - uDaylight, 0.0, 1.0 );
		vec3 nightTint = vec3( 0.65, 0.72, 1.0 ); // cool blue moonlight
		texel.rgb = mix( texel.rgb, texel.rgb * nightTint, nightAmount * 0.45 );

		// Torch warmth — artificial light gives warm orange tint
		if( torchLight > 0.01 && !hasSunlight )
		{
			vec3 torchTint = vec3( 1.0, 0.88, 0.65 ); // warm orange
			float warmth = min( torchLight, 1.0 ) * 0.35;
			texel.rgb = mix( texel.rgb, texel.rgb * torchTint, warmth );
		}

		// Seasonal color grading
		vec3 seasonGrade;
		float seasonStrength;
		if( uSeason == 0 )      { seasonGrade = vec3( 0.90, 1.10, 0.85 ); seasonStrength = 0.3; }  // spring: lush green
		else if( uSeason == 1 ) { seasonGrade = vec3( 1.10, 1.02, 0.85 ); seasonStrength = 0.25; } // summer: warm golden
		else if( uSeason == 2 ) { seasonGrade = vec3( 1.15, 0.85, 0.60 ); seasonStrength = 0.4; }  // autumn: strong amber
		else                    { seasonGrade = vec3( 0.80, 0.88, 1.15 );  seasonStrength = 0.35; } // winter: cold blue-grey
		texel.rgb = mix( texel.rgb, texel.rgb * seasonGrade, seasonStrength );

		// Weather effects on sunlit tiles
		if( hasSunlight && uWeatherIntensity > 0.0 )
		{
			float wi = uWeatherIntensity;
			if( uWeather == 1 ) // Storm — darken, increase saturation, blue tint
			{
				texel.rgb *= mix( 1.0, 0.7, wi );
				float grey = dot( texel.rgb, perceivedBrightness );
				texel.rgb = mix( vec3(grey), texel.rgb, 1.0 + wi * 0.3 ); // boost saturation
				texel.rgb = mix( texel.rgb, texel.rgb * vec3( 0.8, 0.85, 1.0 ), wi * 0.3 );
			}
			else if( uWeather == 2 ) // HeatWave — warm, slight haze
			{
				texel.rgb = mix( texel.rgb, texel.rgb * vec3( 1.1, 1.0, 0.85 ), wi * 0.3 );
			}
			else if( uWeather == 3 ) // ColdSnap — brighten, desaturate, blue shift
			{
				float grey = dot( texel.rgb, perceivedBrightness );
				texel.rgb = mix( texel.rgb, vec3(grey), wi * 0.3 ); // desaturate
				texel.rgb = mix( texel.rgb, texel.rgb * vec3( 0.9, 0.95, 1.15 ), wi * 0.4 );
				texel.rgb *= mix( 1.0, 1.1, wi ); // brighten slightly
			}
		}

		// Depth fog — fade tiles below view level toward atmospheric haze
		if( uRenderDepth > 1.0 )
		{
			float zBelow = uViewLevel - float(vTileExtra.x);
			float depthFade = clamp( ( zBelow - 1.0 ) / ( uRenderDepth - 1.0 ), 0.0, 1.0 );
			depthFade *= depthFade; // quadratic — gentle near camera, strong far away
			float fogStrength = 0.6;
			if( uWeather == 1 || uWeather == 3 )
				fogStrength += uWeatherIntensity * 0.2;
			// Fog color is always brighter than the darkest tiles so it reads as haze
			vec3 nightFog = vec3( 0.10, 0.10, 0.18 ); // visible blue-grey even at night
			vec3 dayFog   = vec3( 0.45, 0.50, 0.60 );  // hazy blue during day
			vec3 fogColor = mix( nightFog, dayFog, uDaylight );
			texel.rgb = mix( texel.rgb, fogColor, depthFade * fogStrength );
		}

		// Ambient occlusion — darken edges near solid walls
		uint aoFlags = vTileExtra.y;
		if( aoFlags != 0u )
		{
			// AO bits: 0=above, 1=north(y-), 2=east(x+), 3=south(y+), 4=west(x-)
			// In isometric view, texcoords map: x=0 is left edge, x=1 is right edge
			// y=0 is top, y=1 is bottom
			float aoStrength = 0.25;
			float ao = 0.0;

			// Above — darken entire tile slightly
			if( ( aoFlags & 0x01u ) != 0u )
				ao += 0.15;

			// Directional edge darkening — apply based on which direction has a wall
			// Use world rotation to rotate the AO directions
			uint rotatedAO = aoFlags >> 1; // bits 0-3 = N/E/S/W
			// Rotate N/E/S/W by world rotation
			rotatedAO = ( ( rotatedAO >> uint(uWorldRotation) ) | ( rotatedAO << ( 4u - uint(uWorldRotation) ) ) ) & 0x0fu;

			// After rotation: bit0=screen-north, bit1=screen-east, bit2=screen-south, bit3=screen-west
			// In isometric: screen-north = top-right, screen-east = bottom-right
			// screen-south = bottom-left, screen-west = top-left
			float northAO = float( ( rotatedAO & 0x01u ) != 0u );
			float eastAO  = float( ( rotatedAO & 0x02u ) != 0u );
			float southAO = float( ( rotatedAO & 0x04u ) != 0u );
			float westAO  = float( ( rotatedAO & 0x08u ) != 0u );

			// Edge proximity: how close to each edge (0 = at edge, 1 = far away)
			float topDist    = vTexCoords.y;         // 0 at top edge
			float bottomDist = 1.0 - vTexCoords.y;   // 0 at bottom edge
			float leftDist   = vTexCoords.x;          // 0 at left edge
			float rightDist  = 1.0 - vTexCoords.x;    // 0 at right edge

			// Isometric mapping: top-right = north, bottom-right = east, etc.
			float edgeFalloff = 0.25;
			ao += northAO * smoothstep( edgeFalloff, 0.0, rightDist ) * smoothstep( edgeFalloff, 0.0, topDist ) * aoStrength;
			ao += eastAO  * smoothstep( edgeFalloff, 0.0, rightDist ) * smoothstep( edgeFalloff, 0.0, bottomDist ) * aoStrength;
			ao += southAO * smoothstep( edgeFalloff, 0.0, leftDist )  * smoothstep( edgeFalloff, 0.0, bottomDist ) * aoStrength;
			ao += westAO  * smoothstep( edgeFalloff, 0.0, leftDist )  * smoothstep( edgeFalloff, 0.0, topDist ) * aoStrength;

			texel.rgb *= ( 1.0 - min( ao, 0.5 ) );
		}
	}

	// Mouseover rim highlight
	if( ( vFlags & TF_MOUSEOVER ) != 0 )
	{
		float edgeDist = min( min( vTexCoords.x, 1.0 - vTexCoords.x ), min( vTexCoords.y, 1.0 - vTexCoords.y ) );
		float rim = smoothstep( 0.0, 0.08, edgeDist );
		texel.rgb = mix( vec3( 1.0, 0.95, 0.7 ), texel.rgb, rim * 0.7 + 0.3 );
	}

	fColor = texel;
}
