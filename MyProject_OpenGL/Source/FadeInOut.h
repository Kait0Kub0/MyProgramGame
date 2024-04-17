#pragma once

#include "Base.h"
#include "ThreadManager.h"

enum class FadeMode
{
    Off,
    FadeIn,
    FadeInTex,
    FadeOut,
    FadeOutTex,
    Max
};



class FadeInOut : public Base, public ThreadData
{
private:
    // テクスチャ番号
    int texNo;

    // カラーデータ
    vec4 color;

    // テクスチャ座標 offset
    float offsetNo0;
    float offsetNo1;

    // スピード
    float speed;

    // モード
    FadeMode mode = FadeMode::Off;

public:

    // コンストラクタ
    FadeInOut();

    // セッター
    void SetColor(vec4 color);
    void SetShader(int sNo);
    void SetTextureNo(int tNo);
    void SetSpeed(float speed);
    // ゲッター
    FadeMode GetMode();

    // 初期化
    void Init(int sNo, int tNo = -1);
    void Init(FadeMode mode);   // ループ時の初期化
    void InitIn();      // アルファ値フェードイン
    void InitInTex();   // テクスチャ座標フェードイン
    void InitOut();     // アルファ値フェードアウト
    void InitOutTex();  // テクスチャ座標フェードアウト

    // 動作処理
    bool FadeMove();
    // スレッド型
    static unsigned ThreadFadeMove(void* p);

    // フェードIN
    bool FadeIn();
    bool FadeInTex();

    // フェードOUT
    bool FadeOut();
    bool FadeOutTex();

    // 表示処理
    void Disp();
};


// グローバル変数
extern FadeInOut* fadeInOutObj;
