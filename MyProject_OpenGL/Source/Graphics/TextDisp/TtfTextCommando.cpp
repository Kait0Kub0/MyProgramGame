//----------------------------------------------------------------------
//   TrueTypeFont 文字出力 プログラム                                   
//   Ver 2.0  : テクスチャを使用して描画                                
//----------------------------------------------------------------------
#define  _CRT_SECURE_NO_WARNINGS
#include "TtfSys.h"
#include "TtfText.h"
#include "TtfSub.h"


//----------------------------------------------------------------------
// フォント設定変更用
//----------------------------------------------------------------------
// 配列設定
void TTF_TEXT::Parameterv(int setcom, void* vp)
{
    LOGFONT     workFont;

    // エラーチェック
    ttfTextErrChk(L"ttfTextParameter");

    workFont = logFont;

    // 初期化されていないときの処理
    if (init_f == 0)
    {
        Default();                                   // フォント初期化
    }

    // コマンド処理
    switch (setcom)
    {
        // 初期化系処理-------------------------------------------------------------
    case TTF_TEXT_DEF:                                      // デフォルト設定
        Default();                                          // フォント初期化
        workFont = logFont;
        break;
    case TTF_TEXT_FONT:                                     // 論理フォント作成
        LogicFont();
        workFont = logFont;
        break;

        // 設定変更系処理-------------------------------------------------------------
    case TTF_TEXT_ALL:                                      // 全データ書き換え
        *this = *((TTF_TEXT*)vp);
        break;
    case TTF_TEXT_DISP_SIZE:
        dispSize = *((SIZE*)vp);
        image_f = 0;   // 画像再作成
        break;
    case TTF_TEXT_SCALE:
        scale = *((TTF_SCALE*)vp);
        break;
    case TTF_TEXT_BACK_COLOR:                               // 背景色設定
        back_rgba = *((TTF_RGBA*)vp);
        break;
    case TTF_TEXT_FRONT_COLOR:                              // 文字色設定
        front_rgba = *((TTF_RGBA*)vp);
        break;

    case TTF_TEXT_PADDING:                                  // 隙間設定
        padding = *((TTF_RECT*)vp);
        break;

    case TTF_TEXT_DISP_SIZE_X:
        memcpy((void*)&dispSize.cx, vp, sizeof(LONG));
        image_f = 0;   // 画像再作成
        break;
    case TTF_TEXT_DISP_SIZE_Y:
        memcpy((void*)&dispSize.cy, vp, sizeof(LONG));
        image_f = 0;   // 画像再作成
        break;

    case TTF_TEXT_NAME:                                     // フォントの名前設定
        lstrcpy(logFont.lfFaceName, (LPCWSTR)(vp));
        break;

    default:
        break;
    }

    // フォント設定に変更があったら作成フラグを変更
    if (memcmp(&logFont, &workFont, sizeof(LOGFONT)) != 0)
    {
        logFont_f = 0;      // 論理フォント未作成
        image_f = 0;        // 文字画像未作成
    }
}


