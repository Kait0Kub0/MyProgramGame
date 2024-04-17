#pragma once

#include "system.h"
#include "Base.h"

#define BULLET_RADIUS 0.25f
#define BULLET_SPEED  3.12f


class Bullet : public Base
{
private:
    // データフラグ
    bool dataFlag = false;

    // テクスチャ番号
    int texNo;
    // モデルデータ
    int modelNo;

    // 重力フラグ
    bool gravityFlag = false;


public:

    // コンストラクタ
    Bullet();

    // デストラクタ
    ~Bullet();

    // 初期化
    void Init();
    void Init(GLuint texNo, int mdNo, int shaderNo);

    // ショット処理
    void Shot(Base* base, vec3 mov, float speedOffset = 1.0f);

    // 移動処理
    void Move();

    // 表示処理
    void Disp();

};
