#include <cmath>
#include <cstdlib>

#include "gg.h"
using namespace gg;

/*
** OBJ ファイル
*/
static GgElements *obj = 0;                           // OBJ ファイルデータ

/*
** 座標変換
*/
static GgMatrix mp;                                   // 透視投影変換行列

/*
** シェーダ
*/
static GLuint program;                                // プログラムオブジェクト
static GLint pvLoc, nvLoc;                            // attribute 変数のインデックス
static GLint mwLoc, mcLoc, mgLoc, mtLoc;              // 変換行列の uniform 変数のインデックス
static GLint ldirLoc, lcolLoc, acolLoc;               // 光源の uniform 変数のインデックス
static GLint kdiffLoc;                                // 材質の uniform 変数のインデックス
static GLint texLoc;                                  // サンプラの uniform 変数のインデックス
static GLint tpLoc;                                   // テクスチャの投影方向の uniform 変数のインデックス

/*
** 光源
*/
static const GLfloat ldir[] = { 3.0f, 4.0f, 5.0f };   // 光源方向
static const GLfloat lcol[] = { 0.3f, 0.3f, 0.3f };   // 光源強度
static const GLfloat acol[] = { 0.1f, 0.1f, 0.1f };   // 環境光強度

/*
** 材質
*/
static const GLfloat kdiff[] = { 0.8f, 0.8f, 0.8f };  // 拡散反射係数

/*
** テクスチャ
*/
static GLuint texture;                                // テクスチャ名

/*
** テクスチャの投影方向
*/
static GLfloat tp[] = { -1.0, 3.0, 2.0 };

/*
**  アニメーション
*/
#define CYCLE 10000                                   // 周期

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

  // ビュー変換行列
  GgMatrix mv;
  mv.loadLookat(0.0f, 1.0f, 2.3f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
  
  // モデルビュー変換行列
  GgMatrix mw = mv.rotate(0.0f, 1.0f, 0.0f, 12.56637f * t);
  
  // 法線変換行列
  GgMatrix mg;
  mg.loadNormal(mw);
  
  // モデルビュー・投影変換
  GgMatrix mc = mp * mw;
  
  // テクスチャ変換
  GgMatrix mt;
  mt.loadPerspective(0.15f, 1.0f, 1.0f, 8.0f).lookat(tp[0], tp[2], tp[2], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
 
  // 画面クリア
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // シェーダプログラムの選択
  glUseProgram(program);
  
  // uniform 変数を設定する
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
  
  // 図形を描画する
  obj->draw();

  // 固定機能シェーダに戻す
  glUseProgram(0);
  
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
  mp.loadPerspective(0.5f, (float)w / (float)h, 1.0f, 20.0f);
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
  obj = ggElementsObj("model.dat");
  
  // シェーダプログラムの読み込み
  program = ggLoadShader("simple.vert", "simple.frag");

  // attribute 変数のインデックスの検索（見つからなければ -1）
  pvLoc = glGetAttribLocation(program, "pv");
  nvLoc = glGetAttribLocation(program, "nv");
  
  // uniform 変数のインデックスの検索（見つからなければ -1）
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
  
  // テクスチャファイルの読み込み
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glActiveTexture(GL_TEXTURE0);
  ggLoadImage("sysa.tga", GL_RGBA);

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
