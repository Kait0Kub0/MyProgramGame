//----------------------------------------------------------------------
//  Copyright (C) 2021 by Human
//  PoligonModel 読み込み
//----------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include "../../system.h"

#include "PolRead.h"



char    *f_pol = NULL;      // POL ファイルデータ

OBJ_VARIABLE    *obj;       // インデックス型 ポリゴンオブジェクト
OBJ_VERTEX_BUF  *ver_buf;   // シークエンス型 ポリゴンオブジェクト
OBJ_VERTEX_BUF2 *ver_buf2;  // シークエンス型 ポリゴンオブジェクト


int OBJ_ERREND1( void );
int OBJ_ERREND2( void );
int OBJ_ERREND3( void );


//----------------------------------------------------------------------
//  LoadPolFile    POLファイル読み込み処理
//  形式
//      OBJ_VARIABLE * LoadPolFile( char *f_name )
//  引数
//      f_name      ファイル名
//      r           データバッファ
//  返値
//      int         正常:TRUE
//                  異常:FALSE
//  解説
//     ファイルサイズを調べてデータを読み込む領域を確保します。
//     確保された領域にファイルから全データを読み込みます。
//----------------------------------------------------------------------
OBJ_VARIABLE * LoadPolFile(const char *f_name )
{
    int     ret =TRUE;          // 結果
    int     p_size =0;          // POL ファイルサイズ

    // ポインタ初期化
    f_pol   = NULL;
    obj     = NULL;
    ver_buf = NULL;
    ver_buf2= NULL;

    // ファイル読み込み
    // ポリゴンデータ読み込み
    f_pol = LoadTexFile( f_name, &p_size );
    // データ領域の確保チェック
    if( f_pol == NULL )
    {
        OBJ_ERREND1();
        return FALSE;
    }

    // ポリゴンデータ領域確保
    obj = new OBJ_VARIABLE;
    if( obj == NULL )
    {
        OBJ_ERREND1();
        return FALSE;
    }
    memset( obj, 0x00, sizeof(OBJ_VARIABLE) );

    // テキストから数値に変換
    ret = ObjPoly( f_pol, p_size, obj );
    if( ret == NULL )
    {
        OBJ_ERREND1();
        return FALSE;
    }

    // テキスト領域開放
    delete [] f_pol;

    return obj;
}


//----------------------------------------------------------------------
//  LoadTexFile    ファイルの全データ読み込み
//  形式
//      char * LoadTexFile( char *fname, int *fsize )
//  引数
//      fname       ファイル名
//      buf         データバッファ
//  返値
//      int         正常であればファイルサイズを返します。
//  解説
//     ファイルサイズを調べてデータを読み込む領域を確保します。
//     確保された領域にファイルから全データを読み込みます。
//----------------------------------------------------------------------
char * LoadTexFile(const char *fname, int *fsize )
{
    FILE    *fp;
    size_t  size;
    char    *buf;
    int     loop;

    obj     = NULL;
    ver_buf = NULL;

    fp = fopen( fname, "rt" );                      // ファイルを開く
    if( fp == NULL )    return FALSE;
    fseek( fp, 0, SEEK_END );                       // ファイルサイズ取得
    size = ftell( fp );

    buf = new char[size];                           // バッファ確保
    if( buf == NULL )
    {
        fclose( fp );
       return FALSE;
    }

    fseek( fp, 0, SEEK_SET );                       // ファイルの先頭に戻る
    size = fread( buf, sizeof(char), size, fp );    // 全データをバッファに読み込む
    fclose( fp );                                   // ファイルを閉じる

    *fsize = (int)size;

    // テキストファイルの改行を文字列終了コードに変換
    for( loop=0; loop<*fsize; loop++ )
    {
        if( *(buf+loop) == 0x0A )
        {
            *(buf+loop) = 0x00;
        }
    }

    return buf;
}


