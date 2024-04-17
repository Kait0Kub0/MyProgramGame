
#include "BulletSys.h"
#include "Bullet.h"
#include "Hitchk.h"
#include "ExplosionSys.h"


// コンストラクタ
BulletSys::BulletSys( int max )
{
    // 弾数セット
    bulletMax = max;
    // Bullet オブジェクトの作成
    bulletObj = new Bullet[ bulletMax ];
}
// デストラクタ
BulletSys::~BulletSys()
{
    // Bullet オブジェクトの削除
    if (bulletObj != nullptr)
    {
        // 領域の削除
        delete[] bulletObj;
        // アドレスも消す
        bulletObj = nullptr;
    }
}


// 爆発システムセット
void BulletSys::SetExplosionSys(ExplosionSys* es)
{
    // エラーチェック
    if (es == nullptr)
    {
        cout << "BulletSys::SetExplosionSysの引数が不正です。" << endl;
        return;
    }

    // 爆発システムセット
    explosionSysObj = es;
}


// 弾数変更
void BulletSys::SetBulletMax(int max)
{
    // 領域の開放
    if (bulletObj != nullptr)
    {
        // 前の領域を開放
        delete[] bulletObj;
        bulletObj = nullptr; // アドレスもクリア
    }

    // 最大値の更新
    bulletMax = max;

    // 領域の確保
    bulletObj = new Bullet[bulletMax];
}


// 初期化
void BulletSys::Init()
{

    // 全ての弾の初期化
    for (int i = 0; i < bulletMax; i++)
    {
        // ブレットの初期化
        // + i はアドレスをずらす処理
        (bulletObj + i)->Init();
    }

}
// モデルデータの初期化
void BulletSys::Init(GLuint texNo, int mdNo, int shaderNo)
{
    // 全モデルの初期化
    for (int i = 0; i < bulletMax; i++)
    {
        // 全モデルに
        // テクスチャ・モデル・シェーダーの管理番号を渡す
        (bulletObj + i)->Init(texNo, mdNo, shaderNo);
    }
}


// ショット
void BulletSys::Shot(Base* base, vec3 mov, float speedOffset)
{
    // 全ての弾へショット処理を渡す
    for (int i = 0; i < bulletMax; i++)
    {
        // 生存チェック
        if ((bulletObj + i)->dispFlag == true)
        {
            continue;   // ループの先頭へ戻る
        }

        // 弾を使用する
        (bulletObj + i)->Shot(base, mov, speedOffset);

        // １発で処理を終了
        break;
    }
}


// 移動処理
void BulletSys::Move()
{
    // 全弾データの移動
    for (int i = 0; i < bulletMax; i++)
    {
        // 生存チェック
        if ((bulletObj + i)->dispFlag == false)
        {
            continue;   // ループの先頭へ戻る
        }

        (bulletObj + i)->Move();
    }
}


// 当たり判定
void BulletSys::HitChk()
{

    // ブレットのループ
    for (int bLoop = 0; bLoop < bulletMax; bLoop++)
    {
        // ブレットの生存チェック
        if ((bulletObj + bLoop)->dispFlag == false)
        {
            continue;   // ループの先頭へ戻る
        }

        // アドレスセット
        Base* bobj = (bulletObj + bLoop);

        // 当たり判定のオブジェクトのループ
        for (int hLoop = 0; hLoop < hitObj.size(); hLoop++)
        {

            // 当たり判定のオブジェクトの生存チェック
            if (hitObj[hLoop]->dispFlag == false)
            {
                continue;   // ループの先頭へ戻る
            }
            // アドレスセット
            Base* hobj = hitObj[hLoop];

            // 円と円の当たり判定を行う
            // bool ret = Hitchk::CircleHitCircle( (enemyObj+bLoop), hitObj[hLoop] );
            bool ret = Hitchk::CircleHitCircle(bobj, hobj);

            // 当たりの場合
            if (ret == true)
            {
                // オブジェクトの死亡
                bobj->dispFlag = false;
                //hobj->dispFlag = false; // 登録されたモデル

                // バーチャルの共通関数の呼び出し
                hobj->CommonHit();

                // 爆発の発生
                explosionSysObj->Ignition(hobj);

                break;  // 当たり判定オブジェクトのループを抜ける
            }

        } //当たり判定ループ

    } // ブレットのループ

}

// 表示処理
void BulletSys::Disp()
{
    // 全弾の表示
    for (int i = 0; i < bulletMax; i++)
    {
        // 生存チェック
        if ((bulletObj + i)->dispFlag == false)
        {
            continue;   // ループの先頭へ戻る
        }

        (bulletObj + i)->Disp();
    }
}

