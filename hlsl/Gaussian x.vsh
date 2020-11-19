
varying float c1;
varying float c2;
varying float c3;
varying float c4;
varying float norm;

void main()
{
  float wid = 2.0;
//  float wid = screen_texture_sz.y/color_texture_sz.y*0.4;
  c1 = exp(-1.0/wid/wid);
  c2 = exp(-4.0/wid/wid);
  c3 = exp(-9.0/wid/wid);
  c4 = exp(-16.0/wid/wid);
  norm = 1.0 / (1.0 + 2.0*(c1+c2+c3+c4));

  gl_TexCoord[0]  = gl_TextureMatrix[0] * gl_MultiTexCoord0;
  gl_Position     = ftransform();
}

