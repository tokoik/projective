#version 120

// ���ˌ����x
varying vec3 idiff;      // �g�U���ˌ����x

// �e�N�X�`�����W
varying vec4 tc;

// �e�N�X�`��
uniform sampler2D tex;

void main(void)
{
  vec4 color = texture2DProj(tex, tc);

  gl_FragColor = vec4(mix(idiff, idiff * color.rgb, color.a), 1.0);
}
