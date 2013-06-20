#version 330

// 光源
uniform vec3 ldir;	// 光源方向
uniform vec3 lcol;	// 光源強度
uniform vec3 acol;	// 環境光強度

// 材質
uniform vec3 kdiff;	// 拡散反射係数

// 頂点属性
layout (location = 0) in vec4 pv;	// ローカル座標系の頂点位置
layout (location = 1) in vec4 nv;  // 頂点の法線ベクトル

// 変換行列
uniform mat4 mw;    // 視点座標系への変換行列
uniform mat4 mc;    // クリッピング座標系への変換行列
uniform mat4 mg;    // 法線ベクトルの変換行列
uniform mat4 mt;    // テクスチャ座標の変換行列

// テクスチャの投影方向
uniform vec3 tp;

// フラグメントシェーダに送る反射光強度
out vec3 idiff;
out vec3 tdiff;

// フラグメントシェーダに送るテクスチャ座標
out vec4 tc;

void main(void)
{
  vec3 l = normalize(ldir);						// 光線ベクトル
  vec3 v = normalize((mw * pv).xyz);			// 視線ベクトル
  vec3 n = normalize((mg * nv).xyz);			// 法線ベクトル
  vec3 h = normalize(l - v);					// 中間ベクトル
  vec3 t = normalize(tp);
  
  idiff = (max(dot(n, l), 0) * lcol + acol) * kdiff;
  tdiff = max(dot(n, t), 0) * kdiff;
  tc = mt * pv;
  
  gl_Position = mc * pv;
}