//----------------------------------------------------------------------
//  ObjPoly     POLデータ変換
//  形式
//      int ObjPoly( char *buf, int size, Obj_VARIABLE *r )
//  引数
//      buf     データバッファ
//      r       ファイル名
//  返値
//      int     正常:TRUE
//              異常:FALSE
//  解説
//     頂点数分のデータ領域を確保します。
//     確保された領域にデータをセットします。
//----------------------------------------------------------------------
int ObjPoly( char *buf, int size, OBJ_VARIABLE *r )
{
    int     loop;
    int     ver_cnt;    // 頂点数
    int     nor_cnt;    // 法線数
    int     col_cnt;    // カラー数
    int     tex_cnt;    // テクスチャ座標数
    int     pol_cnt;    // ポリゴン数
    int     point;      // データポイント

    vec3        *vp;    // 頂点データ用ポインタ
    vec3        *np;    // 法線データ用ポインタ
    vec4        *cp;    // カラーデータ用ポインタ
    vec2        *tp;    // テクスチャデータ用ポインタ
    OBJ_POLY    *pp;    // ポリゴンデータ用ポインタ

    // データ開始行の検索
    point = BufChk("# Data Count", buf, size );
    if( point == FALSE ) return FALSE;
    // 頂点数：法線数：カラー数：ポリゴン数
    sscanf( (buf+point), "%d %d %d %d %d", &ver_cnt, &nor_cnt, &col_cnt, &tex_cnt, &pol_cnt );


    // データ数設定
    r->tex_cnt = tex_cnt;   // テクスチャ座標数
    r->col_cnt = col_cnt;   // カラー数
    r->nol_cnt = nor_cnt;   // 法線数
    r->ver_cnt = ver_cnt;   // 頂点座標数
    r->pol_cnt = pol_cnt;   // ポリゴン数


    // データ領域確保
    r->obj_pol = new OBJ_POLY[pol_cnt];
    r->obj_v   = new vec3[ver_cnt];
    r->obj_n   = new vec3[nor_cnt];
    r->obj_c   = new vec4[col_cnt];
    r->obj_t   = new vec2[tex_cnt];

    if( (r->obj_pol == NULL)
     || (r->obj_v   == NULL)
     || (r->obj_n   == NULL)
     || (r->obj_c   == NULL)
     || (r->obj_t   == NULL) )  return FALSE;

    // 頂点データ開始行検索
    point = BufChk( "# Vertex Data", buf, size );
    if( point == FALSE ) return FALSE;
    // 頂点データセット */
    for( loop=0; loop<ver_cnt; loop++ )
    {
        vp = (r->obj_v)+loop;
        sscanf( (buf+point), "%f,%f,%f", &vp->x, &vp->y, &vp->z );
        point += (int)strlen( (buf+point) ) + 1;        // １行分移動
    }

    // 法線データ開始行検索
    point = BufChk( "# Normal Data", buf, size );
    if( point == FALSE ) return FALSE;
    // 法線データセット
    for( loop=0; loop<nor_cnt; loop++ )
    {
        np = (r->obj_n)+loop;
        sscanf( (buf+point), "%f,%f,%f", &np->x, &np->y, &np->z );
        point += (int)strlen( (buf+point) ) + 1;        // １行分移動
    }

    // カラーデータ開始行検索
    point = BufChk( "# Color Data", buf, size );
    if( point == FALSE ) return FALSE;
    // カラーデータセット
    for( loop=0; loop<col_cnt; loop++ )
    {
        cp = (r->obj_c)+loop;
        sscanf( (buf+point), "%f,%f,%f,%f", &cp->r, &cp->g, &cp->b, &cp->a );
        point += (int)strlen( (buf+point) ) + 1;        // １行分移動
    }

    // テクスチャデータ開始行検索
    point = BufChk( "# Texture Data", buf, size );
    if( point == FALSE ) return FALSE;
    // テクスチャデータセット
    for( loop=0; loop<tex_cnt; loop++ )
    {
        tp = (r->obj_t)+loop;
        sscanf( (buf+point), "%f,%f", &tp->s, &tp->t );
        point += (int)strlen( (buf+point) ) + 1;        // １行分移動
    }

    // ポリゴンデータ開始行検索
    point = BufChk( "# Polygon Data", buf, size );
    if( point == FALSE ) return FALSE;
    // ポリゴンデータセット
    for( loop=0; loop<pol_cnt; loop++ )
    {
        pp = (r->obj_pol)+loop;
        sscanf( (buf+point), "%d,%d,%d : %d,%d,%d : %d,%d,%d : %d,%d,%d",
                        &pp->v[0], &pp->v[1], &pp->v[2],    // 頂点インデックス
                        &pp->n[0], &pp->n[1], &pp->n[2],    // 法線インデックス
                        &pp->c[0], &pp->c[1], &pp->c[2],    // カラーインデックス
                        &pp->t[0], &pp->t[1], &pp->t[2] );  // テクスチャインデックス
        point += (int)strlen( (buf+point) ) + 1;        // １行分移動
    }

    return TRUE;
}


