#pragma once

#include "Base.h"

#define TARGET_MAX 30

// 前方宣言
class TargetCircle;
class Look;
class MissileSys;
class Enemy;

class TargetCircleSys
{
private:

    // オブジェクトの数
    int targetMax;

    // ターゲットオブジェクト
    TargetCircle* targetObj;

    // 視点オブジェクト
    Look* lookObj;

    // プレイヤーのベース
    Base* playerBase;

    // ミサイルシステム
    MissileSys* missileSysObj;

public:
    // コンストラクタ
    TargetCircleSys();
    // デストラクタ
    ~TargetCircleSys();

    // セッター
    void SetEnemy(Enemy* enemy);
    void SetLook(Look* look);
    void SetMissile(TargetCircle *target, Base* bs);
    void SetMissileSys(MissileSys* ms);

    // 初期化
    void Init();
    void Init(GLuint texNo, int mdNo, int shaderNo);

    // ミサイル発射処理
    void MissileShot();

    // 移動処理
    void Move();

    // 表示処理
    void Disp();

    // ターゲット解除
    void ReleaseEnemy(Enemy* enemy);

};
