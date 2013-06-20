#version 330

// ���ˌ����x
in vec3 idiff;
in vec3 tdiff;

// �e�N�X�`�����W
in vec4 tc;

// �e�N�X�`��
uniform sampler2D tex;

// �J���[�o�b�t�@
layout (location = 0) out vec4 fc;

void main(void)
{
  vec4 color = textureProj(tex, tc);

  fc = vec4(idiff + tdiff * color.rgb * color.a, 1.0);
}
