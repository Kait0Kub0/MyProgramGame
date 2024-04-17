#pragma once

#include "system.h"

#include "Enemy.h"

// define 設定には ； をつけない！！！！
#define DEF_BULLET_SPEED    1.0f
#define DEF_BULLET_MAX      300

// 前方宣言
class Bullet;
class ExplosionSys;

class BulletSys
{
private:
    // 弾数
    int bulletMax;
    // ブレットオブジェクト
    Bullet* bulletObj;

    // 当たり判定のオブジェクト
    vector<Base*> hitObj;

    // 爆発システム
    ExplosionSys* explosionSysObj;

public:

    // コンストラクタ
    BulletSys( int max = DEF_BULLET_MAX );
    // デストラクタ
    ~BulletSys();

    // セッター・ゲッター
    // 当たり判定のオブジェクト
    template<typename T> void SetHitObj(T* ho, int addMax = 1);

    // 爆発システムセット
    void SetExplosionSys(ExplosionSys* es);

    // 弾数変更
    void SetBulletMax( int max );

    // 初期化
    void Init();
    // モデルデータの初期化
    void Init(GLuint texNo, int mdNo, int shaderNo);

    // ショット
    void Shot(  Base* base, vec3 mov,
                float speedOffset = 1.0f);

    // 移動処理
    void Move();

    // 当たり判定
    void HitChk();

    // 表示処理
    void Disp();


};


// セッター・ゲッター
// 当たり判定のオブジェクト
template<typename T> void BulletSys::SetHitObj(T* ho, int addMax)
{
    // オブジェクトの登録
    int size = hitObj.size();        // 登録数の取得

    hitObj.resize(size + addMax);    // 領域の拡張

    // 登録するオブジェクトの数ループ
    for (int i = 0; i < addMax; i++)
    {
        // ベースの取得
        Base* base = (Base *)(ho + i);
    
        hitObj[size + i] = base;         // データの登録
    }

}
