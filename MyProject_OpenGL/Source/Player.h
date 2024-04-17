#pragma once

#include "system.h"
#include "Base.h"

#define PLAYER_SPEED_MAX 0.1f
#define PLAYER_SPEED_MIN 0.05f
#define PLAYER_SPEED_ACCELE 0.002f
#define PLAYER_SPEED_JUMP 0.13f
#define PLAYER_RELOAD_TIME 100
#define PLAYER_ROT_MAX 5.0f
#define LOCK_ON_DEGREE 25           // ロックオン範囲(半分の角度)

// 前方宣言
class GameMainSys;
class Map;
class Enemy;
class EnemySys;
class BulletSys;

class Player : public Base
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

    vec3 keyMov;

    // ゲームメインシステム
    GameMainSys* gameMainSysObj;

    // エネミーのリスト
    vector<Base*> enemyBase;
    EnemySys* enemySysObj;
    int enemyMax;
    int lockOnNo;

    // 弾システム
    BulletSys* bulletSysObj;

    // プレイヤー番号
    int playerNo;

    //マップデータ
    Map* mapObj;
    vec3 mapSizeHalf;

    // 方向変換フラグ
    bool keyInFlag = false;
    // ロックオンフラグ
    bool lockOnFlag = false;

    // 弾の発射間隔計測
    int shotTimeStart;

    // クォータニオン
    quat q1;
    quat q2;
    
    // キー入力時間記録変数
    float keyTime = 0;

public:

    // コンストラクタ
    Player(GameMainSys* gms);

    // デストラクタ
    ~Player();

    // アクセサーメソッド（セッター・ゲッター）
    void SetEnemySys(EnemySys* es);
    void SetEnemy(Base* bs);
    void SetPlayerNo(int pno);
    void SetMap(Map* map);
    void SetBulletSys(BulletSys* bs);

    // 初期化
    void Init(bool dispFlag);
    void Init(GLuint texNo, int mdNo, int shaderNo);

    // キー入力
    void KeyIn();
    void KeyIn_Debug();

    // ロックオン対象サーチ
    void LockSearch();

    // ロックオン
    void LockOn();

    // マップの当たり判定
    void HitMap();

    // 移動処理
    void Move();

    // 表示処理
    void Disp();

    // 当たり判定で使用する共有処理
    virtual void CommonHit();

};


