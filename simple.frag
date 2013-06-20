#version 330

// 反射光強度
in vec3 idiff;
in vec3 tdiff;

// テクスチャ座標
in vec4 tc;

// テクスチャ
uniform sampler2D tex;

// カラーバッファ
layout (location = 0) out vec4 fc;

void main(void)
{
  vec4 color = textureProj(tex, tc);

  fc = vec4(idiff + tdiff * color.rgb * color.a, 1.0);
}
