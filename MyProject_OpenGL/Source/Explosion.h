#pragma once

#include "system.h"
#include "Base.h"


class Explosion : public Base
{
private:
    // データフラグ
    bool dataFlag = false;

    // テクスチャ番号
    int texNo;
    // モデルデータ
    int modelNo;

    // 爆発時間
    float uTime;

public:

    // コンストラクタ
    Explosion();

    // デストラクタ
    ~Explosion();

    // 初期化
    void Init();
    void Init(GLuint texNo, int mdNo, int shaderNo);

    // 爆発処理
    void Ignition(Base* base);

    // 移動処理
    void Move();

    // 表示処理
    void Disp();
};
