#include <cmath>
#include <cstdlib>

#include "gg.h"
using namespace gg;

/*
** OBJ ファイル
*/
#include "Obj.h"
static Obj *obj = 0;                      // OBJ ファイルデータ

/*
** 座標変換
*/
#include "matrix.h"
static GLfloat mp[16];                    // 透視投影変換行列

/*
** シェーダ
*/
static GLuint program;                    // プログラムオブジェクト
static GLint pvLoc, nvLoc;                // attribute 変数のインデックス
static GLint mwLoc, mcLoc, mgLoc, mtLoc;  // 変換行列の uniform 変数のインデックス
static GLint texLoc;                      // サンプラの uniform 変数のインデックス

/*
** テクスチャ
*/
static GLuint texture;                    // テクスチャ名

/*
**  アニメーション
*/
#define CYCLE 10000                       // 周期

/*
** 画面表示
*/
static void display(void)
{
  // 時刻の計測
  static int firstTime = 0;
  GLfloat t;
  if (firstTime == 0) { firstTime = glutGet(GLUT_ELAPSED_TIME); t = 0.0f; }
  else t = (GLfloat)((glutGet(GLUT_ELAPSED_TIME) - firstTime) % CYCLE) / (GLfloat)CYCLE;

  // モデリング変換行列
  GLfloat mm[16];
  rotate(mm, 0.0f, 1.0f, 0.0f, 12.56637f * t);

  // 視野変換行列
  GLfloat mv[16];
  lookat(mv, 0.0f, 1.0f, 2.3f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
  
  // モデルビュー変換行列
  GLfloat mw[16];
  multiply(mw, mv, mm);
  
  // 法線変換行列
  GLfloat mg[16];
  normal(mg, mw);
  
  // モデルビュー・投影変換
  GLfloat mc[16];
  multiply(mc, mp, mw);
  
  // テクスチャ変換
  GLfloat mt[16], mt1[16], mt2[16];
  lookat(mt1, -1.0, 3.0, 2.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  multiply(mt2, mt1, mm);
  perspective(mt1, 0.15, 1.0, 1.0, 8.0);
  multiply(mt, mt1, mt2);
  scale(mt1, 0.5, -0.5, 1.0);
  multiply(mt2, mt1, mt);
  translate(mt1, 0.5, 0.5, 0.0);
  multiply(mt, mt1, mt2);
 
  // 画面クリア
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // シェーダプログラムの選択
  glUseProgram(program);
  
  // uniform 変数を設定する
  glUniformMatrix4fv(mwLoc, 1, GL_FALSE, mw);
  glUniformMatrix4fv(mcLoc, 1, GL_FALSE, mc);
  glUniformMatrix4fv(mgLoc, 1, GL_FALSE, mg);
  glUniformMatrix4fv(mtLoc, 1, GL_FALSE, mt);
  glUniform1i(texLoc, 0);
  
  // 図形を描画する
  obj->draw(pvLoc, nvLoc);

  glutSwapBuffers();
}

/*
** ウィンドウのリサイズ
*/
static void resize(int w, int h)
{
  // ウィンドウ全体をビューポートにする
  glViewport(0, 0, w, h);
  
  // 透視投影変換行列を求める（アスペクト比 w / h）
  perspective(mp, 0.5f, (float)w / (float)h, 1.0f, 20.0f);
}

/*
** アニメーション
*/
static void idle(void)
{
  glutPostRedisplay();
}

/*
** キーボード
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
** 後始末
*/
static void cleanup(void)
{
  delete obj;
}

/*
** 初期化
*/
static void init(void)
{
  // ゲームグラフィックス特論の都合にもとづく初期化
  ggInit();

  // OBJ ファイルの読み込み
  obj = new Obj("model.dat", true);
  
  // シェーダプログラムの読み込み
  program = loadShader("simple.vert", "simple.frag", 0);

  // attribute 変数のインデックスの検索（見つからなければ -1）
  pvLoc = glGetAttribLocation(program, "pv");
  nvLoc = glGetAttribLocation(program, "nv");
  
  // uniform 変数のインデックスの検索（見つからなければ -1）
  mwLoc = glGetUniformLocation(program, "mw");
  mcLoc = glGetUniformLocation(program, "mc");
  mgLoc = glGetUniformLocation(program, "mg");
  mtLoc = glGetUniformLocation(program, "mt");
  texLoc = glGetUniformLocation(program, "tex");
  
  // テクスチャファイルの読み込み
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glActiveTexture(GL_TEXTURE0);
  loadImage("sysa.raw", 512, 512, GL_RGBA);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // 隠面消去
  glEnable(GL_DEPTH_TEST);

  // 背景色
  glClearColor(0.2f, 0.4f, 0.6f, 1.0f);
  
  // 後始末
  atexit(cleanup);
}

/*
** メインプログラム
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
