#include "Boss.h"
#include "BulletSys.h"
#include "GameMainSys.h"
#include "Hitchk.h"

// コンストラクタ
Boss::Boss( GameMainSys* gms )
{
    // ゲームメインシステム設定
    gameMainSysObj = gms;

    // 位置情報
    pos = vec3(0, 0, 0);
    // 回転情報
    rot = vec3(0, 0, 0);

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
Boss::~Boss()
{

}


// ブレットシステム取得
void Boss::SetBulletSys(BulletSys* bs)
{
    // エラーチェック
    if (bs == nullptr)
    {
        cout << "Boss::SetBulletSysで引数の値が不正です。";
        return;
    }

    // ポインタのアドレスセット
    this->bulletSysObj = bs;

}


// プレイヤーのセット
void Boss::SetPlayer(Base* base)
{
    // エラーチェック
    if (base == nullptr)
    {
        cout << "Boss::SetPlayerで引数の値が不正です。";
        return;
    }

    this->playerBase = base;

}



// ゲームメインシステムセット
void Boss::SetGameMainSys(GameMainSys* gms)
{
    // エラーチェック
    if (gms == nullptr)
    {
        cout << "Boss::SetGameMainSysで引数の値が不正です。" << endl;
        return;
    }

    gameMainSysObj = gms;
}



// 初期化
void Boss::Init()
{
    dispFlag = false;

    pos = vec3(0, BOSS_RADIUS / 4.0, 0);

    //pos.x = ((rand() % 170) - 80) / 10.0f;
    //pos.z = ((rand() % 170) - 80) / 10.0f;

    rot = vec3(0, 0, 0);
    mov = vec3(0, 0, 0);
    scale = vec3(0.02, 0.02, 0.02);
    jumpMove = vec3(0, 0, 0);
    speed = ENEMY_SPEED;
    radius = 4.0f;
    alpha = 1.0f;

    // 中心座標
    centerPos = vec3(0, 0, 0);

    // ライフポイント
    life = BOSS_HP;
    // 撃破フラグOFF
    gameMainSysObj->bossKillFlag = false;


    // クォータニオンの初期化
    vec3 eulerAngles(radians(rot.x), radians(rot.y), radians(rot.z));
    myQuaternion = quat(eulerAngles);

    // ショットタイムの初期化(現在時間の取得)
    shotStartTime = timeGetTime();

}
void Boss::Init(GLuint texNo, int mdNo, int shaderNo)
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
void Boss::Ai()
{

    switch (bmode)
    {
    case BossMode::Think:
        Think();
        break;
    case BossMode::Shot1:
        Shot01();
        break;
    case BossMode::Shot2:
        Shot02();
        break;
    }
}


// 思考
void Boss::Think()
{
    // 時間の経過チェック
    int currentTime = timeGetTime();
    // ショットタイムのチェック
    if ((currentTime - shotStartTime) > BOSS_SHOT_TIME)
    {
        // 攻撃時間の初期化
        shotStartTime = currentTime;

        // 攻撃の分岐
        int attackNo = rand() % 2;
        switch (attackNo)
        {
        case 0:
            this->bmode = BossMode::Shot1;
            break;
        case 1:
            this->bmode = BossMode::Shot2;
            break;
        }

    }

}


// 攻撃１(渦巻き攻撃)
void Boss::Shot01()
{
    // 角度 720°

    // ショット方向計算
    vec3 EulerAngles(radians(shotRot.x), radians(shotRot.y), radians(shotRot.z));
    quat shotQuat = quat(EulerAngles);
    vec3 defMov = vec3(1, 0, 0);
    vec3 smov = shotQuat * defMov;

    // ショット
    Base damyBase = *this;
    damyBase.pos.y = 0.0;
    damyBase.radius = 0.3;
    bulletSysObj->Shot(&damyBase, smov, 0.2f);

    // 角度の変更
    shotRot.y += 3;

    // 角度チェック
    if (shotRot.y > 720)
    {
        // ショットタイマーの初期化
        shotStartTime = timeGetTime();

        // 回転量の初期化
        shotRot = vec3(0, 0, 0);

        // モード変更
        bmode = BossMode::Think;
    }

}



// 攻撃２
void Boss::Shot02()
{
    // 角度設定
    // ショット方向計算
    // 回転量
    shotRot.y = rand() % 360;
    shotRot.z = ((rand() % 50) + 800) / 10.0f;

    // Z軸回転
    vec3 EulerAngles(0, 0, radians(shotRot.z));
    quat shotQuat = quat(EulerAngles);
    vec3 defMov = vec3(1, 0, 0);
    vec3 smov = shotQuat * defMov;

    //Y軸回転
    EulerAngles = vec3(0, radians(shotRot.y), 0);
    shotQuat = quat(EulerAngles);
    smov = shotQuat * smov;

    // ショット
    Base damyBase = *this;
    damyBase.pos.y = 0.0;
    damyBase.radius = 0.3;
    bulletSysObj->Shot(&damyBase, smov, 1.0f, true);

    shotStartTime++;

    // 時間のチェック
    int currentTime = timeGetTime();

    if ((currentTime - shotStartTime) > BOSS_SHOT_TIME_MAX )
    {
        // ショットタイマーの初期化
        shotStartTime = timeGetTime();

        // 回転量の初期化
        shotRot = vec3(0, 0, 0);

        // モード変更
        bmode = BossMode::Think;
    }

}


// ターゲット方向を向く
void Boss::TargetRot()
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
    dotRot = glm::clamp(dotRot, -BOSS_ROT, BOSS_ROT);

    // 回転量設定
    rot = vec3(0, dotRot, 0);
}


// マップの当たり判定
void Boss::HitMap()
{

}



// プレイヤーとの当たり判定
void Boss::HitChk()
{

    // 円と円の当たり判定の押し戻しを行う
    bool ret = Hitchk::PushCircleHitCircle(this, playerBase);


    // 地面との押し戻し
    if (pos.y < 0.0f)
    {
        pos.y = 0.0f;
    }

}



// 移動処理
void Boss::Move()
{
    // 前方向ベクトル
    vec3 defMov = vec3(0, 0, -1);
    //mov = myQuaternion * defMov;

    // 移動処理
    pos += (mov * speed) + jumpMove;

    // 重力処理
    jumpMove -= vec3(0, 0.001, 0);

    // 地面との接触
    if (pos.y < (BOSS_RADIUS / 4.0))
    {
        pos.y = (BOSS_RADIUS / 4.0);
        jumpMove = vec3(0, 0, 0);
    }

}


// 表示処理
void Boss::Disp()
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
void Boss::CommonHit()
{
    life--;

    if (life <= 0)
    {
        // 表示フラグOFF
        this->dispFlag = false;

        // 撃破フラグON
        gameMainSysObj->bossKillFlag = true;
    }

}
