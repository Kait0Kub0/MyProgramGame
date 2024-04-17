//----------------------------------------------------------------------
//  Copyright (C) 2014 by Human
//  OpenGL プログラム
//----------------------------------------------------------------------
#ifndef _MY_TEXTURE_INCLUDE
#define _MY_TEXTURE_INCLUDE

#include "../system.h"

// 対応フォーマット数
#define CHK_FILE_MAX 3

// 画像データファイル拡張子
#define BMP_FILE_END    "BMP"
#define TGA_FILE_END    "TGA"
#define PNG_FILE_END    "PNG"

enum
{
	BITMAP_FILE_DAT,
	TARGA_FILE_DAT,
	PNG_FILE_DAT,
};


typedef struct
{
	int             sizeX;  // 横幅
	int             sizeY;  // 縦幅
	unsigned char   *data;  // 画像データが格納されているアドレス
} TEX_DAT;


extern char ChkFileFormat[CHK_FILE_MAX][4];


extern TEX_DAT * TextureGL(GLuint texno, const char *f_name, GLuint wrap, GLuint filter);
extern void TextureDelGL(GLuint texno);
extern void TexDatFree(TEX_DAT **image);
extern char * GetExtension(const char *pn);
extern int rgb2rgba(unsigned char *rgba, unsigned char *rgb, int size);
extern int topBottom(unsigned char *rgba, int sizeX, int sizeY);

#endif