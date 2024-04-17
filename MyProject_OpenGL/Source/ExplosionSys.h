#pragma once

#include "system.h"

// define 設定には ； をつけない！！！！
#define DEF_EXPLOSION_SPEED    1.0f
#define DEF_EXPLOSION_MAX      300

// 前方宣言
class Explosion;
class MusicSys;

class ExplosionSys
{
private:
    // 爆発数
    int explosionMax;
    // 爆発オブジェクト
    Explosion* explosionObj;

    // 音の再生システム
    MusicSys* musicSys;

public:

    // コンストラクタ
    ExplosionSys();
    // デストラクタ
    ~ExplosionSys();

    // 初期化
    void Init();
    // モデルデータの初期化
    void Init(GLuint texNo, int mdNo, int shaderNo);

    // 爆発発生
    void Ignition(Base* base);

    // 移動処理
    void Move();

    // 表示処理
    void Disp();

};
