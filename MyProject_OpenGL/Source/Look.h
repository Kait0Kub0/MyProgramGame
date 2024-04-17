#pragma once


#include "system.h"
#include "Base.h"

// 前方宣言


//#define LENGTH_DEF  2.5f
#define LOOK_LENGTH_DEF  8.0f
#define LOOK_HEIGHT_DEF  3.0f
#define LOOK_ROT_SPEED 0.02f
#define LOOK_RADIUS 0.4f

class Look : public Base
{
private:
    bool debugFlag = false;
    bool keyFlag = false;

public:

    // 距離（長さ）
    float length;

    // 視点の高さ
    float height = 0;

    // 注視点
    vec3 gazePos;

    // アップベクトル
    vec3 upVec;

    // 基準オブジェクト
    Base* baseObj;

    // ウィンドウサイズ
    RECT rectWindow;

    // クライアント領域
    RECT rectClient;

    // 中央座標
    POINT centerPos;

    // 逆行列変換視点ベクトル
    vec3 inverseVec;


    Look();
    ~Look();

    // ベースのセット
    void SetBase(Base* base, float height = LOOK_HEIGHT_DEF, float length = LOOK_LENGTH_DEF);
    vec3 GetInverseVec();

    // 初期化
    void Init();


    // 移動処理
    void Move();

    // 表示処理
    void Disp();

    // ウィンドウ情報の取得
    void GetWindowConfig();

    // マウスを中央へ移動
    void SetMouseCenter();

    // デバッグ用視点
    void DebugLook();

};


// 実体へのアクセス
extern Look* lookObj;
