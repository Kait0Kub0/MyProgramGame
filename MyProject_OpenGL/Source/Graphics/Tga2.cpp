//----------------------------------------------------------------------
//  Copyright (C) 2014 by Human
//  OpenGL プログラム
//----------------------------------------------------------------------
#include "../system.h"

// プロトタイプ宣言
int TGA_image_2_24( FILE *fp );     // 24bit 画像読み込み
int TGA_image_2_32( FILE *fp );     // 32bit 画像読み込み
int TGA_ERREND( void );             // エラー処理関数

// TGA読み込み用ヘッダ
TGA_HED         TgaHed;

// TGA 作業用変数
TEX_DAT     *TgaImage;  // 画像データ管理変数ポインタ
FILE        *TgaFp;     // ファイルポインタ

// 画像データ操作用変数
unsigned char   *tga24rgb, *tga32rgba;
unsigned char   *tgatex32rgba;

//----------------------------------------------------------------------
//  テクスチャ作成
//----------------------------------------------------------------------
TEX_DAT * TGA_makeImage(const char *filename )
{
    int     ret;
    size_t  tex32rgbaSize;

    TgaImage = NULL;    // 画像データ管理変数ポインタ
    TgaFp    = NULL;    // ファイルポインタ

    tga24rgb     = NULL;    // TGA 24bit 画像データポインタ初期化
    tga32rgba    = NULL;    // TGA 32bit 画像データポインタ初期化
    tgatex32rgba = NULL;    // TEX 32bit 画像データポインタ初期化


    // 画像データ管理用構造体の作成
    TgaImage = (TEX_DAT *)malloc( sizeof(TEX_DAT) );
    if( TgaImage == NULL )  // 領域確保失敗
    {
        TGA_ERREND();
        return  FALSE;
    }
    memset( (char*)TgaImage, 0x00, sizeof(TEX_DAT) ); // 領域初期化


    // ファイルオープン
    TgaFp = fopen( filename, "rb" );
    if( TgaFp == NULL )     // オープン失敗
    {
        TGA_ERREND();
        return  FALSE;
    }

    // ヘッダ情報 読み込み（間違い例）
//    ret = fread( &TgaHed, 1, sizeof(TEX_DAT), TgaFp );
//    if(ret == FALSE)    return  FALSE;

    TgaHed.id_length      = fgetc( TgaFp );     // ID フィールド長
    TgaHed.cmap_type      = fgetc( TgaFp );     // カラーマップ有無（0:無し 1:有り）
    TgaHed.image_type     = fgetc( TgaFp );     // 2=無圧縮RGBAタイプ 10=圧縮RGBAタイプ

    TgaHed.cmap_index     = fgetwc( TgaFp );    // カラーマップインデックス
    TgaHed.cmap_length    = fgetwc( TgaFp );    // カラーマップレングス
    TgaHed.cmap_size      = fgetc ( TgaFp );    // カラーマップサイズ

    TgaHed.x_origin       = fgetwc( TgaFp );    // 画像 左上 X 座標
    TgaHed.y_origin       = fgetwc( TgaFp );    // 画像 左上 Y 座標

    TgaHed.width          = fgetwc( TgaFp );    // 幅   (dot)
    TgaHed.height         = fgetwc( TgaFp );    // 高さ (dot)

    TgaHed.pixel_size     = fgetc ( TgaFp );    // Pixel Size. 32=32bit

    TgaHed.attributes     = fgetc ( TgaFp );    // アトリビュート


    // 未対応データチェック
    // 圧縮データ未対応
    if( TgaHed.image_type != 2 )
    {
        TGA_ERREND();
        return FALSE;
    }

    // インデックスデータ未対応
    if( TgaHed.cmap_type != 0 )
    {
        TGA_ERREND();
        return FALSE;
    }

    // コメントデータのスキップ
    if( TgaHed.id_length != 0 )
    {
        fseek( TgaFp, TgaHed.id_length, SEEK_CUR );
    }

    // サイズセット
    TgaImage->sizeX = TgaHed.width;
    TgaImage->sizeY = TgaHed.height;


    // テクスチャデータ領域確保
    tex32rgbaSize = TgaHed.width * TgaHed.height * 4;       // 32bit テクスチャサイズ計算
    tgatex32rgba = (unsigned char *)malloc( tex32rgbaSize );   // 領域確保
    if( tgatex32rgba == NULL ) return  FALSE;


    // 無圧縮RGBデータ
    switch( TgaHed.pixel_size )
    {
        case 24:
            ret = TGA_image_2_24( TgaFp );
            break;
        case 32:
            ret = TGA_image_2_32( TgaFp );
            break;
        default:
            // 未対応データ
            ret = FALSE;
            break;
    }

    // エラーチェック
    if( ret == FALSE )
    {
        TGA_ERREND();
        return FALSE;
    }
    else
    {
        TgaImage->data = tgatex32rgba;
    }

    // ファイルクローズ
    fclose( TgaFp );

    return  TgaImage;
}


