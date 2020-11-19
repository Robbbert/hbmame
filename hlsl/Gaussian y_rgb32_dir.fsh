
#pragma optimize (on)
#pragma debug (off)

uniform sampler2D mpass_texture;
uniform vec2      color_texture_sz;
uniform vec2      color_texture_pow2_sz;
uniform vec2      screen_texture_sz;
uniform vec2      screen_texture_pow2_sz;

varying float c1;
varying float c2;
varying float c3;
varying float c4;
varying float norm;

const vec4 gamma = vec4(2.2);
const vec4 gammai = vec4(1.0/2.2);
#define TEX2D(v) pow(texture2D(mpass_texture, (v)), gamma)

void main()
{
  vec4 sum   = vec4(0.0);

  vec2 xy = gl_TexCoord[0].st;
  float oney = 1.0/color_texture_pow2_sz.y;

  sum += TEX2D(xy + vec2( 0.0, -4.0 * oney)) * vec4(c4);
  sum += TEX2D(xy + vec2( 0.0, -3.0 * oney)) * vec4(c3);
  sum += TEX2D(xy + vec2( 0.0, -2.0 * oney)) * vec4(c2);
  sum += TEX2D(xy + vec2( 0.0, -1.0 * oney)) * vec4(c1);
  sum += TEX2D(xy);
  sum += TEX2D(xy + vec2( 0.0, +1.0 * oney)) * vec4(c1);
  sum += TEX2D(xy + vec2( 0.0, +2.0 * oney)) * vec4(c2);
  sum += TEX2D(xy + vec2( 0.0, +3.0 * oney)) * vec4(c3);
  sum += TEX2D(xy + vec2( 0.0, +4.0 * oney)) * vec4(c4);

  gl_FragColor = pow(sum*vec4(norm),gammai);
}

