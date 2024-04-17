#pragma once

#include "system.h"

// define 設定には ； をつけない！！！！
#define DEF_ENEMY_SPEED    1.0f
#define DEF_ENEMY_MAX      5

// 前方宣言
class Enemy;
class BulletSys;
class ExplosionSys;
class GameMainSys;
class Player;
class TargetCircleSys;

class EnemySys
{
private:
    // エネミー数
    int enemyMax;
    // エネミーオブジェクト
    Enemy* enemyObj;

    // ゲームメインシステム
    GameMainSys* gameMainSysObj;


    // 当たり判定を行うオブジェクトのリスト
    vector<Base*> hitObj;

    // ブレットシステム
    BulletSys* bulletSysObj;

    // プレイヤーのベース
    Player* playerObj;

    // ターゲットサークルシステム
    TargetCircleSys* targetSysObj;

public:

    // コンストラクタ
    EnemySys(GameMainSys *gms);
    // デストラクタ
    ~EnemySys();

    // セッター・ゲッター
    // 当たり判定のオブジェクト
    void SetHitObj(Base* base);

    // ブレットシステムの取得
    void SetBulletSys(BulletSys* bs);

    // プレイヤー情報取得
    void SetPlayer(Player* pl);

    // ターゲットサークルシステムのセット
    void SetTargetCircleSys(TargetCircleSys* tcs);

    // エネミーオブジェクトの取得
    Enemy* GetEnemyObj() { return enemyObj; };
    // エネミー数の取得
    int GetEnemyMax()    { return enemyMax; };


    // 初期化
    void Init();
    // モデルデータの初期化
    void Init(GLuint texNo, int mdNo, int shaderNo);

    // Ai
    void Ai();

    // 移動処理
    void Move();

    // 当たり判定
    void HitChk();

    // 表示処理
    void Disp();
};
