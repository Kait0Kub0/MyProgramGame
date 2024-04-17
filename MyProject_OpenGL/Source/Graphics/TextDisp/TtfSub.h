//----------------------------------------------------------------------
//   TrueTypeFont 文字出力 プログラム                                   
//   Ver 2.0  : テクスチャを使用して描画                                
//----------------------------------------------------------------------
#pragma once



// フォント設定初期化
extern  int ttfTextClear( TTF_TEXT &ttfText );

// 論理フォント作成処理
extern  void ttfTextFont( TTF_TEXT &ttfText, LOGFONT lFont );

// ２のｎ乗チェック処理
extern  SIZE checkPow2n( SIZE size );

// エラーチェック関数
extern  int ttfTextErrChk(LPCWSTR str );

// フォント画像の作成用
extern  void createStringBitmapText( HDC hDC, TTF_TEXT &ttfText );
