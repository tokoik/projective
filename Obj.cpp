/*
** �O�p�`�������� Alias OBJ �`���t�@�C���̓ǂݍ���
*/
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>
#include <float.h>

#include "Obj.h"

/*
** �I�u�W�F�N�g�̏�����
*/
void Obj::init()
{
  nv = nf = 0;
  vert = norm = fnorm = 0;
  face = 0;
}

/*
** �������̊m��
*/
void Obj::alloc(int v, int f)
{
  init();
  try {
    vert = new GLfloat[v][3];
    norm = new GLfloat[v][3];
    fnorm = new GLfloat[f][3];
    face = new GLuint[f][3];
  }
  catch (std::bad_alloc e) {
    free();
    init();
    throw e;
  }

  nv = v;
  nf = f;
}

/*
** �������̉��
*/
void Obj::free()
{
  delete[] vert;
  delete[] norm;
  delete[] fnorm;
  delete[] face;
}

/*
** �I�u�W�F�N�g�̃R�s�[
*/
void Obj::copy(const Obj &o)
{
  if (o.nv > 0 && o.nf > 0) {
    free();
    alloc(o.nv, o.nf);
    memcpy(vert, o.vert, sizeof (GLfloat) * 3 * nv);
    memcpy(norm, o.norm, sizeof (GLfloat) * 3 * nv);
    memcpy(fnorm, o.fnorm, sizeof (GLfloat) * 3 * nf);
    memcpy(face, o.face, sizeof (int) * 3 * nf);
  }
}

/*
** �t�@�C���̓ǂݍ���
*/
bool Obj::load(const char *name, bool normalize)
{
  // �t�@�C���̓ǂݍ���
  std::ifstream file(name, std::ios::binary);
  if (file.fail()) {
    std::cerr << "Can't open file: " << name << std::endl;
    return false;
  }

  // �f�[�^�̐��ƍ��W�l�̍ŏ��l�E�ő�l�𒲂ׂ�
  char buf[1024];
  int v = 0, f = 0;
  float xmin, xmax, ymin, ymax, zmin, zmax;
  xmax = ymax = zmax = -(xmin = ymin = zmin = FLT_MAX);
  while (file.getline(buf, sizeof buf)) {
    if (buf[0] == 'v' && buf[1] == ' ') {
      float x, y, z;
      sscanf(buf, "%*s %f %f %f", &x, &y, &z);
      if (x < xmin) xmin = x;
      if (x > xmax) xmax = x;
      if (y < ymin) ymin = y;
      if (y > ymax) ymax = y;
      if (z < zmin) zmin = z;
      if (z > zmax) zmax = z;
      ++v;
    }
    else if (buf[0] == 'f' && buf[1] == ' ') {
      ++f;
    }
  }

  // ���K��
  GLfloat scale, cx, cy, cz;
  if (normalize) {
    float sx = xmax - xmin;
    float sy = ymax - ymin;
    float sz = zmax - zmin;
    scale = sx;
    if (sy > scale) scale = sy;
    if (sz > scale) scale = sz;
    scale = (scale != 0.0f) ? 1.0f / scale : 1.0f;
    cx = (xmax + xmin) * 0.5f;
    cy = (ymax + ymin) * 0.5f;
    cz = (zmax + zmin) * 0.5f;
  }    
  else {
    scale = 1.0f;
    cx = cy = cz = 0.0f;
  }

  // �t�@�C���̊����߂�
  file.clear();
  file.seekg(0L, std::ios::beg);

  // �������̊m��
  alloc(v, f);

  // �f�[�^�̓ǂݍ���
  v = f = 0;
  while (file.getline(buf, sizeof buf)) {
    if (buf[0] == 'v' && buf[1] == ' ') {
      float x, y, z;
      sscanf(buf, "%*s %f %f %f", &x, &y, &z);
      vert[v][0] = (x - cx) * scale;
      vert[v][1] = (y - cy) * scale;
      vert[v][2] = (z - cz) * scale;
      ++v;
    }
    else if (buf[0] == 'f' && buf[1] == ' ') {
      if (sscanf(buf + 2, "%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", face[f], face[f] + 1, face[f] + 2) != 3) {
        if (sscanf(buf + 2, "%d//%*d %d//%*d %d//%*d", face[f], face[f] + 1, face[f] + 2) != 3) {
          sscanf(buf + 2, "%d %d %d", face[f], face[f] + 1, face[f] + 2);
        }
      }
      --face[f][0];
      --face[f][1];
      --face[f][2];
      ++f;
    }
  }

  // �ʖ@���x�N�g���̎Z�o
  for (int i = 0; i < f; ++i) {
    GLfloat dx1 = vert[face[i][1]][0] - vert[face[i][0]][0];
    GLfloat dy1 = vert[face[i][1]][1] - vert[face[i][0]][1];
    GLfloat dz1 = vert[face[i][1]][2] - vert[face[i][0]][2];
    GLfloat dx2 = vert[face[i][2]][0] - vert[face[i][0]][0];
    GLfloat dy2 = vert[face[i][2]][1] - vert[face[i][0]][1];
    GLfloat dz2 = vert[face[i][2]][2] - vert[face[i][0]][2];

    fnorm[i][0] = dy1 * dz2 - dz1 * dy2;
    fnorm[i][1] = dz1 * dx2 - dx1 * dz2;
    fnorm[i][2] = dx1 * dy2 - dy1 * dx2;
  }

  // ���_�̉��z�@���x�N�g���̎Z�o
  for (int i = 0; i < v; ++i) {
    norm[i][0] = norm[i][1] = norm[i][2] = 0.0;
  }
  
  for (int i = 0; i < f; ++i) {
    norm[face[i][0]][0] += fnorm[i][0];
    norm[face[i][0]][1] += fnorm[i][1];
    norm[face[i][0]][2] += fnorm[i][2];

    norm[face[i][1]][0] += fnorm[i][0];
    norm[face[i][1]][1] += fnorm[i][1];
    norm[face[i][1]][2] += fnorm[i][2];

    norm[face[i][2]][0] += fnorm[i][0];
    norm[face[i][2]][1] += fnorm[i][1];
    norm[face[i][2]][2] += fnorm[i][2];
  }

  // ���_�̉��z�@���x�N�g���̐��K��
  for (int i = 0; i < v; ++i) {
    GLfloat a = sqrt(norm[i][0] * norm[i][0]
                 + norm[i][1] * norm[i][1]
                 + norm[i][2] * norm[i][2]);

    if (a != 0.0) {
      norm[i][0] /= a;
      norm[i][1] /= a;
      norm[i][2] /= a;
    }
  }

  return true;
}

/*
** �}�`�̕\��
*/
void Obj::draw(GLint pvLoc, GLint nvLoc)
{
  // attribute �ϐ� pv, nv ��z��ϐ����瓾�邱�Ƃ�������
  glEnableVertexAttribArray(pvLoc);
  glEnableVertexAttribArray(nvLoc);
  
  // attribute �ϐ� pv, nv �Ɣz��ϐ� vert, norm �����т���
  glVertexAttribPointer(pvLoc, 3, GL_FLOAT, GL_FALSE, 0, vert);
  glVertexAttribPointer(nvLoc, 3, GL_FLOAT, GL_FALSE, 0, norm);
  
  // ���_�̃C���f�b�N�X�̏ꏊ���w�肵�Đ}�`��`�悷��
  glDrawElements(GL_TRIANGLES, nf * 3, GL_UNSIGNED_INT, face);
}
