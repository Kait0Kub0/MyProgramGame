//----------------------------------------------------------------------
//  Copyright (C) 2014 by Human
//  OpenGL プログラム
//----------------------------------------------------------------------
#include "../system.h"


char ChkFileFormat[ CHK_FILE_MAX ][4] =
{
    BMP_FILE_END,
	TGA_FILE_END,
	PNG_FILE_END,
};


//----------------------------------------------------------------------
//  テクスチャ登録処理
//
// GLuint texno  : テクスチャ管理番号
// char  *f_name : ファイル名
// GLuint wrap   : テクスチャテクスチャ座標     GL_REPEAT :GL_CLAMP
// GLuint filter : テクスチャ拡大・縮小補間処理 GL_NEAREST:GL_LINEAR
//----------------------------------------------------------------------
TEX_DAT * TextureGL( GLuint texno, const char *f_name, GLuint wrap, GLuint filter)
{
    TEX_DAT *image;
    char    *pne;
    int     extension_len;
	int		fno = -1;

	// 拡張子検索
	pne = GetExtension(f_name);
	if (pne != NULL)
	{
		extension_len = (int)strlen((const char *)pne);
		for (int i = 0; i < CHK_FILE_MAX; i++)
		{
			if (_strnicmp(pne, ChkFileFormat[i], strlen(ChkFileFormat[i])) == 0)
			{
				fno = i;
				break;
			}
		}
	}

    // テクスチャ読み込み（ファイル分岐）
	switch (fno)
    {
    case BITMAP_FILE_DAT:
        image = BMP_makeImage( f_name );
        break;

	case TARGA_FILE_DAT:
		image = TGA_makeImage(f_name);
		break;

	case PNG_FILE_DAT:
		image = PNG_makeImage(f_name);
		break;

	default:
        image = NULL;
        break;
    }

    // テクスチャ読込み失敗
    if( image == NULL )
    {
        return NULL;
    }


    // テクスチャ番号選択
    glBindTexture( GL_TEXTURE_2D, texno );


    // ミップマップ　OFF
    if (filter >= GL_NEAREST && filter <= GL_LINEAR)
    {
        //メインメモリからVRAMに画像転送
        glTexImage2D( GL_TEXTURE_2D,                    // 2Dテクスチャ使用
                      0,                                // ミプマップレベル
                      GL_RGBA,                          // テクスチャカラー要素数
                      image->sizeX,                     // テクスチャ画像の幅   ※絵の大きさは２のｎ乗
                      image->sizeY,                     // テクスチャ画像の高さ ※絵の大きさは２のｎ乗
                      0,                                // 境界線の幅を指定 [0:1](ドット)
                      GL_RGBA,                          // 画像データのフォーマット
                      GL_UNSIGNED_BYTE,                 // 画像データのデータ形式
                      image->data );                    // メインメモリ上の画像データのアドレス
    }

    // ミップマップ ON
    if (filter >= GL_NEAREST_MIPMAP_NEAREST && filter <= GL_LINEAR_MIPMAP_LINEAR)
    {
        // ミップマップON
        //メインメモリからVRAMに画像転送
        gluBuild2DMipmaps(
            GL_TEXTURE_2D, 
            GL_RGBA, 
            image->sizeX, 
            image->sizeY,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            image->data);
    }


    // テクスチャパラメータ設定
    // テクスチャ座標（繰り返し・クランプ）設定
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap );     // S 座標
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap );     // T 座標

    // テクスチャ拡大・縮小補間処理
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter );    // 拡大時の設定
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter );    // 縮小時の設定

    return image;
}


//----------------------------------------------------------------------
// テクスチャ削除処理
//----------------------------------------------------------------------
void TextureDelGL( GLuint texno )
{
    glDeleteTextures( 1, &texno );
}


//----------------------------------------------------------------------
// 画像データ領域開放処理
//----------------------------------------------------------------------
void TexDatFree( TEX_DAT **image )
{
    TEX_DAT *work;

    // 画像データ管理用構造体開放処理
    if( *image != NULL )
    {

        work = *image;

        // 画像データ部開放処理
        if( work->data != NULL )
        {
            free( work->data );
            work->data = NULL;
        }

        free( *image );
        *image = NULL;
    }

}

//==========================================================================
// 拡張子の取得処理
//==========================================================================
char * GetExtension(const char *pn)
{
	char *pne;

	pne = (char *)strchr(pn, 0x00);

	// 文字列の最後から最初まで
	while (pne >= pn)
	{
		if ('\\' == *pne)  return NULL;    // ファイル名終了
		if ('.' == *pne)  return pne + 1;   // '.' を発見

		pne = (char *)_mbsdec((const unsigned char *)pn, (const unsigned char *)pne);
	}

	return NULL;
}

//==========================================================================
// rgb→rgba 処理
//==========================================================================
int rgb2rgba(unsigned char *rgba, unsigned char *rgb, int size)
{
	int loop;

	if (rgb == NULL || rgba == NULL) return FALSE;


	for (loop = 0; loop<(size / 3); loop++)
	{
		BYTE r, g, b, a;
		r = *rgb++; // R
		g = *rgb++; // G
		b = *rgb++; // B
		a = 255;    // A

		*rgba++ = r;
		*rgba++ = g;
		*rgba++ = b;
		*rgba++ = a;
	}

	return TRUE;
}

//==========================================================================
//  処理
//==========================================================================
int topBottom(unsigned char *rgba, int sizeX, int sizeY)
{
    int loop;
    unsigned char *work;
    int sizeAll = sizeX * sizeY * 4;
    int lineSize = (sizeX *4);

    if (rgba == NULL) return FALSE;

    // 領域確保
    work = (unsigned char *)malloc(sizeAll);

    // データ入れ替え    
    for (loop = 0; loop<sizeY; loop++)
    {
        memcpy(work + lineSize * loop, rgba + (sizeAll - lineSize * (loop + 1)), lineSize);
    }

    // データ入れ替え
    memcpy(rgba, work, sizeAll);

    // 領域解放
    free(work);

    return TRUE;
}
