//----------------------------------------------------------------------
//   TrueTypeFont 文字出力 プログラム                                   
//   Ver 2.0  : テクスチャを使用して描画                                
//----------------------------------------------------------------------
#pragma once


#include <windows.h>
#include <assert.h>
#include <stdio.h>

#include "../gl3w/gl3w.h"
#include "../gl3w/gl.h"
#include "../gl3w/glcorearb.h"


#if GLUT
    #include <GL/glut.h>                    // GLUT 宣言
#else
    #include <GL/gl.h>                      // OpenGL ヘッダファイル
    #include <GL/glu.h>
    #pragma comment (lib, "opengl32.lib")   // link with Microsoft OpenGL lib
    #pragma comment (lib, "glu32.lib")      // link with Microsoft OpenGL Utility lib
#endif


#define POW2nMAX                2048            // 2のｎ乗チェック最大値

// デフォルトの文字数
#define TTF_TEXT_DEF_WIDTH      256             // （全角）32 文字 ： （半角）64 文字
#define TTF_TEXT_DEF_HEIGHT     32              //  2 行

#define TTF_TEXT_DEF_SCALE      1.0f            // 拡大縮小値

#define TTF_TEXT_DEF_FRONT_C_R  1.0f            // デフォルト文字カラー
#define TTF_TEXT_DEF_FRONT_C_G  1.0f            // デフォルト文字カラー
#define TTF_TEXT_DEF_FRONT_C_B  1.0f            // デフォルト文字カラー
#define TTF_TEXT_DEF_FRONT_C_A  1.0f            // デフォルト文字カラー

#define TTF_TEXT_DEF_BACK_C_R   0.5f            // デフォルト背景カラー
#define TTF_TEXT_DEF_BACK_C_G   0.5f            // デフォルト背景カラー
#define TTF_TEXT_DEF_BACK_C_B   0.5f            // デフォルト背景カラー
#define TTF_TEXT_DEF_BACK_C_A   0.8f            // デフォルト背景カラー


// デフォルト論理フォント
#define TTF_TEXT_DEF_SIZE  16                   // デフォルト文字サイズ
#define TTF_TEXT_DEF_NAME  L"ＭＳ ゴシック"     // デフォルト文字フォント
//#define TTF_TEXT_DEF_NAME "HG丸ｺﾞｼｯｸM-PRO"
//#define TTF_TEXT_DEF_NAME "ＭＳ 明朝"
//#define TTF_TEXT_DEF_NAME "HG正楷書体-PRO"


//システム設定
typedef struct {
    int         sys_Init_flag;                  // TTF2DTEXT システム初期化フラグ
    HWND        *hWnd;                          // ウィンドウハンドル
} TTF_SYSTEM;


extern TTF_SYSTEM  TtfSys;

