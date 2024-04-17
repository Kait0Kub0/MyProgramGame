#pragma once

#include "system.h"
#include "Base.h"


class CharPol : public Base
{
private:
    // データフラグ
    bool dataFlag = false;

    // テクスチャ番号
    int texNo;
    // モデルデータ
    int modelNo;

    // キー情報保存
    bool keyFlag[256];

public:

    // コンストラクタ
    CharPol();

    // デストラクタ
    ~CharPol();

    // 初期化
    void Init();
    void Init(GLuint texNo, int mdNo, int shaderNo);

    // キー入力
    void KeyIn();

    // マップの当たり判定
    void HitMap();

    // 移動処理
    void Move();

    // 表示処理
    void Disp();

};


