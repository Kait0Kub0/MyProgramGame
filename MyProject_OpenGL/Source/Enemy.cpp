#include "Enemy.h"
#include "BulletSys.h"
#include "GameMainSys.h"
#include "Map.h"

// コンストラクタ
Enemy::Enemy()
{
    // ゲームメインシステム設定
    //gameMainSysObj = gms;

    // 位置情報
    pos = vec3(0, 0, 0);
    // 回転情報
    rot = vec3(0, 0, 0);

    // データフラグ
    dataFlag = false;
    // 表示フラグ
    dispFlag = false;

    // テクスチャ番号
    texNo = 0;
    // モデルデータ
    modelNo = 0;

    // 移動ベクトル初期化
    mov = vec3(0, 0, 0);

    // ジャンプ
    jumpMove = vec3(0, 0, 0);

    // スピード
    speed = 0;

    // 半径
    radius = 0;

}


// デストラクタ
Enemy::~Enemy()
{

}

// マップ情報セット
void Enemy::SetMap(Map* map)
{
    if (map == nullptr)
    {
        cout << "Enemy::SetMapの引数が不正です" << endl;
        return;
    }

    mapObj = map;

}

void Enemy::SetGameMainSys(GameMainSys* gms)
{
    if (gms == nullptr)
    {
        cout << "Enemy::SetGameMainSysの引数が不正です" << endl;
        return;
    }

    gameMainSysObj = gms;
}

void Enemy::SetPlayer(Base* base)
{
    if (base == nullptr)
    {
        cout << "Enemy::SetPlayerの引数が不正です" << endl;
        return;
    }

    playerObj = base;
}

// 初期化
void Enemy::Init()
{
    this->dispFlag = true;
    pos = vec3(0, 0, 0);
    pos.x = (rand() % 20) - 10;
    pos.z = (rand() % 20) - 10;

    rot = vec3(0, 0, 0);
    mov = vec3(0, 0, 0);
    scale = vec3(1, 1, 1);
    jumpMove = vec3(0, 0, 0);
    speed = 0.1f;
    radius = 0.5f;
    alpha = 1.0f;

    // 中心座標
    centerPos = vec3(0, radius, 0);


    // クォータニオンの初期化
    vec3 eulerAngles(radians(rot.x), radians(rot.y), radians(rot.z));
    myQuaternion = quat(eulerAngles);


    // マップサイズ取得
    //vec3 mSize = mapObj->GetMapSize();

    //leftPos = vec3(-(mSize.x / 2.0f), 0, -(mSize.z / 2.0f));
    //mapWidth = mSize.x;
    //mapHeight = mSize.z;

}
void Enemy::Init(GLuint texNo, int mdNo, int shaderNo)
{
    // モデルデータの設定
    this->modelNo = mdNo;

    // テクスチャ番号保存
    this->texNo = texNo;

    // シェーダー番号
    this->shaderNo = shaderNo;

    // 座標の初期化
    //Init(false);

}


// キー入力
void Enemy::KeyIn()
{
}


// マップの当たり判定
bool Enemy::HitMap( vec3 pos )
{

    // 左側チェック
    if (leftPos.x > pos.x)
    {
        return true;
    }

    // 右側チェック
    if ( (leftPos.x+mapWidth) < pos.x )
    {
        return true;
    }

    // 上側チェック
    if (leftPos.z > pos.z)
    {
        return true;
    }

    // 下側チェック
    if ( (leftPos.z+mapHeight) < pos.z )
    {
        return true;
    }

    // マップ内
    return false;
}



// 移動処理
void Enemy::Move()
{
    // 前方向ベクトル
    vec3 defMov = vec3(0, 0, -1);

    // 回転クォータニオン
    vec3 rotAngle = vec3(0, glm::radians(rot.y), 0);
    myQuaternion = quat(rotAngle);

    mov = myQuaternion * defMov;

    // 移動処理
    mov = (mov * speed) + jumpMove;
    pos += mov;

    // 移動量計算
    movDistance -= glm::length(mov);

    // 重力処理
    jumpMove -= vec3(0, 0.005, 0);

    // 地面との接触
    if (pos.y < 0)
    {
        pos.y = 0;
        jumpMove = vec3(0, 0, 0);
        // ジャンプフラグ OFF
        jumpFlag = false;
    }

    // ジャンプフラグチェック
    if (jumpFlag == true)
    {
        keyInFlag = true;
    }

    // 外周チェック
    if ( pos.x < leftPos.x || pos.x > -leftPos.x
      || pos.z < leftPos.z || pos.z > -leftPos.z )
    {
        this->dispFlag = false;
    }

}


// AI処理
void Enemy::Ai()
{

}


// 思考処理
void Enemy::Think()
{

}



// 表示処理
void Enemy::Disp()
{
    // 生存チェック
    if (dispFlag == false)
    {
        return;
    }


    // シェーダー選択
    shaderObj->ShaderOn(shaderNo);


    // ポリゴンモデルの表示
    // マトリックスの保存
    glPushMatrix();

    vec3 EulerAngles(radians(rot.x), radians(rot.y), radians(rot.z));
    myQuaternion = quat(EulerAngles);

    mat4 transform = toMat4(myQuaternion);

    // 平行移動
    transform[3][0] = pos.x;
    transform[3][1] = pos.y;
    transform[3][2] = pos.z;

    glMultMatrixf((float*)&transform);

    glScalef(scale.x, scale.y, scale.z);

    // モデル表示
    ModelData* modelData = modelSysObj->GetModelData(modelNo);
    // テクスチャセット
    modelData->textureNo = texNo;
    // POLオブジェクト表示
    ModelDataDisp(modelData);


    // マトリックスの取得
    glPopMatrix();


}


// 当たり判定で使用する共有処理
void Enemy::CommonHit()
{
    // 表示フラグOFF
    this->dispFlag = false;

    // プレイヤー残機マイナス
    //gameMainSysObj->playerCount--;
}
