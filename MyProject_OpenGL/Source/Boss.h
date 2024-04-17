#pragma once

#include "system.h"
#include "Base.h"

#define BOSS_SPEED 0.02f
#define BOSS_ROT   2.0f
#define BOSS_SHOT_TIME 5000
#define BOSS_SHOT_TIME_MAX 60
#define BOSS_RADIUS 4.0f
#define BOSS_BULLET_MAX 800
#define BOSS_HP 30

enum class BossMode
{
    Think,
    Shot1,
    Shot2,
    MAX
};



// 前方宣言
class BulletSys;
class GameMainSys;

class Boss : public Base
{
private:

    // テクスチャ番号
    int texNo;
    // モデルデータ
    int modelNo;

    // 弾の管理システム
    BulletSys* bulletSysObj;

    // プレイヤーのデータ
    Base* playerBase;

    // ショット時間
    int shotStartTime = 0;

    // ショット方向
    vec3 shotRot;

    // 行動モード
    BossMode bmode = BossMode::Think;

    // ゲームメインシステム
    GameMainSys* gameMainSysObj;


public:

    // コンストラクタ
    Boss( GameMainSys* gms );

    // デストラクタ
    ~Boss();

    // アクセサーメソッド（セッター・ゲッター）
    void SetBulletSys(BulletSys* bs);
    void SetPlayer(Base* base);
    void SetGameMainSys(GameMainSys* gms);


    // 初期化
    void Init();
    void Init(GLuint texNo, int mdNo, int shaderNo);

    // AI
    void Ai();

    // 思考
    void Think();

    // 攻撃１
    void Shot01();

    // 攻撃２
    void Shot02();


    // ターゲット方向を向く
    void TargetRot();

    // マップの当たり判定
    void HitMap();

    // プレイヤーとの当たり判定
    void HitChk();

    // 移動処理
    void Move();

    // 表示処理
    void Disp();

    // 当たり判定で使用する共有処理
    virtual void CommonHit();



};

