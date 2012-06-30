#version 120

// 反射光強度
varying vec3 idiff;
varying vec3 tdiff;

// テクスチャ座標
varying vec4 tc;

// テクスチャ
uniform sampler2D tex;

void main(void)
{
  vec4 color = texture2DProj(tex, tc);

  gl_FragColor = vec4(idiff + tdiff * color.rgb * color.a, 1.0);
}
