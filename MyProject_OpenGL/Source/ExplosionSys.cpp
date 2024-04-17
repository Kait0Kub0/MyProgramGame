
#include "ExplosionSys.h"
#include "Explosion.h"
#include "Hitchk.h"
#include "Music/MusicSys.h"


// コンストラクタ
ExplosionSys::ExplosionSys()
{
    // 爆発数セット
    explosionMax = DEF_EXPLOSION_MAX;
    // Explosion オブジェクトの作成
    explosionObj = new Explosion[DEF_EXPLOSION_MAX];

    musicSys = new MusicSys();
    // 音データ読み込み処理
    musicSys->SetMusicOgg("DAT/SOUND/mist.ogg");


}
// デストラクタ
ExplosionSys::~ExplosionSys()
{
    // Explosion オブジェクトの削除
    if (explosionObj != nullptr)
    {
        // 領域の削除
        delete[] explosionObj;
        // アドレスも消す
        explosionObj = nullptr;
    }
}



// 初期化
void ExplosionSys::Init()
{

    // 全ての弾の初期化
    for (int i = 0; i < explosionMax; i++)
    {
        // ブレットの初期化
        // + i はアドレスをずらす処理
        (explosionObj + i)->Init();
    }

}
// モデルデータの初期化
void ExplosionSys::Init(GLuint texNo, int mdNo, int shaderNo)
{
    // 全モデルの初期化
    for (int i = 0; i < explosionMax; i++)
    {
        // 全モデルに
        // テクスチャ・モデル・シェーダーの管理番号を渡す
        (explosionObj + i)->Init(texNo, mdNo, shaderNo);
    }
}


// 爆発発生
void ExplosionSys::Ignition(Base* base)
{
    // 全ての爆発オブジェクトを検索
    for (int i = 0; i < explosionMax; i++)
    {
        // 生存チェック
        if ((explosionObj + i)->dispFlag == true)
        {
            continue;   // ループの先頭へ戻る
        }

        // 爆発オブジェクトを使用する
        (explosionObj + i)->Ignition(base);

        // １発で処理を終了
        break;
    }
}


// 移動処理
void ExplosionSys::Move()
{
    // 全爆発オブジェの移動
    for (int i = 0; i < explosionMax; i++)
    {
        // 生存チェック
        if ((explosionObj + i)->dispFlag == false)
        {
            continue;   // ループの先頭へ戻る
        }

        (explosionObj + i)->Move();
    }
}



// 表示処理
void ExplosionSys::Disp()
{
    // 全弾の表示
    for (int i = 0; i < explosionMax; i++)
    {
        // 生存チェック
        if ((explosionObj + i)->dispFlag == false)
        {
            continue;   // ループの先頭へ戻る
        }

        (explosionObj + i)->Disp();
    }
}