// 単体設定(int)
void TTF_TEXT::Parameteri(int setcom, int vp)
{
    LOGFONT     lFont;

    // エラーチェック
    ttfTextErrChk(L"ttfTextParameteri");

    lFont = logFont;

    // 初期化されていないときの処理
    if (init_f == 0)
    {
        Default();                                          // フォント初期化
    }

    // コマンド処理
    switch (setcom)
    {
        // 初期化系処理-------------------------------------------------------------
    case TTF_TEXT_DEF:                                      // デフォルト設定
        Default();                                          // フォント初期化
        lFont = logFont;
        break;
    case TTF_TEXT_FONT:                                     // 論理フォント作成
        LogicFont();
        lFont = logFont;
        break;


    case TTF_TEXT_DISP_SIZE_X:
        dispSize.cx = (LONG)vp;
        image_f = 0;   // 画像再作成
        break;
    case TTF_TEXT_DISP_SIZE_Y:
        dispSize.cy = (LONG)vp;
        image_f = 0;   // 画像再作成
        break;
    case TTF_TEXT_BACK:                                     // 背景の ON/OFF 設定
        back_f = (BYTE)(vp);
        break;
    case TTF_TEXT_FRONT:                                    // 文字の ON/OFF 設定
        front_f = (BYTE)(vp);
        break;


        // フォント設定変更系処理---------------------------------------------------
    case TTF_TEXT_HEIGHT:                                   // フォント高さ設定
        logFont.lfHeight = (LONG)(vp);
        break;
    case TTF_TEXT_WIDTH:                                    // フォント幅設定
        logFont.lfWidth = (LONG)(vp);
        break;
    case TTF_TEXT_WEIGHT:                                   // フォントの太さ設定
        logFont.lfWeight = (LONG)(vp);
        break;
    case TTF_TEXT_ITALIC:                                   // フォントのイタリック体設定
        logFont.lfItalic = (BYTE)(vp);
        break;
    case TTF_TEXT_UNDERLINE:                                // フォントの下線設定
        logFont.lfUnderline = (BYTE)(vp);
        break;
    case TTF_TEXT_STRIKEOUT:                                // フォントの訂正線
        logFont.lfStrikeOut = (BYTE)(vp);
        break;

    case TTF_TEXT_BACK_TEXTURE:                             // 背景テクスチャセット
        back_TexNo = (GLuint)(vp);
        break;

    default:
        break;
    }

    // フォント設定に変更があったら作成フラグを変更
    if (memcmp(&logFont, &lFont, sizeof(LOGFONT)) != 0)
    {
        logFont_f = 0;      // 論理フォント未作成
        image_f = 0;        // 文字画像未作成
    }
}





//----------------------------------------------------------------------
// フォント名設定処理
//----------------------------------------------------------------------
void TTF_TEXT::FontName(const wchar_t* fontName)
{
    Parameterv(TTF_TEXT_NAME, (void*)fontName);
}



//----------------------------------------------------------------------
// 文字タイプ（イタリック・下線・打ち消し線）処理
//----------------------------------------------------------------------
void TTF_TEXT::FontType(int setcom, bool flag)
{
    if (flag == false)
    {
        Parameteri(setcom, FALSE);
    }
    else
    {
        Parameteri(setcom, TRUE);
    }
}



//----------------------------------------------------------------------
// 文字サイズ設定
//----------------------------------------------------------------------
void TTF_TEXT::FontSize(int size, int weight)
{
    // 文字サイズ
    Parameteri(TTF_TEXT_HEIGHT, size);

    // 文字の太さ
    if (weight >= 0)
    {
        Parameteri(TTF_TEXT_WEIGHT, weight);
    }
}



//----------------------------------------------------------------------
// 文字色変更処理
//----------------------------------------------------------------------
void TTF_TEXT::FrontColor(float r, float g, float b, float a)
{
    front_rgba.r = r;
    front_rgba.g = g;
    front_rgba.b = b;
    front_rgba.a = a;
}


//----------------------------------------------------------------------
// 表示領域設定（left:左の隙間・right:右の隙間・top:上の隙間・bottom:下の隙間）
//----------------------------------------------------------------------
void TTF_TEXT::Padding(int left, int right, int top, int bottom)
{
    TTF_RECT rect = { left ,right, top, bottom };
    Parameterv(TTF_TEXT_PADDING, &rect);
}


//----------------------------------------------------------------------
// 論理フォント作成処理
//----------------------------------------------------------------------
void TTF_TEXT::LogicFont()
{
    HDC     hDC;                  // デバイスコンテキスト

    // エラーチェック
    ttfTextErrChk(L"ttfTextFont");

    // フォントデータ設定
    logFont_f = 1;

    // 論理フォント削除
    if (hFont != 0)
    {
        DeleteObject(hFont);
        hFont = NULL;
    }

    // 論理フォントの作成
    hFont = CreateFontIndirect(&logFont);

    // 文字が設定されている時
    if (string[0] != 0)
    {
        hDC = GetDC(*TtfSys.hWnd);       // ディスプレイデバイスコンテキストのハンドルを取得

        //ビットマップフォントの作成
        createStringBitmapText(hDC, *this);
        image_f = 1;

        // デバイスの開放
        ReleaseDC(*TtfSys.hWnd, hDC);
    }
}



//----------------------------------------------------------------------
// 背景サイズ設定（横幅・縦幅）
//----------------------------------------------------------------------
void TTF_TEXT::BackSize(int sizeW, int sizeH)
{
    dispSize.cx = sizeW;
    dispSize.cy = sizeH;
}


