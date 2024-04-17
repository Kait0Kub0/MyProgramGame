//----------------------------------------------------------------------
//  Copyright (C) 2014 by Human
//  OpenGL プログラム
//----------------------------------------------------------------------
#pragma once

// グローバル変数
extern GLFWwindow* GlfwWindow;

extern GLuint      GLTexNo[10];					// テクスチャ管理番号

extern TEX_DAT    *TexImage[10];				// テクスチャデータアドレス

extern char  PlayerFilename0[];  // 画像ファイル名
extern char  EnemyFilename0[]; // 画像ファイル名

extern GLfloat LightAmbient[];       // 環境光・周囲光
extern GLfloat LightDiffuse[];       // 拡散光
extern vec4 LightPosition;      // 平行光源ベクトル・点光源座標


//  色属性（材質）
extern GLfloat surface_ambient[];	// 
extern GLfloat surface_diffuse[];	// 


// システム管理フラグ
extern int      windowsLoop;


//----------------------------------------------------------------------
// エラー表示
//----------------------------------------------------------------------
static void error_callback(int error, const char* description);


//----------------------------------------------------------------------
// キー入力処理
//----------------------------------------------------------------------
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int getKeyIn(int key);

//----------------------------------------------------------------------
// 画面サイズ変更処理
//----------------------------------------------------------------------
static void reshape_callback(GLFWwindow* window, int width, int height);



//----------------------------------------------------------------------
// 画像読み込み処理
//----------------------------------------------------------------------
int TextureLoad(const char* fileName, bool linear = false, bool mipmap = false);


int sysInit();


void sysDraw();


void sysDrawLoop();


void sysEnd();

