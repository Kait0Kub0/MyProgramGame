#include <iostream>
#include "Look.h"


Look *lookObj = new Look();

//Look* lookObjA[2] = { new Look(), new Look() };

Look::Look()
{
    // ウィンドウ情報取得
    GetWindowConfig();
    // 画面中央座標セット
    SetMouseCenter();

    // 視点の半径
    radius = LOOK_RADIUS;

    // バウンティング作成
    //bouncing = new Bouncing(radius, 8);
}


Look::~Look()
{

}

// ベースのセット
void Look::SetBase(Base* base, float height, float length)
{
    // エラーチェック nullptr
    if (base == nullptr)
    {
        std::cout << "オブジェクト設定エラー" << "\n";
        return;
    }

    // ベースの設定
    baseObj = base;

    // 視点の距離設定
    this->length = length;
    // 視点の高さ
    this->height = height;
}



// 逆行列変換視点ベクトル
vec3 Look::GetInverseVec()
{
    return inverseVec;
}


// 初期化
void Look::Init()
{
    gazePos = vec3(0, 0, 0);    // 注視点
    pos     = vec3(0, 0, 3);    // 視点の位置
    upVec   = vec3(0, 1, 0);    // アップベクトル

    // 視点の高さ
    height = LOOK_HEIGHT_DEF;
    // 視点までの長さ
    length = LOOK_LENGTH_DEF;

    // 逆行列変換視点ベクトル
    inverseVec = pos;

}


// 移動処理
void Look::Move()
{
    // クォータニオン取り出し用変数
    quat lookQuat;
    // クォータニオンポインタ
    lookQuat = baseObj->myQuaternion;

    // ターゲット座標をセット
    pos = baseObj->pos;                            // 視点座標
    pos.y += height;

    // アップベクトルの計算
    vec3 defUpVec = vec3(0, 1, 0);
    upVec = lookQuat * defUpVec;
    //defUpVec = baseObj->myQuaternion * defUpVec;

    // 視点座標計算
    vec3 defPos = vec3(0, height, length);
    gazePos = baseObj->pos + (lookQuat * defPos); // 注視点
    //defPos = baseObj->myQuaternion * defPos;

    // 見下ろし視点
    //pos.y = LOOK_HEIGHT_DEF;
}



// 表示処理
void Look::Disp()
{
    // 視点設定
    gluLookAt(
        pos.x,     pos.y,     pos.z,         // 視点 
        gazePos.x, gazePos.y, gazePos.z,     // 注視点
        upVec.x,   upVec.y,   upVec.z        // アップベクトル
    );

    // デバッグ視点
    DebugLook();

}


// ウィンドウ情報の取得
void Look::GetWindowConfig()
{
    // ウィンドウサイズ取得（ウィンドウサイズ変更には未対応）
    GetWindowRect(hwind, &rectWindow);

    // タイトルバーのサイズ
    int cyCaptionSize = GetSystemMetrics(SM_CYCAPTION);

    // 枠のサイズ
    int cxFrameSize = GetSystemMetrics(SM_CXFRAME);
    int cyFrameSize = GetSystemMetrics(SM_CYFRAME);

    // クライアント領域の計算
    rectClient.top    = rectWindow.top    + cyFrameSize + cyCaptionSize;
    rectClient.left   = rectWindow.left   + cxFrameSize;
    rectClient.bottom = rectWindow.bottom - cyFrameSize;
    rectClient.right  = rectWindow.right  - cxFrameSize;
}



// マウスを中央へ移動
void Look::SetMouseCenter()
{
    // キーの無効化処理
    short keyData;
    keyData = GetAsyncKeyState(VK_LSHIFT);
    if (keyData & 0x8000)
    {
        GetCursorPos(&centerPos);
        return;
    }

    POINT mousePos;

    // クライアント領域の中央座標計算
    mousePos.x = rectClient.left + ((rectClient.right - rectClient.left) / 2 );
    mousePos.y = rectClient.top  + ((rectClient.bottom - rectClient.top) / 2 );

    // 中央座標セット
    centerPos = mousePos;

    // マウス座標セット
    SetCursorPos(mousePos.x, mousePos.y);

}


// デバッグ用視点設定
void Look::DebugLook()
{

    short keyData = GetAsyncKeyState(VK_F11);
    if (keyData & 0x8000)
    {
        if (keyFlag == false)
        {
            keyFlag = true;
            debugFlag = !debugFlag; // フラグの反転
        }
    }
    else
    {
        keyFlag = false;
    }


    if (debugFlag == true)
    {
        // モデルビューマトリックスの初期化
        glLoadIdentity();

        // 視点設定
        gluLookAt(
            gazePos.x, 50, gazePos.z,            // 視点 
            gazePos.x, 0,  gazePos.z,            // 注視点
            0, 0, -1                             // アップベクトル
        );
    }


}