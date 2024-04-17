//----------------------------------------------------------------------
//  Copyright (C) 2014 by Human
//  OpenGL プログラム
//----------------------------------------------------------------------
#ifndef _MY_TGA_INCLUDE
#define _MY_TGA_INCLUDE

// ターガファイルヘッダ
typedef struct
{
    unsigned char   id_length;      // ID フィールド長
    unsigned char   cmap_type;      // カラーマップ有無（0:無し 1:有り）
    unsigned char   image_type;     // 2=無圧縮RGBAタイプ 10=圧縮RGBAタイプ
    unsigned short  cmap_index;     // カラーマップインデックス
    unsigned short  cmap_length;    // カラーマップレングス
    unsigned char   cmap_size;      // カラーマップサイズ
    unsigned short  x_origin;       // 画像 左上 X 座標
    unsigned short  y_origin;       // 画像 左上 Y 座標
    unsigned short  width;          // 幅   (dot)
    unsigned short  height;         // 高さ (dot)
    unsigned char   pixel_size;     // Pixel Size. 32=32bit
    unsigned char   attributes;     // アトリビュート
} TGA_HED;


extern  void      TextureDelGL( GLuint texno );
extern  void      TexDatFree( TEX_DAT **image );

extern  TEX_DAT * TGA_makeImage(const char *filename );


#endif