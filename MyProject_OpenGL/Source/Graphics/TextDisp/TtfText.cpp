//----------------------------------------------------------------------
//   TrueTypeFont 文字出力 プログラム                                   
//   Ver 2.0  : テクスチャを使用して描画                                
//----------------------------------------------------------------------
#define  _CRT_SECURE_NO_WARNINGS
#include "TtfSys.h"
#include "TtfText.h"
#include "TtfSub.h"


TTF_TEXT::TTF_TEXT()
{
    init_f = 0;

    dispSize = SIZE{ 0, 0 };

    dispOffsetMode = 0;

    dispOffset = SIZE{ 0, 0 };

    texNo = 0;
    imgSize = SIZE{ 0, 0 };;
    image_f = 0;
    image = nullptr;

    front_f = 0;
    front_rgba = TTF_RGBA{ 0,0,0,0 };

    padding = TTF_RECT{ 0,0,0,0 };

    back_f = 0;
    back_Tex_f = 0;
    back_TexNo = 0;
    back_rgba = TTF_RGBA{ 0,0,0,0 };

    scale = TTF_SCALE{ 0,0 };

    logFont_f = 0;
    logFont = LOGFONT{};
    hFont = 0;

    string[0] = NULL;

    Default();
}






//----------------------------------------------------------------------
// デフォルト値設定用
//----------------------------------------------------------------------
int TTF_TEXT::Default()
{

    // エラーチェック
    // ttfTextErrChk( L"ttfTextDefault" );

    if( init_f != 0 )
    {
        // 画像領域開放
        if( image != 0 )
        {
            free( image );
            image = NULL;
        }

        // テクスチャ管理番号開放
        if( texNo != 0 )
        {
            glDeleteTextures( 1, &texNo );
        }
    }

    memset( this, 0x00, sizeof(TTF_TEXT) );

    init_f         = TRUE;                 // 初期化フラグ ON

    scale.x        = TTF_TEXT_DEF_SCALE;   // 拡大縮小値
    scale.y        = TTF_TEXT_DEF_SCALE;   // 拡大縮小値

    dispSize.cx    = TTF_TEXT_DEF_WIDTH;   // ユーザー設定サイズ 横
    dispSize.cy    = TTF_TEXT_DEF_HEIGHT;  // ユーザー設定サイズ 縦

    dispOffsetMode = TTF_TEXT_DISP_CENTER; // 枠表示基準位置 中心

    back_f = FALSE;
    back_Tex_f  = FALSE;
    back_rgba.r = TTF_TEXT_DEF_BACK_C_R;
    back_rgba.g = TTF_TEXT_DEF_BACK_C_G;
    back_rgba.b = TTF_TEXT_DEF_BACK_C_B;
    back_rgba.a = TTF_TEXT_DEF_BACK_C_A;

    front_f = TRUE;
    front_rgba.r = TTF_TEXT_DEF_FRONT_C_R;
    front_rgba.g = TTF_TEXT_DEF_FRONT_C_G;
    front_rgba.b = TTF_TEXT_DEF_FRONT_C_B;
    front_rgba.a = TTF_TEXT_DEF_FRONT_C_A;

// 論理フォント初期化
    memset( &logFont, 0x00, sizeof(LOGFONT) );

    logFont.lfHeight         = TTF_TEXT_DEF_SIZE;                     // 文字の高さ
    logFont.lfWidth          = 0;                                     // 文字の幅
    logFont.lfEscapement     = 0;                                     // 文字送りの方向
    logFont.lfOrientation    = 0;                                     // 各文字のベースライン
    logFont.lfWeight         = FW_NORMAL;                             // 文字の太さ
    logFont.lfItalic         = 0;                                     // イタリック 0:なし 1:有り
    logFont.lfUnderline      = 0;                                     // 下線付け   0:なし 1:有り
    logFont.lfStrikeOut      = 0;                                     // 打ち消し線
    logFont.lfCharSet        = SHIFTJIS_CHARSET;                      // SHIFTJIS_CHARSET (シフトJIS文字セット)
    logFont.lfOutPrecision   = 0;                                     // 出力精度
    logFont.lfClipPrecision  = 0;                                     // クリッピング精度
    logFont.lfQuality        = 0;                                     // 出力品質
    //logFont.lfQuality        = ANTIALIASED_QUALITY;                   // 出力品質 1bit 画像を作成するので意味はない
    logFont.lfPitchAndFamily = 0;                                     // フォントのピッチとファミリ
    lstrcpy(logFont.lfFaceName, TTF_TEXT_DEF_NAME );


    return TRUE;
}

