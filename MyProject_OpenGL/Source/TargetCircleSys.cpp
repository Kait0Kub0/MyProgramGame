#include "TargetCircleSys.h"
#include "TargetCircle.h"
#include "Enemy.h"

// コンストラクタ
TargetCircleSys::TargetCircleSys()
{
    targetMax = TARGET_MAX;
    targetObj = nullptr;
    lookObj = nullptr;
    playerBase = nullptr;
    missileSysObj = nullptr;
}
// デストラクタ
TargetCircleSys::~TargetCircleSys()
{

}

// セッター
void TargetCircleSys::SetEnemy(Enemy* enemy)
{
    // エラーチェック
    if (enemy == nullptr)
    {
        cout << "TargetCircleSys::SetEnemyの引数が不正です。" << endl;
        return;
    }
    // 登録済みチェック
    //if (enemy->GetTargetCircle() != nullptr)
    //{
    //    return;
    //}

    // 登録済みのチェック
    for (int i = 0; i < targetMax; i++)
    {
        // アドレス計算
        TargetCircle* tg = (targetObj + i);
        // 生存チェック
        if (tg->dispFlag == true)
        {
            continue;
        }

        // 未登録の場合
        tg->SetEnemy(enemy);
        //enemy->SetTargetCircleCircle(tg);
        break;
    }
}

void TargetCircleSys::SetLook(Look* look)
{
    for (int i = 0; i < targetMax; i++)
    {
        // アドレス計算
        TargetCircle* tg = (targetObj + i);
        tg->SetLook(look);
    }
}

void TargetCircleSys::SetMissile(TargetCircle* target, Base* bs)
{
    target->SetMissile(bs);
}

void TargetCircleSys::SetMissileSys(MissileSys* ms)
{
    // エラーチェック
    if (ms == nullptr)
    {
        cout << "TargetCircleSys::SetMissileSysの引数が不正です。" << endl;
        return;
    }

    missileSysObj = ms;
}

// 初期化
void TargetCircleSys::Init()
{
    for (int i = 0; i < targetMax; i++)
    {
        // アドレス計算
        TargetCircle* tg = (targetObj + i);
        // 初期化呼び出し
        tg->Init();
    }
}
void TargetCircleSys::Init(GLuint texNo, int mdNo, int shaderNo)
{
    // ターゲット数設定
    targetMax = TARGET_MAX;

    // ターゲットオブジェクト作成
    targetObj = new TargetCircle[targetMax];

    for (int i = 0; i < targetMax; i++)
    {
        // アドレス計算
        TargetCircle* tg = (targetObj + i);
        // 初期化呼び出し
        tg->Init(texNo, mdNo, shaderNo);
    }
}

// ミサイル発射処理
void TargetCircleSys::MissileShot()
{
    // ターゲットロックデータのチェック

    // 距離のチェック

    // ミサイル発射

    // ミサイルセット

}

// 移動処理
void TargetCircleSys::Move()
{
    for (int i = 0; i < targetMax; i++)
    {
        // アドレス計算
        TargetCircle* tg = (targetObj + i);

        // 生存チェック
        if (tg->dispFlag == false)
        {
            continue;
        }
        // 移動(ビルボード回転処理)呼び出し
        tg->Move();
    }
}

// 表示処理
void TargetCircleSys::Disp()
{
    for (int i = 0; i < targetMax; i++)
    {
        // アドレス計算
        TargetCircle* tg = (targetObj + i);
        tg->Disp();
    }
}

// ターゲット解除
void TargetCircleSys::ReleaseEnemy(Enemy* enemy)
{
    // 登録されたターゲットサークル取得
    //TargetCircle* tobj = enemy->GetTargetCircle();
    //if (tobj == nullptr)
    //{
    //    // 登録なし
    //    return;
    //}
    //// ミサイル追跡中
    //if (tobj->GetMode() == TargetMode::MissileOn)
    //{
    //    return;
    //}

    //// 登録解除
    //tobj->Init();
    //enemy->SetTargetCircleCircle(nullptr);
}
