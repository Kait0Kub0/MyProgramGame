//----------------------------------------------------------------------
//  Copyright (C) 2021 by Human
//  OpenGL プログラム
//----------------------------------------------------------------------
#include "../system.h"

// プロトタイプ宣言
int BMP_image_8 ( FILE *fp );   //  8bit 画像読み込み
int BMP_image_24( FILE *fp );   // 24bit 画像読み込み
int BMP_ERREND( void );         // エラー処理関数

// BMP読み込み用ヘッダ
BITMAPFILEHEADER    Bmp_f;
BITMAPINFOHEADER    Bmp_i;

// TGA 作業用変数
TEX_DAT     *BmpImage;  // 画像データ管理変数ポインタ
FILE        *BmpFp;     // ファイルポインタ

// 画像データ操作用変数
unsigned char   *bmp8index, *bmp24rgb;
unsigned char   *bmptex32rgba;

/*----------------------------------------------------------------------*/
/* テクスチャ作成                                                       */
/*----------------------------------------------------------------------*/
TEX_DAT * BMP_makeImage(const char *filename )
{
    int     ret;
    size_t  tex32rgbaSize;
    size_t  readSize;

    BmpImage = NULL;    // 画像データ管理変数ポインタ
    BmpFp    = NULL;    // ファイルポインタ

    bmp8index    = NULL;    // BMP  8bit 画像データポインタ初期化
    bmp24rgb     = NULL;    // BMP 24bit 画像データポインタ初期化
    bmptex32rgba = NULL;    // TEX 32bit 画像データポインタ初期化


    // 画像データ管理用構造体の作成
    BmpImage = (TEX_DAT *)malloc( sizeof(TEX_DAT) );
    if( BmpImage == NULL )
    {
        BMP_ERREND();
        return  FALSE;
    }
    memset( (char *)BmpImage, 0x00, sizeof(TEX_DAT) );


    // ファイルオープン
    BmpFp = fopen( filename, "rb");
    if( BmpFp == NULL )
    {
        BMP_ERREND();
        return  FALSE;
    }


    // BITMAPFILEHEADERの読み込み
    readSize = fread( &Bmp_f, sizeof(BITMAPFILEHEADER), 1, BmpFp);
    if( readSize == 0 )
    {
        BMP_ERREND();
        return FALSE;
    }


    // BITMAPINFOHEADERの読み込み
    readSize = fread( &Bmp_i, sizeof(BITMAPINFOHEADER), 1, BmpFp);
    if( readSize == 0 )
    {
        BMP_ERREND();
        return FALSE;
    }


    // サイズセット
    BmpImage->sizeX = Bmp_i.biWidth;
    BmpImage->sizeY = Bmp_i.biHeight;


    // テクスチャ画像データ格納用領域確保
    tex32rgbaSize = Bmp_i.biWidth * Bmp_i.biHeight * 4;
    bmptex32rgba = (unsigned char *)malloc( tex32rgbaSize );
    if(bmptex32rgba == NULL) return  FALSE;


    // 無圧縮データ
    switch(Bmp_i.biBitCount)
    {
        case 8:
            // 8ビット（256色）パレットインデックスデータ
            ret = BMP_image_8( BmpFp );
            break;
        case 24:
            // 24ビットデータ
            ret = BMP_image_24( BmpFp );
            break;
        default:
            // 未対応データ
            ret = FALSE;
            break;
    }

    // エラーチェック
    if( ret == FALSE )
    {
        BMP_ERREND();
        return FALSE;
    }
    else
    {
        BmpImage->data = bmptex32rgba;
    }


    // ファイルクローズ
    fclose(BmpFp);

    return  BmpImage;
}


