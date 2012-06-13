#include <cstdio>
#include <cstdlib>
#include <cmath>
#if defined(WIN32)
#  pragma warning(disable:4996)
//#  pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#  pragma comment(lib, "glew32.lib")
#  include "glew.h"
#  include "wglew.h"
#  include "glut.h"
#elif defined(__APPLE__) || defined(MACOSX)
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

/*
** ����
*/
static const GLfloat lightpos[] = { 0.0f, 0.0f, 1.0f, 0.0f }; /* �ʒu�@�@�@ */
static const GLfloat lightcol[] = { 1.0f, 1.0f, 1.0f, 1.0f }; /* ���ڌ����x */
static const GLfloat lightamb[] = { 0.1f, 0.1f, 0.1f, 1.0f }; /* �������x */

/*
** �e�N�X�`��
*/
#define TEXWIDTH  256                               /* �e�N�X�`���̕��@�@�@ */
#define TEXHEIGHT 256                               /* �e�N�X�`���̍����@�@ */
static const char texture1[] = "tire.raw";          /* �e�N�X�`���t�@�C���� */
static double genfunc[][4] = {            /* �e�N�X�`�������֐��̃p�����[�^ */
  { 1.0, 0.0, 0.0, 0.0 },
  { 0.0, 1.0, 0.0, 0.0 },
};

/*
** ������
*/
static void init(void)
{
  /* �e�N�X�`���̓ǂݍ��݂Ɏg���z�� */
  GLubyte texture[TEXHEIGHT][TEXWIDTH][4];
  FILE *fp;
  
  /* �e�N�X�`���摜�̓ǂݍ��� */
  if ((fp = fopen(texture1, "rb")) != NULL) {
    fread(texture, sizeof texture, 1, fp);
    fclose(fp);
  }
  else {
    perror(texture1);
  }
  
  /* �e�N�X�`���摜�̓o�C�g�P�ʂɋl�ߍ��܂�Ă��� */
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  
  /* �e�N�X�`���̊��蓖�� */
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXWIDTH, TEXHEIGHT, 0,
               GL_RGBA, GL_UNSIGNED_BYTE, texture);
  
  /* �e�N�X�`�����g��E�k��������@�̎w�� */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  
  /* �e�N�X�`���̌J��Ԃ����@�̎w�� */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  
  /* �e�N�X�`���� */
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
  
  /* ���_�̃I�u�W�F�N�g��Ԃɂ�������W�l���e�N�X�`�����W�Ɏg�� */
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  
  /* �e�N�X�`�����W�����֐��̐ݒ� */
  glTexGendv(GL_S, GL_OBJECT_PLANE, genfunc[0]);
  glTexGendv(GL_T, GL_OBJECT_PLANE, genfunc[1]);

  /* �����ݒ� */
  glClearColor(0.3f, 0.3f, 1.0f, 0.0f);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  
  /* �����̏����ݒ� */
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcol);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightcol);
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightamb);
}

/* ����`���֐��̐錾 */
#include "box.h"

/*
** �V�[���̕`��
*/
static void scene(void)
{
  static const GLfloat color[] = { 0.8, 0.8, 0.2, 1.0 };  /* �ގ� (�F) */
  
  /* �ގ��̐ݒ� */
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
  
  /* �e�N�X�`���}�b�s���O�J�n */
  glEnable(GL_TEXTURE_2D);
  
/* �e�N�X�`�����W�̎���������L���ɂ��� */
  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);
  
  /* ����`�� */
 glutSolidTeapot(1.0);
    
  /* �e�N�X�`�����W�̎��������𖳌��ɂ��� */
  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);
  
  /* �e�N�X�`���}�b�s���O�I�� */
  glDisable(GL_TEXTURE_2D);
}


/****************************
** GLUT �̃R�[���o�b�N�֐� **
****************************/

/* �g���b�N�{�[�������p�֐��̐錾 */
#include "trackball.h"

/* �A�j���[�V�����̃T�C�N�� */
#define FRAMES 360

static void display(void)
{
  /* �t���[�������J�E���g���Ď��ԂƂ��Ďg�� */
  static int frame = 0;                      /* �t���[�����@�@�@�@�@�@�@ */
  double t = (double)frame / (double)FRAMES; /* ���ԂƂƂ��� 0��1 �ɕω� */
  
  if (++frame >= FRAMES) frame = 0;
  
  /* �e�N�X�`���s��̐ݒ� */
  glMatrixMode(GL_TEXTURE);
  glLoadIdentity();
  glTranslated(0.5, 0.5, 0.0);
  glRotated(t * 360.0, 0.0, 0.0, 1.0);
  glTranslated(-0.5, -0.5, 0.0);
  
  /* ���f���r���[�ϊ��s��̐ݒ� */
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  /* �����̈ʒu��ݒ� */
  glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
  
  /* ���_�̈ړ��i���̂̕������Ɉړ��j*/
  glTranslated(0.0, 0.0, -3.0);
  
  /* �g���b�N�{�[�������ɂ���] */
  glMultMatrixd(trackballRotation());
  
  /* ��ʃN���A */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  /* �V�[���̕`�� */
  scene();
  
  /* �_�u���o�b�t�@�����O */
  glutSwapBuffers();
}

static void resize(int w, int h)
{
  /* �g���b�N�{�[������͈� */
  trackballRegion(w, h);
  
  /* �E�B���h�E�S�̂��r���[�|�[�g�ɂ��� */
  glViewport(0, 0, w, h);
  
  /* �����ϊ��s��̎w�� */
  glMatrixMode(GL_PROJECTION);
  
  /* �����ϊ��s��̏����� */
  glLoadIdentity();
  gluPerspective(60.0, (double)w / (double)h, 1.0, 100.0);
}

static void idle(void)
{
  /* ��ʂ̕`���ւ� */
  glutPostRedisplay();
}

static void mouse(int button, int state, int x, int y)
{
  switch (button) {
  case GLUT_LEFT_BUTTON:
    switch (state) {
    case GLUT_DOWN:
      /* �g���b�N�{�[���J�n */
      trackballStart(x, y);
      break;
    case GLUT_UP:
      /* �g���b�N�{�[����~ */
      trackballStop(x, y);
      break;
    default:
      break;
    }
    break;
    default:
      break;
  }
}

static void motion(int x, int y)
{
  /* �g���b�N�{�[���ړ� */
  trackballMotion(x, y);
}

static void keyboard(unsigned char key, int x, int y)
{
  switch (key) {
  case 'q':
  case 'Q':
  case '\033':
    /* ESC �� q �� Q ���^�C�v������I�� */
    exit(0);
  default:
    break;
  }
}

/*
** ���C���v���O����
*/
int main(int argc, char *argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutIdleFunc(idle);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc(keyboard);
  init();
  glutMainLoop();
  return 0;
}
