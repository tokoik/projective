#include <cmath>
#include <cstdlib>

#include "gg.h"
using namespace gg;

/*
** OBJ �t�@�C��
*/
#include "Obj.h"
static Obj *obj = 0;                      // OBJ �t�@�C���f�[�^

/*
** ���W�ϊ�
*/
#include "matrix.h"
static GLfloat mp[16];                    // �������e�ϊ��s��

/*
** �V�F�[�_
*/
static GLuint program;                    // �v���O�����I�u�W�F�N�g
static GLint pvLoc, nvLoc;                // attribute �ϐ��̃C���f�b�N�X
static GLint mwLoc, mcLoc, mgLoc, mtLoc;  // �ϊ��s��� uniform �ϐ��̃C���f�b�N�X
static GLint texLoc;                      // �T���v���� uniform �ϐ��̃C���f�b�N�X

/*
** �e�N�X�`��
*/
static GLuint texture;                    // �e�N�X�`����

/*
**  �A�j���[�V����
*/
#define CYCLE 10000                       // ����

/*
** ��ʕ\��
*/
static void display(void)
{
  // �����̌v��
  static int firstTime = 0;
  GLfloat t;
  if (firstTime == 0) { firstTime = glutGet(GLUT_ELAPSED_TIME); t = 0.0f; }
  else t = (GLfloat)((glutGet(GLUT_ELAPSED_TIME) - firstTime) % CYCLE) / (GLfloat)CYCLE;

  // ���f�����O�ϊ��s��
  GLfloat mm[16];
  rotate(mm, 0.0f, 1.0f, 0.0f, 12.56637f * t);

  // ����ϊ��s��
  GLfloat mv[16];
  lookat(mv, 0.0f, 1.0f, 2.3f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
  
  // ���f���r���[�ϊ��s��
  GLfloat mw[16];
  multiply(mw, mv, mm);
  
  // �@���ϊ��s��
  GLfloat mg[16];
  normal(mg, mw);
  
  // ���f���r���[�E���e�ϊ�
  GLfloat mc[16];
  multiply(mc, mp, mw);
  
  // �e�N�X�`���ϊ�
  GLfloat mt[16], mt1[16], mt2[16];
  lookat(mt1, -1.0, 3.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  multiply(mt2, mt1, mm);
  perspective(mt1, 0.15, 1.0, 1.0, 8.0);
  multiply(mt, mt1, mt2);
  scale(mt1, 0.5, -0.5, 1.0);
  multiply(mt2, mt1, mt);
  translate(mt1, 0.5, 0.5, 0.0);
  multiply(mt, mt1, mt2);
 
  // ��ʃN���A
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // �V�F�[�_�v���O�����̑I��
  glUseProgram(program);
  
  // uniform �ϐ���ݒ肷��
  glUniformMatrix4fv(mwLoc, 1, GL_FALSE, mw);
  glUniformMatrix4fv(mcLoc, 1, GL_FALSE, mc);
  glUniformMatrix4fv(mgLoc, 1, GL_FALSE, mg);
  glUniformMatrix4fv(mtLoc, 1, GL_FALSE, mt);
  glUniform1i(texLoc, 0);
  
  // �}�`��`�悷��
  obj->draw(pvLoc, nvLoc);

  glutSwapBuffers();
}

/*
** �E�B���h�E�̃��T�C�Y
*/
static void resize(int w, int h)
{
  // �E�B���h�E�S�̂��r���[�|�[�g�ɂ���
  glViewport(0, 0, w, h);
  
  // �������e�ϊ��s������߂�i�A�X�y�N�g�� w / h�j
  perspective(mp, 0.5f, (float)w / (float)h, 1.0f, 20.0f);
}

/*
** �A�j���[�V����
*/
static void idle(void)
{
  glutPostRedisplay();
}

/*
** �L�[�{�[�h
*/
static void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
    case ' ':
      glutPostRedisplay();
      break;
    case 's':
    case 'S':
      glutIdleFunc(0);
      break;
    case 'g':
    case 'G':
      glutIdleFunc(idle);
      break;
    case '\033':
    case 'Q':
    case 'q':
      exit(0);
    default:
      break;
  }
}

/*
** ��n��
*/
static void cleanup(void)
{
  delete obj;
}

/*
** ������
*/
static void init(void)
{
  // �Q�[���O���t�B�b�N�X���_�̓s���ɂ��ƂÂ�������
  ggInit();

  // OBJ �t�@�C���̓ǂݍ���
  obj = new Obj("model.dat", true);
  
  // �V�F�[�_�v���O�����̓ǂݍ���
  program = loadShader("simple.vert", "simple.frag", 0);

  // attribute �ϐ��̃C���f�b�N�X�̌����i������Ȃ���� -1�j
  pvLoc = glGetAttribLocation(program, "pv");
  nvLoc = glGetAttribLocation(program, "nv");
  
  // uniform �ϐ��̃C���f�b�N�X�̌����i������Ȃ���� -1�j
  mwLoc = glGetUniformLocation(program, "mw");
  mcLoc = glGetUniformLocation(program, "mc");
  mgLoc = glGetUniformLocation(program, "mg");
  mtLoc = glGetUniformLocation(program, "mt");
  texLoc = glGetUniformLocation(program, "tex");
  
  // �e�N�X�`���t�@�C���̓ǂݍ���
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glActiveTexture(GL_TEXTURE0);
  loadImage("sysa.raw", 512, 512, GL_RGBA);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // �B�ʏ���
  glEnable(GL_DEPTH_TEST);

  // �w�i�F
  glClearColor(0.2f, 0.4f, 0.6f, 1.0f);
  
  // ��n��
  atexit(cleanup);
}

/*
** ���C���v���O����
*/
int main(int argc, char *argv[])
{
  glutInitWindowSize(512, 512);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutKeyboardFunc(keyboard);
  glutIdleFunc(idle);
  init();
  glutMainLoop();
  
  return 0;
}
