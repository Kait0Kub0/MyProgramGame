//----------------------------------------------------------------------
//  Copyright (C) 2021 by Human
//  OpenGL プログラム
//----------------------------------------------------------------------
#ifndef _MY_POLREAD_INCLUDE
#define _MY_POLREAD_INCLUDE

#include "../glm/glm.hpp"
using namespace glm;

#include "../vec.h"

// オブジェクトデータ用構造体
typedef struct {
    int   v[3];         // 頂点インデックス
    int   n[3];         // 法線インデックス
    int   c[3];         // カラーインデックス
    int   t[3];         // テクスチャインデックス
} OBJ_POLY;


typedef struct {
    int          tex_cnt;   // テクスチャ座標数
    int          col_cnt;   // カラー数
    int          nol_cnt;   // 法線数
    int          ver_cnt;   // 頂点座標数
    int          pol_cnt;   // ポリゴン数
    vec2        *obj_t;     // テクスチャバッファ
    vec4        *obj_c;     // カラーバッファ
    vec3        *obj_n;     // 法線バッファ
    vec3        *obj_v;     // 頂点バッファ
    OBJ_POLY    *obj_pol;   // ポリゴン情報 バッファ
} OBJ_VARIABLE;


typedef struct {
    int          pol_cnt;   // ポリゴン数
    vec2        *obj_t;     // テクスチャバッファ
    vec4        *obj_c;     // カラーバッファ
    vec3        *obj_n;     // 法線バッファ
    vec3        *obj_v;     // 頂点バッファ
} OBJ_VERTEX_BUF;


typedef struct {
    int              pol_mode;  // ポリゴンモード

    int              tex_f;     // テクスチャ座標フラグ
    int              col_f;     // カラーフラグ
    int              nol_f;     // 法線フラグ
    int              ver_f;     // 頂点座標フラグ

    int              pol_cnt;   // ポリゴン数
    int              dat_cnt;   // データ数
    int              obj_size;  // バッファデータサイズ
    unsigned char   *obj_dat;   // 頂点バッファ
} OBJ_VERTEX_BUF2;


// OpenGL 頂点配列 対応フォーマット
// GL_V2F
// GL_V3F
// GL_C4UB_V2F
// GL_C4UB_V3F
// GL_C3F_V3F
// GL_N3F_V3F
// GL_C4F_N3F_V3F
// GL_T2F_V3F
// GL_T4F_V4F
// GL_T2F_C4UB_V3F
// GL_T2F_C3F_V3F
// GL_T2F_N3F_V3F
// GL_T2F_C4F_N3F_V3F
// GL_T4F_C4F_N3F_V4F

// Polygon Mode
typedef struct {
    VEC4UB  c;
    vec2    v;
} C4UB_V2F;

typedef struct {
    VEC4UB  c;
    vec3    v;
} C4UB_V3F;

typedef struct {
    vec3    c;
    vec3    v;
} C3F_V3F;

typedef struct {
    vec3    n;
    vec3    v;
} N3F_V3F;

typedef struct {
    vec4    c;
    vec3    n;
    vec3    v;
} C4F_N3F_V3F;

typedef struct {
    vec2    t;
    vec3    v;
} T2F_V3F;

typedef struct {
    vec4    t;
    vec4    v;
} T4F_V4F;

typedef struct {
    vec2    t;
    VEC4UB  c;
    vec3    v;
} T2F_C4UB_V3F;

typedef struct {
    vec2    t;
    vec3    c;
    vec3    v;
} T2F_C3F_V3F;

typedef struct {
    vec2    t;
    vec3    n;
    vec3    v;
} T2F_N3F_V3F;

typedef struct {
    vec2    t;
    vec4    c;
    vec3    n;
    vec3    v;
} T2F_C4F_N3F_V3F;

typedef struct {
    vec4    t;
    vec4    c;
    vec3    n;
    vec4    v;
} T4F_C4F_N3F_V4F;


// モデルデータクラス
class ModelData
{
public:
    // 頂点バッファNo.
    GLuint vertexNo;

    // モデルデータ
    OBJ_VERTEX_BUF2 *vertexObj;

    // テクスチャNo.
    GLuint textureNo;

    // 平行移動
    vec3	mov;

    // 回転
    vec3	rot;

};

//----------------------------------------------------------------------
// プロトタイプ宣言
extern  OBJ_VARIABLE *    LoadPolFile( const char *f_name );
extern  char *            LoadTexFile( const char *fname, int *size );
extern  int               ObjPoly( char *buf, int size,  OBJ_VARIABLE *r );
extern  int               BufChk( const char *chk, char *buf, int size );
extern  OBJ_VERTEX_BUF  * VertexBufSet ( OBJ_VARIABLE *obj_dat );
extern  OBJ_VERTEX_BUF2 * VertexBufSet2( OBJ_VARIABLE *obj_dat );

extern  void ObjIndexFree( OBJ_VARIABLE *objp );
extern  void ObjVertexFree ( OBJ_VERTEX_BUF *objp );
extern  void ObjVertexFree2( OBJ_VERTEX_BUF2 *objp );

extern  ModelData * ModelDataLoad(const char * filename);
extern  void ModelDataDisp(ModelData *model);

#endif