//----------------------------------------------------------------------
// 基準点
//----------------------------------------------------------------------
void TTF_TEXT::BackPoint(int mode, int posX, int posY)
{
    // モードセット
    dispOffsetMode = mode;


    switch (mode)
    {
    case TTF_TEXT_DISP_OFFSET:
        dispOffset.cx = posX;
        dispOffset.cy = posY;
        break;
    case TTF_TEXT_DISP_CENTER:
    case TTF_TEXT_DISP_TOP_LEFT:
    case TTF_TEXT_DISP_TOP_RIGHT:
    case TTF_TEXT_DISP_BOTTOM_LEFT:
    case TTF_TEXT_DISP_BOTTOM_RIGHT:
        break;
    default:
        dispOffsetMode = TTF_TEXT_DISP_CENTER;
        break;
    }

}



//----------------------------------------------------------------------
// 背景テクスチャ(texNo:テクスチャ番号・flag:有効無効)
//----------------------------------------------------------------------
void TTF_TEXT::BackTexture(bool flag, GLuint texNo)
{
    if (texNo > 0)
    {
        back_TexNo = texNo;
    }

    back_Tex_f = flag;

}



//----------------------------------------------------------------------
// 背景色変更処理
//----------------------------------------------------------------------
void TTF_TEXT::BackColor(float r, float g, float b, float a)
{
    back_rgba.r = r;
    back_rgba.g = g;
    back_rgba.b = b;
    back_rgba.a = a;
}






//----------------------------------------------------------------------
// 文字列設定用関数
//----------------------------------------------------------------------
void TTF_TEXT::String(const wchar_t* fmt, ...)
{
    HDC                 hDC;                        // デバイスコンテキスト
    const size_t        strMax = TTF_MAX_STRING;
    wchar_t             text[strMax + 1] = {0};     // 文字バッファ
    va_list             ap;                         // リストポインタ


    // エラーチェック
    ttfTextErrChk(L"ttfTextSetStr");

    // 初期化されていないときの処理
    if (init_f == 0)
    {
        Default();                                      // フォント初期化
    }

    // 書式設定されていたら
    if (fmt != NULL)
    {
        va_start(ap, fmt);                              // 引数の構文解析
        _vsnwprintf(text, strMax, fmt, ap);             // シンボルを実際の数に変換
        va_end(ap);                                     // 結果はテキストに設定される
        text[strMax] = 0x00;                            // 文字列終了コードセット
    }

    // 論理フォントが作成されていないとき
    if (logFont_f == 0)
    {
        memset(string, 0x00, sizeof(string));
        memcpy(string, text, sizeof(wchar_t) * wcslen(text));

        Parameteri(TTF_TEXT_FONT, 0);     // 論理フォントの作成
    }
    else
    {
        // 文字列の変更チェック
        int str1 = (int)wcslen(text) * sizeof(wchar_t);
        int str2 = (int)wcslen(string) * sizeof(wchar_t);
        if (str1 == str2)
        {
            if (memcmp(text, string, str1) == 0)
            {
                return;
            }
        }

        // 入力された文字列のセット
        memset(string, 0x00, sizeof(string));
        memcpy(string, text, sizeof(wchar_t) * wcslen(text));

        hDC = GetDC(*TtfSys.hWnd);         // ディスプレイデバイスコンテキストのハンドルを取得

        //ビットマップフォントの作成
        createStringBitmapText(hDC, *this);
        image_f = 1;

        // デバイスの開放
        ReleaseDC(*TtfSys.hWnd, hDC);
    }

    return;
}


