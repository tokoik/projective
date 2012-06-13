#if defined(WIN32)
//#  pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#  include "glut.h"
#elif defined(__APPLE__) || defined(MACOSX)
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "box.h"

/*
** ���̕`��
*/
void box(double x, double y, double z)
{
  /* ���_�̍��W�l */
  const GLdouble vertex[][4][3] = {
		{{ -x, -y, -z }, {  x, -y, -z }, {  x, -y,  z }, { -x, -y,  z }},
		{{ -x, -y, -z }, { -x,  y, -z }, {  x,  y, -z }, {  x, -y, -z }},
		{{  x, -y, -z }, {  x,  y, -z }, {  x,  y,  z }, {  x, -y,  z }},
		{{  x, -y,  z }, {  x,  y,  z }, { -x,  y,  z }, { -x, -y,  z }},
		{{ -x, -y,  z }, { -x,  y,  z }, { -x,  y, -z }, { -x, -y, -z }},
		{{ -x,  y,  z }, {  x,  y,  z }, {  x,  y, -z }, { -x,  y, -z }},
  };
  
  /* ���_�̃e�N�X�`�����W */
  static const GLdouble texcoord[][4][2] = {
		{{ 0.0, 0.0 }, { 0.0, 1.0 }, { 1.0, 1.0 }, { 1.0, 0.0 }},
		{{ 0.0, 0.0 }, { 0.0, 1.0 }, { 1.0, 1.0 }, { 1.0, 0.0 }},
		{{ 0.0, 0.0 }, { 0.0, 1.0 }, { 1.0, 1.0 }, { 1.0, 0.0 }},
		{{ 0.0, 0.0 }, { 0.0, 1.0 }, { 1.0, 1.0 }, { 1.0, 0.0 }},
		{{ 0.0, 0.0 }, { 0.0, 1.0 }, { 1.0, 1.0 }, { 1.0, 0.0 }},
		{{ 0.0, 0.0 }, { 0.0, 1.0 }, { 1.0, 1.0 }, { 1.0, 0.0 }},
  };
  
  /* �ʂ̖@���x�N�g�� */
  static const GLdouble normal[][3] = {
    {  0.0, -1.0,  0.0 },
    {  0.0,  0.0, -1.0 },
    {  1.0,  0.0,  0.0 },
    {  0.0,  0.0,  1.0 },
    { -1.0,  0.0,  0.0 },
    {  0.0,  1.0,  0.0 },
  };
  
  int i, j;
  
  /* �l�p�`�U���Ŕ���`�� */
  glBegin(GL_QUADS);
  for (j = 0; j < 6; ++j) {
    glNormal3dv(normal[j]);
    for (i = 0; i < 4; ++i) {
      /* �e�N�X�`�����W�̎w�� */
      glTexCoord2dv(texcoord[j][i]);
      /* �Ή����钸�_���W�̎w�� */
      glVertex3dv(vertex[j][i]);
    }
  }
  glEnd();
}
