#version 120

// ����
uniform vec3 ldir;	// ��������
uniform vec3 lcol;	// �������x
uniform vec3 acol;	// �������x

// �ގ�
uniform vec3 kdiff;	// �g�U���ˌW��

// ���_����
attribute vec4 pv;	// ���[�J�����W�n�̒��_�ʒu
attribute vec4 nv;  // ���_�̖@���x�N�g��

// �ϊ��s��
uniform mat4 mw;    // ���_���W�n�ւ̕ϊ��s��
uniform mat4 mc;    // �N���b�s���O���W�n�ւ̕ϊ��s��
uniform mat4 mg;    // �@���x�N�g���̕ϊ��s��
uniform mat4 mt;    // �e�N�X�`�����W�̕ϊ��s��

// �e�N�X�`���̓��e����
uniform vec3 tp;

// �t���O�����g�V�F�[�_�ɑ��锽�ˌ����x
varying vec3 idiff;
varying vec3 i;
varying vec3 tdiff;

// �t���O�����g�V�F�[�_�ɑ���e�N�X�`�����W
varying vec4 tc;

void main(void)
{
  vec3 l = normalize(ldir);						// �����x�N�g��
  vec3 v = normalize((mw * pv).xyz);			// �����x�N�g��
  vec3 n = normalize((mg * nv).xyz);			// �@���x�N�g��
  vec3 h = normalize(l - v);					// ���ԃx�N�g��
  vec3 t = normalize(tp);
  
  idiff = (max(dot(n, l), 0) * lcol + acol) * kdiff;
  tdiff = max(dot(n, t), 0) * kdiff;
  tc = mt * pv;
  
  gl_Position = mc * pv;
}