//----------------------------------------------------------------------
//  BufChk      データ検索
//  形式
//      int     BufChk( char *chk, char *buf, int size )
//  引数
//      chk     チェックデータ
//      buf     データバッファ
//      size    データバッファのサイズ
//  返値
//      int     正常:データの見つかった位置を返します。
//              異常:FALSE
//  解説
//     指定されたデータをバッファから検索します。
//----------------------------------------------------------------------
int BufChk( const char *chk, char *buf, int size )
{
    int     leng;                       // １行の文字数
    int     point=0;                    // 移動ポインタ
    int     chk_size;                   // チェック文字数

    chk_size = (int)strlen( chk );      // チェック文字数

    // バッファ内検索
    while( point<size )
    {
        leng = (int)strlen(buf) + 1;                // １行の文字数
        point += leng;                              // バッファ位置移動
        if( memcmp( chk, buf, chk_size ) == 0 )     // 文字列を比較する
        {
            return point;                           // バッファ位置を返す
        }
        buf += leng;                                // バッファを１行分進める
    }

    return FALSE;                                   // データ無し
}


//----------------------------------------------------------------------
//  VertexBufSet 頂点バッファ用データ領域へコピー
//  形式
//      OBJ_VERTEX_BUF * VertexBufSet( OBJ_VARIABLE *obj_dat )
//  引数
//  返値
//  解説
//----------------------------------------------------------------------
OBJ_VERTEX_BUF * VertexBufSet( OBJ_VARIABLE *obj_dat )
{
    int loop0, loop1;
    int dat_cnt;
    int index_no;


    OBJ_POLY    *obj_pol;             // ポリゴンインデックス バッファ

    vec2        *obj_t0, *obj_t1;     // テクスチャバッファ
    vec4        *obj_c0, *obj_c1;     // カラーバッファ
    vec3        *obj_n0, *obj_n1;     // 法線バッファ
    vec3        *obj_v0, *obj_v1;     // 頂点バッファ

    // ポインタ初期化
    obj     = NULL;
    ver_buf = NULL;

    dat_cnt = obj_dat->pol_cnt * 3;

    ver_buf = new OBJ_VERTEX_BUF;
    memset( ver_buf, 0x00, sizeof( OBJ_VERTEX_BUF ) );


    ver_buf->pol_cnt = obj_dat->pol_cnt;


    // 頂点データ領域確保
    ver_buf->obj_v = new vec3[ dat_cnt ];

    obj_v0  = ver_buf->obj_v;   // コピー先バッファ
    obj_v1  = obj_dat->obj_v;   // コピー元バッファ
    obj_pol = obj_dat->obj_pol; // ポリゴンデーター先頭アドレス

    for( loop0=0; loop0<obj_dat->pol_cnt; loop0++ )
    {
        for( loop1=0; loop1<3; loop1++ )
        {
            index_no = obj_pol->v[loop1];       // インデックス番号取り出し
            *obj_v0 = *(obj_v1 + index_no);     // データコピ－
//            *obj_v0 = obj_v1[index_no];
            obj_v0++;                           // バッファアドレス更新
        }
        obj_pol++;
    }


    // 法線データ領域確保
    ver_buf->obj_n = new vec3[ dat_cnt ];

    obj_n0  = ver_buf->obj_n;   // コピー先バッファ
    obj_n1  = obj_dat->obj_n;   // コピー元バッファ
    obj_pol = obj_dat->obj_pol; // ポリゴンデーター先頭アドレス

    for( loop0=0; loop0<obj_dat->pol_cnt; loop0++ )
    {
        for( loop1=0; loop1<3; loop1++ )
        {
            index_no = obj_pol->n[loop1];
            *obj_n0 = *(obj_n1 + index_no);
//            *obj_n0 = obj_n1[index_no];
            obj_n0++;
        }
        obj_pol++;
    }


    // カラーデータ領域確保
    ver_buf->obj_c = new vec4[ dat_cnt ];

    obj_c0 = ver_buf->obj_c;    // コピー先バッファ
    obj_c1 = obj_dat->obj_c;    // コピー元バッファ
    obj_pol = obj_dat->obj_pol; // ポリゴンデーター先頭アドレス

    for( loop0=0; loop0<obj_dat->pol_cnt; loop0++ )
    {
        for( loop1=0; loop1<3; loop1++ )
        {
            index_no = obj_pol->c[loop1];
            *obj_c0 = *(obj_c1 + index_no);
//            *obj_c0 = obj_c1[index_no];
            obj_c0++;
        }
        obj_pol++;
    }


    // テクスチャデータ領域確保
    ver_buf->obj_t = new vec2[ dat_cnt ];

    obj_t0 = ver_buf->obj_t;    // コピー先バッファ
    obj_t1 = obj_dat->obj_t;    // コピー元バッファ
    obj_pol = obj_dat->obj_pol; // ポリゴンデーター先頭アドレス

    for( loop0=0; loop0<obj_dat->pol_cnt; loop0++ )
    {
        for( loop1=0; loop1<3; loop1++ )
        {
            index_no = obj_pol->t[loop1];
            *obj_t0 = *(obj_t1 + index_no);
//            *obj_t0 = obj_t1[index_no];
            obj_t0++;
        }
        obj_pol++;
    }

    return ver_buf;
}


