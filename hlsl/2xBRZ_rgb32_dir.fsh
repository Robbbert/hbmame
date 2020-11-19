	// Copyright (C) 2014-2016 DeSmuME team
	//
	// This file is free software: you can redistribute it and/or modify
	// it under the terms of the GNU General Public License as published by
	// the Free Software Foundation, either version 2 of the License, or
	// (at your option) any later version.
	//
	// This file is distributed in the hope that it will be useful,
	// but WITHOUT ANY WARRANTY; without even the implied warranty of
	// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	// GNU General Public License for more details.
	//
	// You should have received a copy of the GNU General Public License
	// along with the this software.  If not, see <http://www.gnu.org/licenses/>.

	#define BLEND_NONE 0
	#define BLEND_NORMAL 1
	#define BLEND_DOMINANT 2
	#define LUMINANCE_WEIGHT 1.0
	#define EQUAL_COLOR_TOLERANCE 30.0/255.0
	#define STEEP_DIRECTION_THRESHOLD 2.2
	#define DOMINANT_DIRECTION_THRESHOLD 3.6
	#define M_PI 3.1415926535897932384626433832795

	uniform sampler2D color_texture;
	uniform vec2 color_texture_pow2_sz;

	varying vec2 texCoord[25];

	float reduce(const vec3 color)
	{
		return dot(color, vec3(65536.0, 256.0, 1.0));
	}
	
	float DistYCbCr(const vec3 pixA, const vec3 pixB)
	{
		const vec3 w = vec3(0.2627, 0.6780, 0.0593);
		const float scaleB = 0.5 / (1.0 - w.b);
		const float scaleR = 0.5 / (1.0 - w.r);
		vec3 diff = pixA - pixB;
		float Y = dot(diff, w);
		float Cb = scaleB * (diff.b - Y);
		float Cr = scaleR * (diff.r - Y);
		
		return sqrt( ((LUMINANCE_WEIGHT * Y) * (LUMINANCE_WEIGHT * Y)) + (Cb * Cb) + (Cr * Cr) );
	}
	
	bool IsPixEqual(const vec3 pixA, const vec3 pixB)
	{
		return (DistYCbCr(pixA, pixB) < EQUAL_COLOR_TOLERANCE);
	}
	
	bool IsBlendingNeeded(const ivec4 blend)
	{
		return any(notEqual(blend, ivec4(BLEND_NONE)));
	}
	
	void ScalePixel(const ivec4 blend, const vec3 k[9], inout vec3 dst[4])
	{
		float v0 = reduce(k[0]);
		float v4 = reduce(k[4]);
		float v5 = reduce(k[5]);
		float v7 = reduce(k[7]);
		float v8 = reduce(k[8]);
		
		float dist_01_04 = DistYCbCr(k[1], k[4]);
		float dist_03_08 = DistYCbCr(k[3], k[8]);
		bool haveShallowLine = (STEEP_DIRECTION_THRESHOLD * dist_01_04 <= dist_03_08) && (v0 != v4) && (v5 != v4);
		bool haveSteepLine   = (STEEP_DIRECTION_THRESHOLD * dist_03_08 <= dist_01_04) && (v0 != v8) && (v7 != v8);
		bool needBlend = (blend[2] != BLEND_NONE);
		bool doLineBlend = (  blend[2] >= BLEND_DOMINANT ||
					       !((blend[1] != BLEND_NONE && !IsPixEqual(k[0], k[4])) ||
					         (blend[3] != BLEND_NONE && !IsPixEqual(k[0], k[8])) ||
					         (IsPixEqual(k[4], k[3]) && IsPixEqual(k[3], k[2]) && IsPixEqual(k[2], k[1]) && IsPixEqual(k[1], k[8]) && !IsPixEqual(k[0], k[2])) ) );
		
		vec3 blendPix = ( DistYCbCr(k[0], k[1]) <= DistYCbCr(k[0], k[3]) ) ? k[1] : k[3];
		dst[1] = mix(dst[1], blendPix, (needBlend && doLineBlend && haveSteepLine) ? 0.25 : 0.00);
		dst[2] = mix(dst[2], blendPix, (needBlend) ? ((doLineBlend) ? ((haveShallowLine) ? ((haveSteepLine) ? 5.0/6.0 : 0.75) : ((haveSteepLine) ? 0.75 : 0.50)) : 1.0 - (M_PI/4.0)) : 0.00);
		dst[3] = mix(dst[3], blendPix, (needBlend && doLineBlend && haveShallowLine) ? 0.25 : 0.00);
	}
	
	//---------------------------------------
	// Input Pixel Mapping:  --|21|22|23|--
	//                       19|06|07|08|09
	//                       18|05|00|01|10
	//                       17|04|03|02|11
	//                       --|15|14|13|--
	//
	// Output Pixel Mapping:     00|01
	//                           03|02
	
	void main()
	{
		vec3 src[16];
		src[ 0] = texture2D(color_texture, texCoord[ 0]).rgb;
		src[ 1] = texture2D(color_texture, texCoord[ 1]).rgb;
		src[ 2] = texture2D(color_texture, texCoord[ 2]).rgb;
		src[ 3] = texture2D(color_texture, texCoord[ 3]).rgb;
		src[ 4] = texture2D(color_texture, texCoord[ 4]).rgb;
		src[ 5] = texture2D(color_texture, texCoord[ 5]).rgb;
		src[ 6] = texture2D(color_texture, texCoord[ 6]).rgb;
		src[ 7] = texture2D(color_texture, texCoord[ 7]).rgb;
		src[ 8] = texture2D(color_texture, texCoord[ 8]).rgb;
		src[ 9] = texture2D(color_texture, texCoord[ 9]).rgb;
		src[10] = texture2D(color_texture, texCoord[10]).rgb;
		src[11] = texture2D(color_texture, texCoord[11]).rgb;
		src[12] = texture2D(color_texture, texCoord[12]).rgb;
		src[13] = texture2D(color_texture, texCoord[13]).rgb;
		src[14] = texture2D(color_texture, texCoord[14]).rgb;
		src[15] = texture2D(color_texture, texCoord[15]).rgb;
		
		float v[9];
		v[0] = reduce(src[0]);
		v[1] = reduce(src[1]);
		v[2] = reduce(src[2]);
		v[3] = reduce(src[3]);
		v[4] = reduce(src[4]);
		v[5] = reduce(src[5]);
		v[6] = reduce(src[6]);
		v[7] = reduce(src[7]);
		v[8] = reduce(src[8]);
		
		ivec4 blendResult = ivec4(BLEND_NONE);
		
		// Preprocess corners
		// Pixel Tap Mapping: --|--|--|--|--
		//                    --|--|07|08|--
		//                    --|05|00|01|10
		//                    --|04|03|02|11
		//                    --|--|14|13|--
		
		// Corner (1, 1)
		if ( !((v[0] == v[1] && v[3] == v[2]) || (v[0] == v[3] && v[1] == v[2])) )
		{
			float dist_03_01 = DistYCbCr(src[ 4], src[ 0]) + DistYCbCr(src[ 0], src[ 8]) + DistYCbCr(src[14], src[ 2]) + DistYCbCr(src[ 2], src[10]) + (4.0 * DistYCbCr(src[ 3], src[ 1]));
			float dist_00_02 = DistYCbCr(src[ 5], src[ 3]) + DistYCbCr(src[ 3], src[13]) + DistYCbCr(src[ 7], src[ 1]) + DistYCbCr(src[ 1], src[11]) + (4.0 * DistYCbCr(src[ 0], src[ 2]));
			bool dominantGradient = (DOMINANT_DIRECTION_THRESHOLD * dist_03_01) < dist_00_02;
			blendResult[2] = ((dist_03_01 < dist_00_02) && (v[0] != v[1]) && (v[0] != v[3])) ? ((dominantGradient) ? BLEND_DOMINANT : BLEND_NORMAL) : BLEND_NONE;
		}
		
		
		// Pixel Tap Mapping: --|--|--|--|--
		//                    --|06|07|--|--
		//                    18|05|00|01|--
		//                    17|04|03|02|--
		//                    --|15|14|--|--
		// Corner (0, 1)
		if ( !((v[5] == v[0] && v[4] == v[3]) || (v[5] == v[4] && v[0] == v[3])) )
		{
			vec3 src17 = texture2D(color_texture, texCoord[17]).rgb;
			vec3 src18 = texture2D(color_texture, texCoord[18]).rgb;
			float dist_04_00 = DistYCbCr(src17  , src[ 5]) + DistYCbCr(src[ 5], src[ 7]) + DistYCbCr(src[15], src[ 3]) + DistYCbCr(src[ 3], src[ 1]) + (4.0 * DistYCbCr(src[ 4], src[ 0]));
			float dist_05_03 = DistYCbCr(src18  , src[ 4]) + DistYCbCr(src[ 4], src[14]) + DistYCbCr(src[ 6], src[ 0]) + DistYCbCr(src[ 0], src[ 2]) + (4.0 * DistYCbCr(src[ 5], src[ 3]));
			bool dominantGradient = (DOMINANT_DIRECTION_THRESHOLD * dist_05_03) < dist_04_00;
			blendResult[3] = ((dist_04_00 > dist_05_03) && (v[0] != v[5]) && (v[0] != v[3])) ? ((dominantGradient) ? BLEND_DOMINANT : BLEND_NORMAL) : BLEND_NONE;
		}
		
		// Pixel Tap Mapping: --|--|22|23|--
		//                    --|06|07|08|09
		//                    --|05|00|01|10
		//                    --|--|03|02|--
		//                    --|--|--|--|--
		// Corner (1, 0)
		if ( !((v[7] == v[8] && v[0] == v[1]) || (v[7] == v[0] && v[8] == v[1])) )
		{
			vec3 src22 = texture2D(color_texture, texCoord[22]).rgb;
			vec3 src23 = texture2D(color_texture, texCoord[23]).rgb;
			float dist_00_08 = DistYCbCr(src[ 5], src[ 7]) + DistYCbCr(src[ 7], src23  ) + DistYCbCr(src[ 3], src[ 1]) + DistYCbCr(src[ 1], src[ 9]) + (4.0 * DistYCbCr(src[ 0], src[ 8]));
			float dist_07_01 = DistYCbCr(src[ 6], src[ 0]) + DistYCbCr(src[ 0], src[ 2]) + DistYCbCr(src22  , src[ 8]) + DistYCbCr(src[ 8], src[10]) + (4.0 * DistYCbCr(src[ 7], src[ 1]));
			bool dominantGradient = (DOMINANT_DIRECTION_THRESHOLD * dist_07_01) < dist_00_08;
			blendResult[1] = ((dist_00_08 > dist_07_01) && (v[0] != v[7]) && (v[0] != v[1])) ? ((dominantGradient) ? BLEND_DOMINANT : BLEND_NORMAL) : BLEND_NONE;
		}
		
		// Pixel Tap Mapping: --|21|22|--|--
		//                    19|06|07|08|--
		//                    18|05|00|01|--
		//                    --|04|03|--|--
		//                    --|--|--|--|--
		// Corner (0, 0)
		if ( !((v[6] == v[7] && v[5] == v[0]) || (v[6] == v[5] && v[7] == v[0])) )
		{
			vec3 src18 = texture2D(color_texture, texCoord[18]).rgb;
			vec3 src19 = texture2D(color_texture, texCoord[19]).rgb;
			vec3 src21 = texture2D(color_texture, texCoord[21]).rgb;
			vec3 src22 = texture2D(color_texture, texCoord[22]).rgb;
			float dist_05_07 = DistYCbCr(src18  , src[ 6]) + DistYCbCr(src[ 6], src22  ) + DistYCbCr(src[ 4], src[ 0]) + DistYCbCr(src[ 0], src[ 8]) + (4.0 * DistYCbCr(src[ 5], src[ 7]));
			float dist_06_00 = DistYCbCr(src19  , src[ 5]) + DistYCbCr(src[ 5], src[ 3]) + DistYCbCr(src21  , src[ 7]) + DistYCbCr(src[ 7], src[ 1]) + (4.0 * DistYCbCr(src[ 6], src[ 0]));
			bool dominantGradient = (DOMINANT_DIRECTION_THRESHOLD * dist_05_07) < dist_06_00;
			blendResult[0] = ((dist_05_07 < dist_06_00) && (v[0] != v[5]) && (v[0] != v[7])) ? ((dominantGradient) ? BLEND_DOMINANT : BLEND_NORMAL) : BLEND_NONE;
		}
		
		vec3 dst[4];
		dst[0] = src[0];
		dst[1] = src[0];
		dst[2] = src[0];
		dst[3] = src[0];
		
		// Scale pixel
		if (IsBlendingNeeded(blendResult))
		{
			vec3 k[9];
			vec3 tempDst3;
			
			k[0] = src[0];
			k[1] = src[1];
			k[2] = src[2];
			k[3] = src[3];
			k[4] = src[4];
			k[5] = src[5];
			k[6] = src[6];
			k[7] = src[7];
			k[8] = src[8];
			ScalePixel(blendResult.xyzw, k, dst);
			
			k[1] = src[7];
			k[2] = src[8];
			k[3] = src[1];
			k[4] = src[2];
			k[5] = src[3];
			k[6] = src[4];
			k[7] = src[5];
			k[8] = src[6];
			tempDst3 = dst[3];
			dst[3] = dst[2];
			dst[2] = dst[1];
			dst[1] = dst[0];
			dst[0] = tempDst3;
			ScalePixel(blendResult.wxyz, k, dst);
			
			k[1] = src[5];
			k[2] = src[6];
			k[3] = src[7];
			k[4] = src[8];
			k[5] = src[1];
			k[6] = src[2];
			k[7] = src[3];
			k[8] = src[4];
			tempDst3 = dst[3];
			dst[3] = dst[2];
			dst[2] = dst[1];
			dst[1] = dst[0];
			dst[0] = tempDst3;
			ScalePixel(blendResult.zwxy, k, dst);
			
			k[1] = src[3];
			k[2] = src[4];
			k[3] = src[5];
			k[4] = src[6];
			k[5] = src[7];
			k[6] = src[8];
			k[7] = src[1];
			k[8] = src[2];
			tempDst3 = dst[3];
			dst[3] = dst[2];
			dst[2] = dst[1];
			dst[1] = dst[0];
			dst[0] = tempDst3;
			ScalePixel(blendResult.yzwx, k, dst);
			
			// Rotate the destination pixels back to 0 degrees.
			tempDst3 = dst[3];
			dst[3] = dst[2];
			dst[2] = dst[1];
			dst[1] = dst[0];
			dst[0] = tempDst3;
		}
		
		vec2 f = step(0.5, fract(texCoord[0] * color_texture_pow2_sz));
		gl_FragColor.rgb = mix( mix(dst[0], dst[1], f.x),
		                        mix(dst[3], dst[2], f.x), f.y );
		gl_FragColor.a = 1.0;
	}