//----------------------------------------------------------------------
// 繰り返し表示用関数
//----------------------------------------------------------------------
void TTF_TEXT::Print(float kx, float ky)
{
    float w, h;

    float p0x, p0y;
    float p1x, p1y;
    float t0x, t0y;
    float t1x, t1y;

    GLint viewport[4];          //ビューポート

    // シェーダー解除
    glUseProgram(NULL);
    for (int i = 0; i < 8; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);   // マルチテクスチャ番号選択
        glBindTexture(GL_TEXTURE_2D, 0);    // テクスチャ OFF
    }

    // エラーチェック
    ttfTextErrChk(L"ttfTextPrint");

    // 文字データが登録されていない
    if (string[0] == 0) return;

    // 文字画像が作られていない
    if (image_f == 0)
    {
        String(string);
    }


    // 環境保存
    glPushAttrib(GL_ENABLE_BIT);

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // マトリックス環境の設定
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glGetIntegerv(GL_VIEWPORT, viewport);       //ビューポート取得
    w = (float)viewport[2] / 2.0f;
    h = (float)viewport[3] / 2.0f;
    glOrtho(-w, w, -h, h, -1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // テクスチャ座標計算
    float workTx;
    workTx = (float)padding.left / dispSize.cx;
    t0x = -(workTx * 2);

    workTx = (float)padding.right / dispSize.cx;
    t1x = 1.0f + (workTx * 2);

    float workTy;
    workTy = (float)padding.bottom / dispSize.cy;
    t0y = -(workTy * 2);

    workTy = (float)padding.top / dispSize.cy;
    t1y = 1.0f + (workTy * 2);



    // モード別設定
    switch (dispOffsetMode)
    {
    case TTF_TEXT_DISP_OFFSET:
        p0x = ((float)dispSize.cx / 2);
        p0y = ((float)dispSize.cy / 2);
        p1x = -p0x;
        p1y = -p0y;
        p0x += dispOffset.cx;
        p0y += dispOffset.cy;
        p1x += dispOffset.cx;
        p1y += dispOffset.cy;
        break;
    case TTF_TEXT_DISP_TOP_LEFT:
        p0x = (float)dispSize.cx;
        p0y = 0;
        p1x = 0;
        p1y = -(float)dispSize.cy;
        break;
    case TTF_TEXT_DISP_TOP_RIGHT:
        p0x = 0;
        p0y = 0;
        p1x = -(float)dispSize.cx;
        p1y = -(float)dispSize.cy;

        break;
    case TTF_TEXT_DISP_BOTTOM_LEFT:
        p0x = (float)dispSize.cx;
        p0y = (float)dispSize.cy;
        p1x = 0;
        p1y = 0;
        break;
    case TTF_TEXT_DISP_BOTTOM_RIGHT:
        p0x = 0;
        p0y = (float)dispSize.cy;
        p1x = -(float)dispSize.cx;
        p1y = 0;
        break;
    case TTF_TEXT_DISP_CENTER:
    default:
        // デフォルト
        p0x = (float)dispSize.cx / 2;
        p0y = (float)dispSize.cy / 2;
        p1x = -p0x;
        p1y = -p0y;
        break;
    }


    glTranslatef(kx, ky, 0.0);

    glScalef(scale.x, scale.y, 1.0f);

    // 背景表示
    if (back_f == TRUE)
    {

        // アルファ値の比較書き込み
        glDisable(GL_ALPHA_TEST);
        // アルファ値が 0.5 以外のデータを書き込む
        //glAlphaFunc(GL_GEQUAL, 0.01);

        glDisable(GL_TEXTURE_2D);

        if (back_Tex_f == TRUE && back_TexNo > 0)
        {
            glEnable(GL_TEXTURE_2D);
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        }

        glActiveTexture(GL_TEXTURE0);   // マルチテクスチャのテクスチャ番号
        glBindTexture(GL_TEXTURE_2D, back_TexNo);

        glColor4fv((float*)&back_rgba);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);    glVertex2f(p1x, p1y);
        glTexCoord2f(1.0f, 0.0f);    glVertex2f(p0x, p1y);
        glTexCoord2f(1.0f, 1.0f);    glVertex2f(p0x, p0y);
        glTexCoord2f(0.0f, 1.0f);    glVertex2f(p1x, p0y);
        glEnd();
    }

    // 文字表示
    if (front_f == TRUE)
    {
        // テクスチャー描画
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);   // マルチテクスチャ番号選択
        glBindTexture(GL_TEXTURE_2D, texNo);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        glColor4fv((float*)&front_rgba);
        // 頂点データ登録
        glBegin(GL_QUADS);
        glTexCoord2f(t0x, t0y);    glVertex2f(p1x, p1y);
        glTexCoord2f(t1x, t0y);    glVertex2f(p0x, p1y);
        glTexCoord2f(t1x, t1y);    glVertex2f(p0x, p0y);
        glTexCoord2f(t0x, t1y);    glVertex2f(p1x, p0y);
        glEnd();
    }

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);

    // 環境復帰
    glPopAttrib();

}