//----------------------------------------------------------------------
//  24 ビットの処理
//----------------------------------------------------------------------
int TGA_image_2_24( FILE *fp )
{
    int             width, height;
    size_t          readSize;
    size_t          tga24rgbSize;

    unsigned char   *tgabuf;
    unsigned char   *texbuf;


    texbuf = tgatex32rgba;  // 作業用ポインタにアドレスコピー


    // TGA 画像データ領域確保
    tga24rgbSize = TgaHed.width * TgaHed.height * 3;        // 24bit TGA 画像データ部サイズ計算
    tga24rgb = (unsigned char *)malloc( tga24rgbSize );     // 領域確保
    if( tga24rgb == NULL )  return  FALSE;
    tgabuf = tga24rgb;                                      // 作業用ポインタにアドレスコピー


    // TGA 画像データ読み込み
    readSize = fread( tgabuf, sizeof(unsigned char), tga24rgbSize, TgaFp );
    if( readSize != tga24rgbSize )  return  FALSE;


    // データ入れ替え
    for( height=0; height<TgaHed.height; height++ )
    {
        for( width=0; width<TgaHed.width; width++ )
        {
            *(texbuf+0) = *(tgabuf+2);      // RGBA データ書き込み( R )
            *(texbuf+1) = *(tgabuf+1);      // RGBA データ書き込み( G )
            *(texbuf+2) = *(tgabuf+0);      // RGBA データ書き込み( B )
            *(texbuf+3) = 255;              // RGBA データ書き込み( A )

            tgabuf += 3;    // TGA 画像データ取り出しアドレス更新
            texbuf += 4;    // TXE 画像データ書き込みアドレス更新
        }
    }

    // TGA 画像データ領域開放
    free( tga24rgb );

    return TRUE;
}


//----------------------------------------------------------------------
//  32 ビットの処理
//----------------------------------------------------------------------
int TGA_image_2_32( FILE *fp )
{
    int             width, height;
    size_t          readSize;
    size_t          tga32rgbaSize;

    unsigned char   *tgabuf;
    unsigned char   *texbuf;


    texbuf = tgatex32rgba;  // 作業用ポインタにアドレスコピー


    // TGA 画像データ領域確保
    tga32rgbaSize = TgaHed.width * TgaHed.height * 4;       // 32bit TGA 画像データ部サイズ計算
    tga32rgba = (unsigned char *)malloc( tga32rgbaSize );   // 領域確保
    if( tga32rgba == NULL ) return  FALSE;
    tgabuf = tga32rgba;                                     // 作業用ポインタにアドレスコピー


    // TGA 画像データ読み込み
    readSize = fread( tgabuf, sizeof(unsigned char), tga32rgbaSize, TgaFp );
    if( readSize != tga32rgbaSize )  return  FALSE;


    // データ入れ替え
    for( height=0; height<TgaHed.height; height++ )
    {
        for( width=0; width<TgaHed.width; width++ )
        {
            *(texbuf+0) = *(tgabuf+2);      // RGBA データ書き込み( R )
            *(texbuf+1) = *(tgabuf+1);      // RGBA データ書き込み( G )
            *(texbuf+2) = *(tgabuf+0);      // RGBA データ書き込み( B )
            *(texbuf+3) = *(tgabuf+3);      // RGBA データ書き込み( A )

            tgabuf += 4;                // TGA 画像データ取り出しアドレス更新
            texbuf += 4;                // TXE 画像データ書き込みアドレス更新
        }
    }

    // TGA 画像データ領域開放
    free( tga32rgba );

    return TRUE;
}


//----------------------------------------------------------------------
//  エラー処理
//----------------------------------------------------------------------
int TGA_ERREND( void )
{

//    MessageBox( NULL, "TGA ファイル読み込み失敗", "TGA", MB_OK );

    // 画像データ管理ポインタ初期化
    if( TgaImage != NULL )
    {
        if( TgaImage->data != NULL )
        {
            free( TgaImage->data );
             TgaImage->data = NULL;
        }

        free( TgaImage );
        TgaImage = NULL;
    }


    // ファイル管理ポインタ初期化
    if( TgaFp != NULL )
    {
        fclose( TgaFp );
        TgaFp = NULL;
    }


    // TGA 24 bit 画像管理ポインタ初期化
    if( tga24rgb != NULL )
    {
        free( tga24rgb );
        tga24rgb = NULL;
    }


    // TGA 32 bit 画像管理ポインタ初期化
    if( tga32rgba != NULL )
    {
        free( tga32rgba );
        tga32rgba = NULL;
    }


    // TEX 32 bit 画像管理ポインタ初期化
    if( tgatex32rgba != NULL )
    {
        free( tgatex32rgba );
        tgatex32rgba = NULL;
    }

    return TRUE;
}