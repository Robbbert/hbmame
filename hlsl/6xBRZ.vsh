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

	//---------------------------------------
	// Input Pixel Mapping:  20|21|22|23|24
	//                       19|06|07|08|09
	//                       18|05|00|01|10
	//                       17|04|03|02|11
	//                       16|15|14|13|12
	
	uniform vec2 color_texture_pow2_sz;

	varying vec2 texCoord[25];
	
	void main()
	{
		gl_Position = ftransform();

		float dx = 1.0 / color_texture_pow2_sz.x;
		float dy = 1.0 / color_texture_pow2_sz.y;

		vec2 inTexCoord0 = (gl_TextureMatrix[0] * gl_MultiTexCoord0).xy;

		texCoord[20] = inTexCoord0 + vec2(-2.0 * dx,-2.0 * dy);
		texCoord[21] = inTexCoord0 + vec2(      -dx,-2.0 * dy);
		texCoord[22] = inTexCoord0 + vec2( 0.0     ,-2.0 * dy);
		texCoord[23] = inTexCoord0 + vec2(       dx,-2.0 * dy);
		texCoord[24] = inTexCoord0 + vec2( 2.0 * dx,-2.0 * dy);
		
		texCoord[19] = inTexCoord0 + vec2(-2.0 * dx,      -dy);
		texCoord[ 6] = inTexCoord0 + vec2(      -dx,      -dy);
		texCoord[ 7] = inTexCoord0 + vec2( 0.0     ,      -dy);
		texCoord[ 8] = inTexCoord0 + vec2(       dx,      -dy);
		texCoord[ 9] = inTexCoord0 + vec2( 2.0 * dx,      -dy);
		
		texCoord[18] = inTexCoord0 + vec2(-2.0 * dx, 0.0     );
		texCoord[ 5] = inTexCoord0 + vec2(      -dx, 0.0     );
		texCoord[ 0] = inTexCoord0 + vec2( 0.0     , 0.0     ); // Center pixel
		texCoord[ 1] = inTexCoord0 + vec2(       dx, 0.0     );
		texCoord[10] = inTexCoord0 + vec2( 2.0 * dx, 0.0     );
		
		texCoord[17] = inTexCoord0 + vec2(-2.0 * dx,       dy);
		texCoord[ 4] = inTexCoord0 + vec2(      -dx,       dy);
		texCoord[ 3] = inTexCoord0 + vec2( 0.0     ,       dy);
		texCoord[ 2] = inTexCoord0 + vec2(       dx,       dy);
		texCoord[11] = inTexCoord0 + vec2( 2.0 * dx,       dy);
		
		texCoord[16] = inTexCoord0 + vec2(-2.0 * dx, 2.0 * dy);
		texCoord[15] = inTexCoord0 + vec2(      -dx, 2.0 * dy);
		texCoord[14] = inTexCoord0 + vec2( 0.0     , 2.0 * dy);
		texCoord[13] = inTexCoord0 + vec2(       dx, 2.0 * dy);
		texCoord[12] = inTexCoord0 + vec2( 2.0 * dx, 2.0 * dy);
	}