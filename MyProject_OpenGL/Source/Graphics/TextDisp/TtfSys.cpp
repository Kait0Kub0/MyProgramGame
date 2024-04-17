//----------------------------------------------------------------------
//   TrueTypeFont 文字出力 プログラム                                   
//   Ver 2.0  : テクスチャを使用して描画                                
//----------------------------------------------------------------------
#define  _CRT_SECURE_NO_WARNINGS
#include "TtfSys.h"
#include "TtfText.h"
#include "TtfSub.h"

TTF_SYSTEM  TtfSys = { 0, 0 };


//----------------------------------------------------------------------
// フォントシステム初期化
//----------------------------------------------------------------------
int ttfTextSysInit(HWND* hwnd)
{
    assert(hwnd != NULL);
    assert(*hwnd != NULL);

    TtfSys.hWnd = hwnd;
    TtfSys.sys_Init_flag = 1;

    return TRUE;
}


//----------------------------------------------------------------------
// フォント初期化
//----------------------------------------------------------------------
int ttfTextClear(TTF_TEXT& ttfText)
{
    // エラーチェック
    ttfTextErrChk(L"ttfTextClear");

    if (TtfSys.sys_Init_flag == 0)
    {
        return FALSE;
    }

    // 論理フォント初期化
    if (ttfText.hFont != 0)
    {
        DeleteObject(ttfText.hFont);
        ttfText.hFont = 0;
    }

    // 文字画像領域の開放
    if (ttfText.image != 0)
    {
        free(ttfText.image);
        ttfText.image = 0;
    }

    memset(&ttfText, 0x00, sizeof(TTF_TEXT));

    ttfText.Default();

    return TRUE;
}


//----------------------------------------------------------------------
// フォント画像の作成用
//----------------------------------------------------------------------
void createStringBitmapText(HDC hDC, TTF_TEXT& ttfText)
{

    typedef struct {
        BITMAPINFOHEADER    bmiHeader;
        // 1 ビットマップはモノクロ
        // bmiColors メンバは 2 つのエントリが必要
        // ビットマップ配列の各ビットが 1 つのピクセルを表す
        // ビットがクリアされていると ピクセルは bmiColors テーブルの最初のエントリの色で表示
        // ビットがセットされていると ピクセルは bmiColors テーブルの 2 番目のエントリの色で表示
        RGBQUAD             bmiColors[2];
    } BITMAPINFO_MONO;

    typedef struct {
        BITMAPINFOHEADER    bmiHeader;
        RGBQUAD             bmiColors[256];
    } BITMAPINFO_256;


    HDC                 hMemDC;
    HBITMAP             hbmp = 0;
    BITMAP              bmp;

    HGDIOBJ             hFontBackup;
    HGDIOBJ             hBmpBackup;

    //    BITMAPINFO_MONO     bitinfo_mono;
    BITMAPINFO_256      bitinfo256;
    BITMAPINFO* bitinfo;

    int                 pixsize;
    int                 len;

    RECT                ImgRect;


    // メモリデバイスコンテキストを作成
    hMemDC = CreateCompatibleDC(hDC);

    if (hMemDC)
    {
        // メモリデバイスコンテキストのフォント設定をバックアップ
        hFontBackup = SelectObject(hMemDC, ttfText.hFont);

        // 文字数
        len = (int)wcslen(ttfText.string);
        // 文字列を描画するサイズセット
        ttfText.imgSize = ttfText.dispSize;

        // 画像サイズ計算
        SIZE image;
        image.cx = ttfText.imgSize.cx - (ttfText.padding.left + ttfText.padding.right);
        image.cy = ttfText.imgSize.cy - (ttfText.padding.top + ttfText.padding.bottom);


        // ビットマップ画像データ領域の確保
        memset(&bmp, 0x00, sizeof(BITMAP));
        //1dot1bit
        bmp.bmType = 0;                                            // ビットマップタイプ
        bmp.bmWidth = image.cx;                                     // 横幅
        bmp.bmHeight = image.cy;                                     // 縦幅
        bmp.bmWidthBytes = ((image.cx + 7) / 8 + 1) & (~1);              // ドットサイズから偶数バイトに変換
        bmp.bmPlanes = 1;                                            // ビットマップ画像のプレーン数
        bmp.bmBitsPixel = 1;                                            // 1ピクセルに必要なビット数
        bmp.bmBits = calloc(bmp.bmWidthBytes * bmp.bmHeight, sizeof(char));
        //1dot32bit
                //bmp.bmType          = 0;                                             // ビットマップタイプ
                //bmp.bmWidth         = ttfText.imgSize.cx;                           // 横幅
                //bmp.bmHeight        = ttfText.imgSize.cy;                           // 縦幅
                //bmp.bmWidthBytes    = ttfText.imgSize.cx * 4;                       // ドットサイズから偶数バイトに変換
                //bmp.bmPlanes        = 1;                                             // ビットマップ画像のプレーン数
                //bmp.bmBitsPixel     = 32;                                            // 1ピクセルに必要なビット数
                //bmp.bmBits          = calloc( bmp.bmWidthBytes * bmp.bmWidth * bmp.bmHeight, sizeof(char) );

        if (bmp.bmBits)
        {
            // 幅、高さ、色形式（カラープレーンと 1 ピクセル当たりの色数）を持つ
            // ビットマップを作成
            hbmp = CreateBitmapIndirect(&bmp);

            // メモリーの解放
            free(bmp.bmBits);
            bmp.bmBits = 0;
        }

        if (hbmp)
        {
            // メモリデバイスコンテキストのオブジェクトを選択
            hBmpBackup = SelectObject(hMemDC, hbmp);

            // 現在の背景色を、指定された色の値に設定
            SetBkColor(hMemDC, RGB(0, 0, 0));
            // デバイスコンテキストのテキストの色を指定された色に設定
            SetTextColor(hMemDC, RGB(255, 255, 255));
            // デバイスコンテキストの背景モードを設定
            // OPAQUE :: テキスト、ハッチブラシ、ペンのいずれかの描画を行う前に、
            //           現在の背景色で背景を塗りつぶす
            SetBkMode(hMemDC, OPAQUE);

            // 文字の表示領域設定
            ImgRect.left = 0;
            ImgRect.top = 0;
            ImgRect.right = image.cx;
            ImgRect.bottom = image.cy;

            //文字間隔
            //SetTextCharacterExtra(hMemDC,10);

            // 現在選択されているフォント、背景色、および文字の色を使用して文字列を描画
            DrawTextEx(hMemDC, ttfText.string, len, &ImgRect, DT_LEFT | DT_TOP | DT_NOCLIP | DT_WORDBREAK, NULL);

            // メモリーの解放
            if (ttfText.image != 0)
            {
                free(ttfText.image);
                ttfText.image = 0;
            }

            // ビットマップ画像データのバイト数計算
            pixsize = bmp.bmHeight * bmp.bmWidth * bmp.bmWidthBytes;
            ttfText.image = (unsigned char*)calloc(pixsize, sizeof(char));

            // ビットマップヘッダポインタに領域設定
            bitinfo = (BITMAPINFO*)&bitinfo256;

            // ビットマップヘッダの初期化
            bitinfo->bmiHeader.biSize = sizeof(bitinfo->bmiHeader);
            bitinfo->bmiHeader.biWidth = bmp.bmWidth;
            bitinfo->bmiHeader.biHeight = bmp.bmHeight;
            bitinfo->bmiHeader.biPlanes = 1;
            bitinfo->bmiHeader.biBitCount = 8;
            bitinfo->bmiHeader.biCompression = BI_RGB;
            bitinfo->bmiHeader.biSizeImage = pixsize;
            bitinfo->bmiHeader.biXPelsPerMeter = 1;
            bitinfo->bmiHeader.biYPelsPerMeter = 1;
            bitinfo->bmiHeader.biClrUsed = 0;
            bitinfo->bmiHeader.biClrImportant = 0;


            // ビットマップのビットを取得し、指定された形式でバッファへコピー
            GetDIBits(hMemDC, hbmp, 0, bmp.bmHeight, ttfText.image, bitinfo, DIB_RGB_COLORS);

            // フォント画像サイズ設定
            ttfText.imgSize.cx = bmp.bmWidth;
            ttfText.imgSize.cy = bmp.bmHeight;

            // メモリデバイスコンテキストのオブジェクトを戻す
            SelectObject(hMemDC, hBmpBackup);

            // ビットマップオブジェクトを削除
            DeleteObject(hbmp);
        }

        SelectObject(hMemDC, hFontBackup);

        // デバイスコンテキストを削除
        DeleteDC(hMemDC);


        // テクスチャ画像作成
        if (ttfText.texNo == 0)
        {
            glGenTextures(1, &ttfText.texNo);
        }

        // VRAM 転送
        glBindTexture(GL_TEXTURE_2D, ttfText.texNo);

        //メインメモリからVRAMに画像転送
        glTexImage2D(GL_TEXTURE_2D,            // 2Dテクスチャ使用
            0,                        // ミプマップレベル
            GL_ALPHA,                  // テクスチャカラー要素数
            ttfText.imgSize.cx,      // テクスチャ画像の幅   ※絵の大きさは２のｎ乗
            ttfText.imgSize.cy,      // テクスチャ画像の高さ ※絵の大きさは２のｎ乗
            0,                        // 境界線の幅を指定 [0:1](ドット)
            GL_ALPHA,                 // 画像データのフォーマット
            GL_UNSIGNED_BYTE,         // 画像データのデータ形式
            ttfText.image);         // メインメモリ上の画像データのアドレス

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    }
}



