
#include "EnemySys.h"
#include "Enemy.h"

#include "Hitchk.h"
#include "GameMainSys.h"
#include "Player.h"

// コンストラクタ
EnemySys::EnemySys(GameMainSys* gms)
{
    // ゲームメインシステム設定
    gameMainSysObj = gms;

    // Enemy数セット
    enemyMax = DEF_ENEMY_MAX;
    // Enemy オブジェクトの作成
    enemyObj = new Enemy[DEF_ENEMY_MAX];

    // 作成したエネミーをhitObjに登録
    for (int i = 0; i < enemyMax; i++)
    {
        // ヒットオブジェクトに登録
        SetHitObj( (enemyObj + i) );
    }

    // 全エネミーにゲームメインシステムをセット
    for (int i = 0; i < enemyMax; i++)
    {
        (enemyObj + i)->SetGameMainSys(gameMainSysObj);
    }

}

// デストラクタ
EnemySys::~EnemySys()
{
    // Enemy オブジェクトの削除
    if (enemyObj != nullptr)
    {
        // 領域の削除
        delete[] enemyObj;
        // アドレスも消す
        enemyObj = nullptr;
    }
}


// セッター・ゲッター
// 当たり判定のオブジェクト
void EnemySys::SetHitObj(Base* base)
{
    // オブジェクトの登録
    int size = hitObj.size();   // 登録数の取得  0       1

    hitObj.resize(size + 1);    // 領域の拡張    0 + 1   1 + 1

    hitObj[size] = base;        // データの登録  [0]     [1]

}


// ブレットシステムの取得
void EnemySys::SetBulletSys(BulletSys* bs)
{
    // エラーチェック
    if (bs == nullptr)
    {
        cout << "EnemySys::SetBulletSysの引数が不正です。" << endl;
        return;
    }

    // アドレスセット
    for (int i = 0; i < enemyMax; i++)
    {
        //(enemyObj + i)->SetBulletSys(bs);
    }

}


// プレイヤー情報取得
void EnemySys::SetPlayer(Player* pl)
{
    // エラーチェック
    if (pl == nullptr)
    {
        cout << "EnemySys::SetPlayerの引数が不正です。" << endl;
        return;
    }

    // アドレスセット
    //for (int i = 0; i < enemyMax; i++)
    //{
    //    (enemyObj + i)->SetPlayer((Base*)pl);
    //    
    //}

    // 管理システムもキープ
    this->playerObj = pl;

    // プレイヤーにエネミー情報セット
    for (int i = 0; i < enemyMax; i++)
    {
        playerObj->SetEnemy(enemyObj + i);
    }
}

// ターゲットサークルシステムのセット
void EnemySys::SetTargetCircleSys(TargetCircleSys* tcs)
{
    // エラーチェック
    if (tcs == nullptr)
    {
        cout << "EnemySys::SetTargetSysの引数が不正です。" << endl;
        return;
    }
    targetSysObj = tcs;
}

// 初期化
void EnemySys::Init()
{

    // 全てのエネミーの初期化
    for (int i = 0; i < enemyMax; i++)
    {
        // エネミーの初期化
        // + i はアドレスをずらす処理
        (enemyObj + i)->Init();
    }

}
// モデルデータの初期化
void EnemySys::Init(GLuint texNo, int mdNo, int shaderNo)
{
    // 全モデルの初期化
    for (int i = 0; i < enemyMax; i++)
    {
        // 全モデルに
        // テクスチャ・モデル・シェーダーの管理番号を渡す
        (enemyObj + i)->Init(texNo, mdNo, shaderNo);
    }
}


// Ai
void EnemySys::Ai()
{
    // 全モデルのAI処理
    for (int i = 0; i < enemyMax; i++)
    {
        // 生存チェック
        if ((enemyObj + i)->dispFlag == false)
        {
            continue;   // ループの先頭へ戻る
        }

        (enemyObj + i)->Ai();
    }

}



// 移動処理
void EnemySys::Move()
{
    // 全エネミーデータの移動
    for (int i = 0; i < enemyMax; i++)
    {
        // 生存チェック
        if ((enemyObj + i)->dispFlag == false)
        {
            continue;   // ループの先頭へ戻る
        }

        //(enemyObj + i)->Move();

    }
}


// 当たり判定
void EnemySys::HitChk()
{

    // ヒットオブジェクトのループ
    for (int hLoop = 0; hLoop < hitObj.size(); hLoop++)
    {
        // ヒットオブジェクトの生存チェック
        if (hitObj[hLoop]->dispFlag == false)
        {
            continue;   // ループの先頭へ戻る
        }

        // アドレスセット
        Base* hobj = hitObj[hLoop];


        // 当たり判定のオブジェクトのループ
        for ( int tLoop=(hLoop+1); tLoop < hitObj.size(); tLoop++ )
        {

            // 当たり判定のオブジェクトの生存チェック
            if ( hitObj[ tLoop ]->dispFlag == false )
            {
                continue;   // ループの先頭へ戻る
            }
            // アドレスセット
            Base* tobj = hitObj[tLoop];

            // 円と円の当たり判定の押し戻しを行う
            bool ret = Hitchk::PushCircleHitCircle( hobj, tobj );

        } //当たり判定ループ

    } // ヒットオブジェクトのループ

}

// 表示処理
void EnemySys::Disp()
{
    // 全エネミーの表示
    for (int i = 0; i < enemyMax; i++)
    {
        // 生存チェック
        if ((enemyObj + i)->dispFlag == false)
        {
            continue;   // ループの先頭へ戻る
        }

        (enemyObj + i)->Disp();
    }
}