//----------------------------------------------------------------------
//  VertexBufSet2 頂点バッファ用データ領域へコピー
//  形式
//      OBJ_VERTEX_BUF2 * VertexBufSet2( OBJ_VARIABLE *obj_dat )
//  引数
//  返値
//  解説
//----------------------------------------------------------------------
OBJ_VERTEX_BUF2 * VertexBufSet2( OBJ_VARIABLE *obj_dat )
{
    vec2    tex_d = { 0.0f, 0.0f };
    vec4    col_d = { 1.0f, 1.0f, 1.0f, 1.0f };
    vec3    nol_d = { 1.0f, 0.0f, 0.0f };
    vec3    ver_d = { 0.0f, 0.0f, 0.0f };

    int     loop0, loop1;
    int     dat_cnt;
    int     index_no;

    OBJ_POLY    *obj_pol;             // ポリゴンインデックス バッファ

    unsigned char *obj_buf;

    vec2        *obj_t0, *obj_t1;     // テクスチャバッファ
    vec4        *obj_c0, *obj_c1;     // カラーバッファ
    vec3        *obj_n0, *obj_n1;     // 法線バッファ
    vec3        *obj_v0, *obj_v1;     // 頂点バッファ

    // ポインタ初期化
    obj     = NULL;
    ver_buf = NULL;
    ver_buf2= NULL;

    // ポリゴンシークエンス バッファ
    ver_buf2 = new OBJ_VERTEX_BUF2;
    memset( ver_buf2, 0x00, sizeof(OBJ_VERTEX_BUF2) );

    dat_cnt = obj_dat->pol_cnt * 3;

    ver_buf2->pol_mode = GL_T2F_C4F_N3F_V3F;                    // データ種類
    ver_buf2->pol_cnt  = obj_dat->pol_cnt;                      // ポリゴン数
    ver_buf2->dat_cnt  = dat_cnt;                               // データ数（頂点数）
    ver_buf2->obj_size = sizeof(T2F_C4F_N3F_V3F) * dat_cnt;     // バッファサイズ

    // 頂点データ領域確保
    ver_buf2->obj_dat = new unsigned char[ver_buf2->obj_size];

    obj_buf = ver_buf2->obj_dat;// コピー先バッファ
    obj_t1  = obj_dat->obj_t;   // コピー元バッファ
    obj_c1  = obj_dat->obj_c;   // コピー元バッファ
    obj_n1  = obj_dat->obj_n;   // コピー元バッファ
    obj_v1  = obj_dat->obj_v;   // コピー元バッファ
    obj_pol = obj_dat->obj_pol; // ポリゴンデーター先頭アドレス

    if( obj_dat->tex_cnt > 0 )  ver_buf2->tex_f = TRUE; // テクスチャ座標フラグ
    if( obj_dat->col_cnt > 0 )  ver_buf2->col_f = TRUE; // カラーフラグ
    if( obj_dat->nol_cnt > 0 )  ver_buf2->nol_f = TRUE; // 法線フラグ
    if( obj_dat->ver_cnt > 0 )  ver_buf2->ver_f = TRUE; // 頂点座標フラグ


    // ポリゴンデータ書き込み
    for( loop0=0; loop0<obj_dat->pol_cnt; loop0++ )
    {

        // ポリゴン頂点データ書き込み
        for( loop1=0; loop1<3; loop1++ )
        {
            // テクスチャ座標データコピー
            obj_t0 = (vec2 *)obj_buf;               // コピー先バッファ
            if( ver_buf2->tex_f == TRUE )
            {
                index_no = obj_pol->t[loop1];       // インデックス番号取り出し
                *obj_t0 = *(obj_t1 + index_no);     // データコピ－
            }
            else
            {
                *obj_t0 = tex_d;                    // ダミーデータコピー
            }
            obj_buf += sizeof(vec2);                // バッファアドレス更新


            // カラーデータコピー
            obj_c0 = (vec4 *)obj_buf;               // コピー先バッファ
            if( ver_buf2->col_f == TRUE )
            {
                index_no = obj_pol->c[loop1];       // インデックス番号取り出し
                *obj_c0 = *(obj_c1 + index_no);     // ダミーデータコピー
            }
            else
            {
                *obj_c0 = col_d;                    // データコピ－
            }
            obj_buf += sizeof(vec4);                // バッファアドレス更新


            // 法線データコピー
            obj_n0 = (vec3 *)obj_buf;               // コピー先バッファ
            if( ver_buf2->nol_f == TRUE )
            {
                index_no = obj_pol->n[loop1];       // インデックス番号取り出し
                *obj_n0 = *(obj_n1 + index_no);     // データコピ－
            }
            else
            {
                *obj_n0 = nol_d;                    // ダミーデータコピー
            }
            obj_buf += sizeof(vec3);                // バッファアドレス更新


            // 頂点座標データコピー
            obj_v0 = (vec3 *)obj_buf;               // コピー先バッファ
            if( ver_buf2->ver_f == TRUE )
            {
                index_no = obj_pol->v[loop1];       // インデックス番号取り出し
                *obj_v0 = *(obj_v1 + index_no);     // データコピ－
            }
            else
            {
                *obj_v0 = ver_d;                     // ダミーデータコピー
            }
            obj_buf += sizeof(vec3);                // バッファアドレス更新
        }

        obj_pol++;
    }

    return ver_buf2;
}