//==========================================================================
// ２のｎ乗チェック処理
//==========================================================================
SIZE checkPow2n(SIZE size)
{
    int     pow2n;
    SIZE    setSize = { TTF_TEXT_DEF_WIDTH, TTF_TEXT_DEF_HEIGHT };

    // 横幅チェック
    pow2n = 16;
    while (pow2n < POW2nMAX)
    {
        if (size.cx <= pow2n)
        {
            setSize.cx = pow2n;
            break;
        }
        pow2n = pow2n * 2;
    }

    // 縦幅チェック
    pow2n = 16;
    while (pow2n < POW2nMAX)
    {
        if (size.cy <= pow2n)
        {
            setSize.cy = pow2n;
            break;
        }
        pow2n = pow2n * 2;
    }

    return setSize;

}




//----------------------------------------------------------------------
// 外部フォント読み込み
//----------------------------------------------------------------------
void ttfTextFontLoad(const wchar_t* fontPath)
{
    // 外部フォント読み込み
    AddFontResourceEx(fontPath, FR_PRIVATE, NULL);

}


//----------------------------------------------------------------------
// エラーチェック関数
//----------------------------------------------------------------------
int ttfTextErrChk(LPCWSTR str)
{
    int ret = 0;

    // システムの初期化しないといけません。
    assert(TtfSys.sys_Init_flag != 0);

    if (TtfSys.sys_Init_flag == 0)
    {
        // エラーメッセージ
        ret = MessageBox(NULL,
            L"ttfTextSysInitを実行してから使用してください。",
            str,
            MB_ABORTRETRYIGNORE
        );
        if (ret == IDABORT)
        {
            exit(0);
        }
    }

    return ret;
}