/*----------------------------------------------------------------------*/
/* 8ビット（256色）パレットインデックスデータ                           */
/*----------------------------------------------------------------------*/
int BMP_image_8( FILE *fp )
{
    int         width, height;
    int         PaletteCnt;
    int         PaletteNo;
    RGBQUAD     p[256];

    size_t      readSize;
    size_t      bmp8indexSize;

    unsigned char   *bmpbuf;
    unsigned char   *texbuf;


    texbuf = bmptex32rgba;  // 作業用ポインタにアドレスコピー


    // BMP 画像データ領域確保
    bmp8indexSize = Bmp_i.biWidth * Bmp_i.biHeight;     // ピクセルデータの容量を計算
    bmp8index = (unsigned char *)malloc( bmp8indexSize );
    if(bmp8index == NULL) return  FALSE;
    bmpbuf = bmp8index;

    // パレット読み込み
    if( Bmp_i.biClrUsed == 0 )
    {
        PaletteCnt = 256;
    }
    else
    {
        PaletteCnt = Bmp_i.biClrUsed;
    }

    // BMPパレットの読み込み
    readSize = fread( p, sizeof(RGBQUAD), PaletteCnt, fp );
    if( readSize != PaletteCnt ) return FALSE;

    // BMP 画像データ読み込み
    readSize = fread( bmpbuf, sizeof(unsigned char), bmp8indexSize, fp );
    if(readSize != bmp8indexSize) return FALSE;

    // テクスチャ作成
    for( height=0; height<Bmp_i.biHeight; height++ )
    {
        for( width=0; width<Bmp_i.biWidth; width++ )
        {
            PaletteNo = *bmpbuf;
            *(texbuf+0) = p[ PaletteNo ].rgbRed;       // 赤 R
            *(texbuf+1) = p[ PaletteNo ].rgbGreen;     // 緑 G
            *(texbuf+2) = p[ PaletteNo ].rgbBlue;      // 青 B
            *(texbuf+3) = 255;                         // アルファ値 A

            if( *(texbuf+0) + *(texbuf+1) + *(texbuf+2) == 0 )
            {
                *(texbuf+3) = 0;                       // アルファ値 A
            }

            bmpbuf += 1;
            texbuf += 4;
        }
    }

    free( bmp8index );

    return TRUE;
}


/*----------------------------------------------------------------------*/
/* パレット24ビットデータの処理                                         */
/*----------------------------------------------------------------------*/
int BMP_image_24( FILE *fp )
{
    int         width, height;
    int         iLine;

    size_t      readSize;
    size_t      bmp24rgbSize;

    unsigned char   *pLine;
    unsigned char   *bmpbuf;
    unsigned char   *texbuf;


    texbuf = bmptex32rgba;  // 作業用ポインタにアドレスコピー


    // 幅を偶数にする
    iLine = Bmp_i.biWidth + (Bmp_i.biWidth & 1);

    // １ラインのデータ数セット
    iLine *= 3;
    if ( (iLine % 4) != 0 )
    {
        iLine += 4-(iLine % 4);
    }

    // BMP 画像データ領域確保
    bmp24rgbSize = iLine * Bmp_i.biHeight;  // 24bit BMP 画像データ部サイズ計算
    bmp24rgb = (unsigned char *)malloc( bmp24rgbSize );
    if( bmp24rgb == NULL ) return  FALSE;
    bmpbuf = bmp24rgb;


    // BMP 画像データ読み込み
    readSize = fread( bmp24rgb, sizeof(unsigned char), bmp24rgbSize, fp );
    if( readSize != bmp24rgbSize ) return FALSE;    // 読み取り失敗


    // データ入れ替え
    for( height=0; height< Bmp_i.biHeight; height++ )
    {
        pLine = bmpbuf;
        // 24bit→32bit変換
        for ( width=0; width<Bmp_i.biWidth; width++ )
        {
            *(texbuf+0) = *(pLine + 2);             // RGB データ取り出し( B )
            *(texbuf+1) = *(pLine + 1);             // RGB データ取り出し( G )
            *(texbuf+2) = *(pLine + 0);             // RGB データ取り出し( R )
            *(texbuf+3) = 255;                      // アルファ値 A

            if( *(texbuf+0) + *(texbuf+1) + *(texbuf+2) == 0 )
            {
                *(texbuf+3) = 0;                    // アルファ値 A
            }

            pLine  += 3;    // rgb  3 Byte
            texbuf += 4;    // rgba 4 Byte
        }
        bmpbuf += iLine;    // 1 ライン分ポインタ移動
    }

    free( bmp24rgb );

    return TRUE;
}

/*----------------------------------------------------------------------*/
/* エラー処理                                                           */
/*----------------------------------------------------------------------*/
int BMP_ERREND( void )
{

//    MessageBox( NULL, "BMP ファイル読み込み失敗", "BMP ERROR", MB_OK );

    // 画像データ管理ポインタ初期化
    if( BmpImage != NULL )
    {
        if( BmpImage->data != NULL )
        {
            free( BmpImage->data );
             BmpImage->data = NULL;
        }

        free( BmpImage );
        BmpImage = NULL;
    }


    // ファイル管理ポインタ初期化
    if( BmpFp != NULL )
    {
        fclose( BmpFp );
        BmpFp = NULL;
    }


    // BMP 8 bit 画像管理ポインタ初期化
    if( bmp8index != NULL )
    {
        free( bmp8index );
        bmp8index = NULL;
    }


    // BMP 24 bit 画像管理ポインタ初期化
    if( bmp24rgb != NULL )
    {
        free( bmp24rgb );
        bmp24rgb = NULL;
    }


    // TEX 32 bit 画像管理ポインタ初期化
    if( bmptex32rgba != NULL )
    {
        free( bmptex32rgba );
        bmptex32rgba = NULL;
    }

    return TRUE;
}