//----------------------------------------------------------------------
//  データー開放処理 (OBJ_VARIABLE)
//----------------------------------------------------------------------
void ObjIndexFree( OBJ_VARIABLE *objp )
{
    if( objp != NULL )
    {
        if( objp->obj_v   != NULL )
        {
            delete[] objp->obj_v;
            objp->obj_v = NULL;
        }

        if( objp->obj_n   != NULL )
        {
            delete[] objp->obj_n;
            objp->obj_n = NULL;
        }

        if( objp->obj_c   != NULL )
        {
            delete[] objp->obj_c;
            objp->obj_c = NULL;
        }

        if( objp->obj_t   != NULL )
        {
            delete[] objp->obj_t;
            objp->obj_t = NULL;
        }

        if( objp->obj_pol != NULL )
        {
            delete[] objp->obj_pol;
            objp->obj_pol = NULL;
        }
        delete objp;
    }
}


//----------------------------------------------------------------------
//  データー開放処理 (OBJ_VERTEX_BUF)
//----------------------------------------------------------------------
void ObjVertexFree( OBJ_VERTEX_BUF *objp )
{
    if( objp != NULL )
    {
        if( objp->obj_v   != NULL )
        {
            delete[] objp->obj_v;
            objp->obj_v = NULL;
        }

        if( objp->obj_n   != NULL )
        {
            delete[] objp->obj_n;
            objp->obj_n = NULL;
        }

        if( objp->obj_c   != NULL )
        {
            delete[] objp->obj_c;
            objp->obj_c = NULL;
        }

        if( objp->obj_t   != NULL )
        {
            delete[] objp->obj_t;
            objp->obj_t = NULL;
        }
        delete objp;
    }
}


//----------------------------------------------------------------------
//  データー開放処理 (OBJ_VERTEX_BU2F)
//----------------------------------------------------------------------
void ObjVertexFree2( OBJ_VERTEX_BUF2 *objp )
{
    if( objp != NULL )
    {
        if( objp->obj_dat   != NULL )
        {
            delete[] objp->obj_dat;
            objp->obj_dat = NULL;
        }

        delete objp;
    }
}


//----------------------------------------------------------------------
//  エラー処理
//----------------------------------------------------------------------
int OBJ_ERREND1( void )
{

//    MessageBox( NULL, "OBJ ファイル読み込み失敗", "TGA", MB_OK );

    // ポリゴンデータ管理ポインタ初期化
    if( f_pol != NULL )
    {
        delete[] f_pol;
        f_pol = NULL;
    }

    ObjIndexFree( obj );
    obj = NULL;

    ObjVertexFree( ver_buf );
    ver_buf = NULL;

    ObjVertexFree2( ver_buf2 );
    ver_buf2 = NULL;

    return TRUE;
}


