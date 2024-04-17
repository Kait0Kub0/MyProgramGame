#include "Enemy.h"
#include "GameMainSys.h"

// コンストラクタ
Enemy::Enemy()
{
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


// プレイヤーのセット
void Enemy::SetPlayer(Base* base)
{
    // エラーチェック
    if (base == nullptr)
    {
        cout << "Enemy::SetPlayerで引数の値が不正です。";
        return;
    }

    this->playerBase = base;

}



// ゲームメインシステムセット
void Enemy::SetGameMainSys(GameMainSys* gms)
{
    // エラーチェック
    if (gms == nullptr)
    {
        cout << "Enemy::SetGameMainSysで引数の値が不正です。" << endl;
        return;
    }

    gameMainSysObj = gms;
}



// 初期化
void Enemy::Init()
{
    dispFlag = true;
    pos = vec3(0, 0, 0);

    pos.x = ( (rand() % 170) - 80 ) / 10.0f;
    pos.z = ( (rand() % 170) - 80 ) / 10.0f;

    rot = vec3(0, 0, 0);
    mov = vec3(0, 0, 0);
    scale = vec3(1, 1, 1);
    jumpMove = vec3(0, 0, 0);
    speed = ENEMY_SPEED;
    radius = 0.5f;
    alpha = 1.0f;

    // 中心座標
    centerPos = vec3(0, radius, 0);

    // クォータニオンの初期化
    vec3 eulerAngles(radians(rot.x), radians(rot.y), radians(rot.z));
    myQuaternion = quat(eulerAngles);

    // ショットタイムの初期化(現在時間の取得)
    shotStartTime = timeGetTime();

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
    Init();

}


// Ai処理
void Enemy::Ai()
{
    // ターゲット方向へ振り向く
    TargetRot();

    // プレイヤー方向を向いているか？
    if (abs(rot.y) <= 1.0f)
    {
        int currentTime = timeGetTime();
        // ショットタイムのチェック
        if ((currentTime - shotStartTime) > ENEMY_SHOT_TIME)
        {
            // ショット時間の初期化(現在時間で上書き)
            shotStartTime = currentTime;

            // ショット方向計算
            vec3 defMov = vec3(0, 0, -1);
            vec3 emov = myQuaternion * defMov;

       }

    }

}


// ターゲット方向を向く
void Enemy::TargetRot()
{
    // 移動処理
    rot = vec3(0, 0, 0);

    // 回転処理

    // 自分の方向ベクトル
    vec3 defMov = vec3(0, 0, -1);
    vec3 emov = myQuaternion * defMov;

    // プレイヤー方向ベクトル
    vec3 pmov = playerBase->pos - this->pos;
    // 単位ベクトルに変換
    pmov = glm::normalize(pmov);

    // 内積計算(ベクトルの掛け算)
    float dot = glm::dot(emov, pmov);

    // 誤差修正(-1 から 1)
    dot = glm::clamp(dot, -1.0f, 1.0f);

    // acos 角度に戻す
    float dotRot = acos(dot);
    // 度数に戻す
    dotRot = glm::degrees(dotRot);

    // 外積計算(直交するベクトルを求める)
    vec3 crossVec = glm::cross(emov, pmov);

    // 回転方向の調整
    if (crossVec.y < 0)
    {
        dotRot = -dotRot;
    }

    // 回転量の調整
    dotRot = glm::clamp(dotRot, -ENEMY_ROT, ENEMY_ROT);

    // 回転量設定
    rot = vec3(0, dotRot, 0);
}


// マップの当たり判定
void Enemy::HitMap()
{

}



// 移動処理
void Enemy::Move()
{
    // 前方向ベクトル
    vec3 defMov = vec3(0, 0, -1);
    mov = myQuaternion * defMov;

    // 移動処理
    pos += (mov * speed) + jumpMove;

    // 重力処理
    jumpMove -= vec3(0, 0.001, 0);

    // 地面との接触
    if (pos.y < 0)
    {
        pos.y = 0;
        jumpMove = vec3(0, 0, 0);
    }

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

    // キャラクタの回転
    vec3 EulerAngles(radians(rot.x), radians(rot.y), radians(rot.z));
    myQuaternion = myQuaternion * quat(EulerAngles);

    // 4x4行列に変換
    mat4 transform = toMat4(myQuaternion);

    // 平行移動
    transform[3][0] = pos.x;
    transform[3][1] = pos.y;
    transform[3][2] = pos.z;

    // OpenGLにマトリックスセット
    glMultMatrixf((float*)&transform);

    // 拡大縮小計算
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
// 注意：virtual は書かない！！
void Enemy::CommonHit()
{
    // 表示フラグOFF
    this->dispFlag = false;

    // 撃破数のカウント
    gameMainSysObj->killsCount++;

}
