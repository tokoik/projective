#version 120

// 光源
const vec3 ldiff  = vec3(0.5, 0.5, 0.5);  // 光源強度の拡散反射成分

// 材質
const vec3 kdiff  = vec3(0.8, 0.8, 0.8);  // 拡散反射係数

// 頂点属性
attribute vec4 pv;	// ローカル座標系の頂点位置
attribute vec4 nv;  // 頂点の法線ベクトル

// 変換行列
uniform mat4 mw;    // 視点座標系への変換行列
uniform mat4 mc;    // クリッピング座標系への変換行列
uniform mat4 mg;    // 法線ベクトルの変換行列
uniform mat4 mt;    // テクスチャ座標の変換行列

// テクスチャの投影方向
uniform vec3 tp;

// フラグメントシェーダに送る反射光強度
varying vec3 idiff;
varying vec3 tdiff;

// フラグメントシェーダに送るテクスチャ座標
varying vec4 tc;

void main(void)
{
  vec3 l = normalize(vec3(4.0, 8.0, 8.0));     // 光線ベクトル
  vec3 v = normalize((mw * pv).xyz);           // 視線ベクトル
  vec3 n = normalize((mg * nv).xyz);           // 法線ベクトル
  vec3 h = normalize(l - v);                   // 中間ベクトル
  vec3 t = normalize(tp);
  
  idiff = max(dot(n, l), 0) * kdiff * ldiff;
  tdiff = max(dot(n, t), 0) * kdiff;
  tc = mt * pv;
  
  gl_Position = mc * pv;
}

