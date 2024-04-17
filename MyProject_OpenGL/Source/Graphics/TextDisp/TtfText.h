//----------------------------------------------------------------------
//   TrueTypeFont 文字出力 プログラム                                   
//   Ver 2.0  : テクスチャを使用して描画                                
//----------------------------------------------------------------------
#pragma once

#define TTF_MAX_STRING  256                     // 一度に表示できる最大文字数


//-----------------------------------------------------------------------------
// フォントデータ設定変更用
// Parameterv・Parameteri 関数で使用するコマンド
enum
{
    TTF_TEXT_DEF,               // デフォルト設定       v:i
    TTF_TEXT_FONT,              // 論理フォント作成     v:i

    TTF_TEXT_ALL,               // 全データ書き換え     v:

    TTF_TEXT_DISP_SIZE,         // サイズ指定           v:
    TTF_TEXT_DISP_SIZE_X,       // サイズ指定 横幅       :i
    TTF_TEXT_DISP_SIZE_Y,       // サイズ指定 縦幅       :i

    TTF_TEXT_DISP_OFFSET,       // 枠の表示位置設定
    TTF_TEXT_DISP_CENTER,       // 枠の中心
    TTF_TEXT_DISP_TOP_LEFT,     // 枠の左上
    TTF_TEXT_DISP_TOP_RIGHT,    // 枠の右上
    TTF_TEXT_DISP_BOTTOM_LEFT,  // 枠の左下
    TTF_TEXT_DISP_BOTTOM_RIGHT, // 枠の右下

    TTF_TEXT_SCALE,             // 拡大縮小             v:
    TTF_TEXT_SCALE_x,           // 拡大縮小 X            :i
    TTF_TEXT_SCALE_y,           // 拡大縮小 Y            :i

    TTF_TEXT_BACK,              // 背景設定              :i
    TTF_TEXT_BACK_COLOR,        // 背景色設定           v:
    TTF_TEXT_BACK_TEXTURE,      // 背景テクスチャ        :i

    TTF_TEXT_FRONT,             // 文字設定              :i
    TTF_TEXT_FRONT_COLOR,       // 文字色設定           v:

    TTF_TEXT_PADDING,           // 枠と文字の隙間       v:

    TTF_TEXT_HEIGHT,            // 文字高さ設定          :i
    TTF_TEXT_WIDTH,             // 文字幅設定            :i
    TTF_TEXT_WEIGHT,            // 文字の太さ設定        :i
    TTF_TEXT_ITALIC,            // イタリック文字設定    :i
    TTF_TEXT_UNDERLINE,         // 下線設定              :i
    TTF_TEXT_STRIKEOUT,         // 訂正線設定            :i
    TTF_TEXT_NAME,              // フォント名設定       v:
};

//-----------------------------------------------------------------------------
// 構造体設定
typedef struct
{
    float x, y;
} TTF_SCALE;

typedef struct {
    float r, g, b, a;
} TTF_RGBA;

typedef struct {
    int left, right, top, bottom;
} TTF_RECT;


// フォント表示用構造体
class TTF_TEXT {
public:
    // コンストラクタ
    TTF_TEXT();

    int             init_f;                     // 初期化フラグ

    SIZE            dispSize;                   // ユーザー設定表示サイズ

    int             dispOffsetMode;             // モード設定   TTF_TEXT_DISP_OFFSET        オリジナルポジション
                                                //              TTF_TEXT_DISP_CENTER        中心
                                                //              TTF_TEXT_DISP_TOP_LEFT・TTF_TEXT_DISP_TOP_RIGHT
                                                //              TTF_TEXT_DISP_BOTTOM_LEFT・TTF_TEXT_DISP_BOTTOM_RIGHT
    SIZE            dispOffset;                 // 表示位置補正(デフォルト枠の中心)

    GLuint          texNo;                      // テクスチャ管理番号
    SIZE            imgSize;                    // 画像のサイズ
    int             image_f;                    // 文字画像作成フラグ      0:未作成  1:作成済み
    unsigned char*  image;                      // 画像データ（文字を絵に変換したデータ）

    int             front_f;                    // 文字表示フラグ
    TTF_RGBA        front_rgba;                 // 文字色

    TTF_RECT        padding;                    // 枠との隙間　左・右・上・下

    int             back_f;                     // 背景表示フラグ
    int             back_Tex_f;                 // 背景テクスチャフラグ
    GLuint          back_TexNo;                 // 背景用テクスチャ管理番号
    TTF_RGBA        back_rgba;                  // 背景色

    TTF_SCALE       scale;                      // 拡大縮小値

    int             logFont_f;                  // 論理フォント作成フラグ  0:未作成  1:作成済み
    LOGFONT         logFont;                    // フォントの書式
    HFONT           hFont;                      // 論理フォント

    wchar_t         string[TTF_MAX_STRING + 1]; // 表示文字（テキスト）


    // デフォルト値設定用
    int Default();

    //=========== 汎用 =========== 

    // フォントデータ設定変更用
    void Parameterv(int setcom, void* vp);
    void Parameteri(int setcom, int   vp);


    //=========== 文字関連 =========== 

    // フォント名設定
    void FontName(const wchar_t* fontName);
    // 文字タイプ（イタリック・下線・打ち消し線）
    void FontType(int setcom, bool flag);
    // 文字サイズ設定
    void FontSize(int size, int  weight = -1);
    // 文字色変更処理
    void FrontColor(float r, float g, float b, float a);
    // 表示領域設定（left:左の隙間・right:右の隙間・top:上の隙間・bottom:下の隙間）
    void Padding(int left, int right, int top, int bottom);
    // 文字列設定用関数
    void String(const wchar_t* fmt, ...);


    //=========== 背景関連 =========== 

    // 背景サイズ設定（横幅・縦幅）
    void BackSize(int sizeW, int sizeH);
    // 基準点
    void BackPoint(int mode, int posX = 0, int posY = 0);
    // 背景テクスチャ(texNo:テクスチャ番号・flag:有効無効)
    void BackTexture(bool flag, GLuint texNo = 0);
    // 背景色変更処理
    void BackColor(float r, float g, float b, float a);



    //=========== 描画関連 =========== 


    // 表示用関数
    void Print(float kx, float ky);


private:
    // 論理フォント作成
    void LogicFont();

};




//-----------------------------------------------------------------------------
// プロトタイプ宣言


//=========== 初期化関連 =========== 

// フォントシステム初期化
extern int ttfTextSysInit(HWND* hwnd);
// フォント初期化
extern int ttfTextClear(TTF_TEXT& ttfText);


//=========== 外部フォント関連 =========== 

// 外部フォント読み込み
extern void ttfTextFontLoad(const wchar_t* fontPath);
