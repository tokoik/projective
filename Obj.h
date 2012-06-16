/*
** �O�p�`�������� Alias OBJ �`���t�@�C���̓ǂݍ���
*/
#ifndef OBJ_H
#define OBJ_H

#include "gg.h"

class Obj {
  int nv, nf;                       // ���_�̐��C�ʂ̐�
  GLfloat (*vert)[3];               // ���_�̈ʒu
  GLfloat (*norm)[3];               // ���_�̖@���x�N�g��
  GLfloat (*fnorm)[3];              // �ʂ̖@���x�N�g��
  GLuint (*face)[3];                // �ʃf�[�^�i���_�̃C���f�b�N�X�j
  void init();                      // ������
  void alloc(int nv, int nf);       // �������̊m��
  void free();                      // �������̉��
  void copy(const Obj &);           // �������̃R�s�[

public:
  Obj() { init(); }
  Obj(const char *name, bool normalize = false) { load(name, normalize); }
  Obj(const Obj &o) { copy(o); }
  virtual ~Obj() { free(); }
  Obj &operator=(const Obj &o) { if (this != &o) copy(o); return *this; }
  
  // OBJ �t�@�C���̓ǂݍ���
  bool load(const char *name, bool normalize = false);

  // �}�`�̕`��
  void draw(GLint pvLoc, GLint nvLoc);
};

#endif
