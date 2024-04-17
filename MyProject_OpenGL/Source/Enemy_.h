#pragma once

#include "system.h"
#include "Base.h"

#define ENEMY_SPEED 0.02f
#define ENEMY_ROT   2.0f
#define ENEMY_SHOT_TIME 3000

// 前方宣言
class GameMainSys;

class Enemy : public Base {
private:

    // データフラグ
    bool dataFlag = false;

    // テクスチャ番号
    int texNo;
    // モデルデータ
    int modelNo;

    // プレイヤーのデータ
    Base* playerBase;

    // ショット時間
    int shotStartTime = 0;

    // ゲームメインシステム
    GameMainSys* gameMainSysObj;

public:

    // コンストラクタ
    Enemy();

    // デストラクタ
    ~Enemy();

    // アクセサーメソッド（セッター・ゲッター）
    void SetPlayer(Base* base);
    void SetGameMainSys(GameMainSys* gms);

    // 初期化
    void Init();
    void Init(GLuint texNo, int mdNo, int shaderNo);

    // AI
    void Ai();

    // ターゲット方向を向く
    void TargetRot();

    // マップの当たり判定
    void HitMap();

    // 移動処理
    void Move();

    // 表示処理
    void Disp();

    // 当たり判定で使用する共有処理
    virtual void CommonHit();

};
