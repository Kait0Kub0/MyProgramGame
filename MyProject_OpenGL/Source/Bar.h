#pragma once

#include "system.h"
#include "Base.h"

// 前方宣言
class Look;

class Bar : public Base
{
private:

    // テクスチャ番号
    int texNo;
    // モデルデータ
    int modelNo;

    // マックスゲージ
    float maxGauge;

    // ダメージ
    float damage;

    // UV OFFSET
    vec2 uvOffset;

    // 視点
    Look* lookObj;


public:

    // コンストラクタ
    Bar();

    // デストラクタ
    ~Bar();

    // セッター
    void SetLook(Look* look);
    void SetMaxGauge(float max);
    void SetDamage(float dm);
    void SetSize(float w, float h);
    void SetPos(vec3 pos);


    // 初期化
    void Init();
    void Init(GLuint texNo, int mdNo, int shaderNo);

    // ダメージ計算処理
    void DamageCalc();

    // 表示処理
    void Disp();

};

