#version 120

// ���ˌ����x
varying vec3 idiff;
varying vec3 tdiff;

// �e�N�X�`�����W
varying vec4 tc;

// �e�N�X�`��
uniform sampler2D tex;

void main(void)
{
  vec4 color = texture2DProj(tex, tc);

  gl_FragColor = vec4(idiff + tdiff * color.rgb * color.a, 1.0);
}
