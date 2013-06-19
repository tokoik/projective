#include <cmath>
#include <cstdlib>

#include "gg.h"
using namespace gg;

/*
** OBJ �t�@�C��
*/
static GgElements *obj = 0;                           // OBJ �t�@�C���f�[�^

/*
** ���W�ϊ�
*/
static GgMatrix mp;                                   // �������e�ϊ��s��

/*
** �V�F�[�_
*/
static GLuint program;                                // �v���O�����I�u�W�F�N�g
static GLint pvLoc, nvLoc;                            // attribute �ϐ��̃C���f�b�N�X
static GLint mwLoc, mcLoc, mgLoc, mtLoc;              // �ϊ��s��� uniform �ϐ��̃C���f�b�N�X
static GLint ldirLoc, lcolLoc, acolLoc;               // ������ uniform �ϐ��̃C���f�b�N�X
static GLint kdiffLoc;                                // �ގ��� uniform �ϐ��̃C���f�b�N�X
static GLint texLoc;                                  // �T���v���� uniform �ϐ��̃C���f�b�N�X
static GLint tpLoc;                                   // �e�N�X�`���̓��e������ uniform �ϐ��̃C���f�b�N�X

/*
** ����
*/
static const GLfloat ldir[] = { 3.0f, 4.0f, 5.0f };   // ��������
static const GLfloat lcol[] = { 0.3f, 0.3f, 0.3f };   // �������x
static const GLfloat acol[] = { 0.1f, 0.1f, 0.1f };   // �������x

/*
** �ގ�
*/
static const GLfloat kdiff[] = { 0.8f, 0.8f, 0.8f };  // �g�U���ˌW��

/*
** �e�N�X�`��
*/
static GLuint texture;                                // �e�N�X�`����

/*
** �e�N�X�`���̓��e����
*/
static GLfloat tp[] = { -1.0, 3.0, 2.0 };

/*
**  �A�j���[�V����
*/
#define CYCLE 10000                                   // ����

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

  // �r���[�ϊ��s��
  GgMatrix mv;
  mv.loadLookat(0.0f, 1.0f, 2.3f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
  
  // ���f���r���[�ϊ��s��
  GgMatrix mw = mv.rotate(0.0f, 1.0f, 0.0f, 12.56637f * t);
  
  // �@���ϊ��s��
  GgMatrix mg;
  mg.loadNormal(mw);
  
  // ���f���r���[�E���e�ϊ�
  GgMatrix mc = mp * mw;
  
  // �e�N�X�`���ϊ�
  GgMatrix mt;
  mt.loadPerspective(0.15f, 1.0f, 1.0f, 8.0f).lookat(tp[0], tp[2], tp[2], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
 
  // ��ʃN���A
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // �V�F�[�_�v���O�����̑I��
  glUseProgram(program);
  
  // uniform �ϐ���ݒ肷��
  glUniformMatrix4fv(mwLoc, 1, GL_FALSE, mw.get());
  glUniformMatrix4fv(mcLoc, 1, GL_FALSE, mc.get());
  glUniformMatrix4fv(mgLoc, 1, GL_FALSE, mg.get());
  glUniformMatrix4fv(mtLoc, 1, GL_FALSE, mt.get());
  glUniform3fv(ldirLoc, 1, ldir);
  glUniform3fv(lcolLoc, 1, lcol);
  glUniform3fv(acolLoc, 1, acol);
  glUniform3fv(kdiffLoc, 1, kdiff);
  glUniform1i(texLoc, 0);
  glUniform3fv(tpLoc, 1, tp);
  
  // �}�`��`�悷��
  obj->draw();

  // �Œ�@�\�V�F�[�_�ɖ߂�
  glUseProgram(0);
  
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
  mp.loadPerspective(0.5f, (float)w / (float)h, 1.0f, 20.0f);
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
  obj = ggElementsObj("model.dat");
  
  // �V�F�[�_�v���O�����̓ǂݍ���
  program = ggLoadShader("simple.vert", "simple.frag");

  // attribute �ϐ��̃C���f�b�N�X�̌����i������Ȃ���� -1�j
  pvLoc = glGetAttribLocation(program, "pv");
  nvLoc = glGetAttribLocation(program, "nv");
  
  // uniform �ϐ��̃C���f�b�N�X�̌����i������Ȃ���� -1�j
  mwLoc = glGetUniformLocation(program, "mw");
  mcLoc = glGetUniformLocation(program, "mc");
  mgLoc = glGetUniformLocation(program, "mg");
  mtLoc = glGetUniformLocation(program, "mt");
  lcolLoc = glGetUniformLocation(program, "lcol");
  ldirLoc = glGetUniformLocation(program, "ldir");
  acolLoc = glGetUniformLocation(program, "acol");
  kdiffLoc = glGetUniformLocation(program, "kdiff");
  texLoc = glGetUniformLocation(program, "tex");
  tpLoc = glGetUniformLocation(program, "tp");
  
  // �e�N�X�`���t�@�C���̓ǂݍ���
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glActiveTexture(GL_TEXTURE0);
  ggLoadImage("sysa.tga", GL_RGBA);

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
