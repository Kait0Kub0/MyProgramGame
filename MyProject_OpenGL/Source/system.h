//----------------------------------------------------------------------
//  Copyright (C) 2023 by Human
//  OpenGL プログラム
//----------------------------------------------------------------------
#pragma once


#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

// C++
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <process.h>

// システム系
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <mbstring.h>
#include <math.h>
//#include < atlstr.h >


// グラフィック系
#include "Graphics/gl3w/gl3w.h"
#include "Graphics/gl3w/gl.h"
#include "Graphics/gl3w/glcorearb.h"
#include <GL/glu.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include "Graphics/GLFW/glfw3.h"
#include "Graphics/GLFW/glfw3native.h"
#include "Graphics/glpng/glpng.h"

#include "Graphics/glm/glm.hpp"
#include "Graphics/glm/gtc/type_ptr.hpp"
#include "Graphics/glm/gtc/quaternion.hpp"
#include "Graphics/glm/gtx/quaternion.hpp"
#include "Graphics/glm/common.hpp"


using namespace std;
using namespace glm;


// 文字出力
#include "Graphics/TextDisp/ttfText.h"


// 音再生
//#include "Music/MusicSys.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "winmm.lib")

// コンソールOFF（黒い文字表示画面）
//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

#define SCREEN_WIDTH   1200
#define SCREEN_HEIGHT   900
#define SCREEN_HEIGHT2  450

#define PI 3.141592653589793

#define SYSEXIT	FALSE

#include "Graphics/vec.h"
#include "Graphics/texture.h"
#include "Graphics/opengl.h"
#include "Graphics/bmp.h"
#include "Graphics/tga.h"
#include "Graphics/png.h"
#include "Graphics/polread/polread.h"
#include "Graphics/GLSprite.h"




// シェーダー管理プログラム
#include "Graphics/Shader/Shader.h"

// 画像管理プログラム
#include "Graphics/ImageSys.h"

// モデルデータ管理プログラム
#include "Graphics/ModelSys.h"

//----------------------------------------------------------------------
// グローバル変数
extern HWND hwind;


//----------------------------------------------------------------------
// グローバル変数
extern int   windowsLoop;


//----------------------------------------------------------------------
// グローバル変数
extern  GLuint      GLTexNo[10];                // テクスチャ管理番号
extern  TEX_DAT     *TexImage[10];              // テクスチャデータアドレス

extern  char        PlayerFilename0[];          // 画像ファイル名
extern  char        EnemyFilename0[];           // 画像ファイル名

// ウィンドウサイズ（クライアント領域）
extern  int     clientW, clientH;
