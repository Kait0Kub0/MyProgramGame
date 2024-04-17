#include "Bullet.h"


// コンストラクタ
Bullet::Bullet()
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
Bullet::~Bullet()
{

}

// 初期化
void Bullet::Init()
{
    dispFlag = false;
    pos = vec3(0, 0, 0);
    rot = vec3(0, 0, 0);
    mov = vec3(0, 0, 0);
    scale = vec3(1) * 0.5f;
    jumpMove = vec3(0, 0, 0);
    speed = BULLET_SPEED;
    radius = BULLET_RADIUS;
    alpha = 1.0f;

    // クォータニオンの初期化
    vec3 eulerAngles(radians(rot.x), radians(rot.y), radians(rot.z));
    myQuaternion = quat(eulerAngles);

}
void Bullet::Init(GLuint texNo, int mdNo, int shaderNo)
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


// ショット処理
void Bullet::Shot(Base* base, vec3 mov, float speedOffset)
{
    // 各データの取り出し
    // ポジション
    this->pos = base->pos;
    this->pos.y += base->radius /3;
    this->rot = base->rot;

    // スピードセット
    this->speed = BULLET_SPEED * speedOffset;

    // 移動方向
    // 前方向ベクトル
    vec3 defMov = vec3(0, 0, -1);
    // 回転クォータニオン
    this->myQuaternion = base->myQuaternion;
    quat workQuat = vec3(glm::radians(mov.x),
                         glm::radians(mov.y),
                         glm::radians(mov.z));

    this->myQuaternion *= workQuat;
    this->mov = this->myQuaternion * defMov;

    // 表示ON
    this->dispFlag = true;

    radius = 0.2;
}


// 移動処理
void Bullet::Move()
{
    // 前方向ベクトル
    // 移動処理
    pos += (mov * speed) + jumpMove;

    // 重力設定
    if (gravityFlag == true)
    {

        // 落ちるスピードの制御
        float gm = mov.y * speed;
        if (gm + jumpMove.y > -0.01)
        {
            // 重力処理
            jumpMove -= vec3(0, 0.001, 0);
        }

    }


    // 画面外処理
    //if (pos.x < -10.0f || pos.x > 10.0f
    // || pos.y < -10.0f  
    // || pos.z < -10.0f || pos.z > 10.0f)
    //{
    //    this->dispFlag = false;
    //    gravityFlag = false;
    //    jumpMove = vec3(0, 0, 0);
    //}

}

// 表示処理
void Bullet::Disp()
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