//----------------------------------------------------------------------
//  エラー処理
//----------------------------------------------------------------------
int OBJ_ERREND2( void )
{

//    MessageBox( NULL, "OBJ データ変換失敗", "TGA", MB_OK );

    // ポリゴンデータ管理ポインタ初期化
    ObjVertexFree( ver_buf );
    ver_buf = NULL;

    return TRUE;
}


//----------------------------------------------------------------------
//  エラー処理
//----------------------------------------------------------------------
int OBJ_ERREND3( void )
{

//    MessageBox( NULL, "OBJ データ変換失敗", "TGA", MB_OK );

    // ポリゴンデータ管理ポインタ初期化
    ObjVertexFree2( ver_buf2 );
    ver_buf2 = NULL;

    return TRUE;
}


//----------------------------------------------------------------------
//  POLデータ読み込み処理
//  filename : ファイル名
//----------------------------------------------------------------------
ModelData * ModelDataLoad(const char * filename)
{
    // モデルデータ管理
    ModelData* modelData = new ModelData();

    int buf_size;


    // POL データ読み込み
    obj = LoadPolFile(filename);
    if (obj == NULL)
    {
        return NULL;
    }

    // 頂点バッファ用データ変換
    ver_buf2 = VertexBufSet2(obj);
    if (ver_buf2 == NULL)
    {
        ObjIndexFree(obj);
        obj = NULL;
        return NULL;
    }

    // インデックス型領域開放
    ObjIndexFree(obj);
    obj = NULL;


    // モデルデータ領域確保
    modelData = new ModelData;
    if (modelData == NULL)
    {
        free(ver_buf2);
        ver_buf2 = NULL;
        return NULL;
    }

    // 頂点データ設定
    modelData->vertexObj = ver_buf2;

    // 頂点バッファ設定
    // バッファ管理番号取得
	glGenBuffers(1, &modelData->vertexNo);

    // 頂点バッファ作成
    // バッファ管理番号選択
	glBindBuffer(GL_ARRAY_BUFFER, modelData->vertexNo);
    // バッファサイズ代入
    buf_size = ver_buf2->obj_size;
    glBufferData(GL_ARRAY_BUFFER, buf_size,
        ver_buf2->obj_dat, GL_STATIC_DRAW);


    // 頂点配列 読み出しアドレス
    // バッファ管理番号選択
    glBindBuffer(GL_ARRAY_BUFFER, modelData->vertexNo);
    glInterleavedArrays(ver_buf2->pol_mode, 0, NULL);
    //    glInterleavedArrays( GL_C4F_N3F_V3F, 0, NULL );

    return modelData;
}


//----------------------------------------------------------------------
//  POLモデル表示処理
//  ModelData : モデルデータ
//----------------------------------------------------------------------
void ModelDataDisp(ModelData *model)
{

    // 頂点配列フラグ設定
    if (model->vertexObj->tex_f == TRUE)
    {
        glEnable(GL_TEXTURE_2D);						// テクスチャ貼り付け ON
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);	// テクスチャ座標データ使用
    }
    else
    {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY); // テクスチャ座標データ使用停止
    }

    if (model->vertexObj->col_f == TRUE)
    {
        glEnableClientState(GL_COLOR_ARRAY);         // カラーデータ使用
    }
    else
    {
        glDisableClientState(GL_COLOR_ARRAY);         // カラーデータ使用停止
    }

    if (model->vertexObj->nol_f == TRUE)
    {
        glEnableClientState(GL_NORMAL_ARRAY);        // 法線データ使用
    }
    else
    {
        glDisableClientState(GL_NORMAL_ARRAY);        // 法線データ使用停止
    }

    if (model->vertexObj->ver_f == TRUE)
    {
        glEnableClientState(GL_VERTEX_ARRAY);        // 頂点座標データ使用
    }
    else
    {
        glDisableClientState(GL_VERTEX_ARRAY);        // 頂点座標データ使用停止
    }


    // テクスチャ選択
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, model->textureNo);

    // 頂点配列 読み出しアドレス
    glBindBuffer(GL_ARRAY_BUFFER, model->vertexNo);     // バッファ管理番号選択
    glInterleavedArrays(model->vertexObj->pol_mode, 0, NULL);


    //	glDisableClientState(GL_COLOR_ARRAY);         // カラーデータ使用停止
    //	glDisableClientState(GL_NORMAL_ARRAY);        // 法線データ使用停止


    //	glBindTexture(GL_TEXTURE_2D, TexNo[0]);
    // ポリゴンの表示
    //	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    // ポリゴンの表示
    glDrawArrays(GL_TRIANGLES, 0, model->vertexObj->pol_cnt * 3);

}

