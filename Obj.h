/*
** 三角形分割した Alias OBJ 形式ファイルの読み込み
*/
#ifndef OBJ_H
#define OBJ_H

#include "gg.h"

class Obj {
  int nv, nf;                       // 頂点の数，面の数
  GLfloat (*vert)[3];               // 頂点の位置
  GLfloat (*norm)[3];               // 頂点の法線ベクトル
  GLfloat (*fnorm)[3];              // 面の法線ベクトル
  GLuint (*face)[3];                // 面データ（頂点のインデックス）
  void init();                      // 初期化
  void alloc(int nv, int nf);       // メモリの確保
  void free();                      // メモリの解放
  void copy(const Obj &);           // メモリのコピー

public:
  Obj() { init(); }
  Obj(const char *name, bool normalize = false) { load(name, normalize); }
  Obj(const Obj &o) { copy(o); }
  virtual ~Obj() { free(); }
  Obj &operator=(const Obj &o) { if (this != &o) copy(o); return *this; }
  
  // OBJ ファイルの読み込み
  bool load(const char *name, bool normalize = false);

  // 図形の描画
  void draw(GLint pvLoc, GLint nvLoc);
};

#endif
