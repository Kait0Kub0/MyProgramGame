#include "Explosion.h"


// コンストラクタ
Explosion::Explosion()
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
Explosion::~Explosion()
{

}

// 初期化
void Explosion::Init()
{
    dispFlag = false;
    pos = vec3(0, 0, 0);
    rot = vec3(0, 0, 0);
    mov = vec3(0, 0, 0);
    scale = vec3(0.1f, 0.1f, 0.1f);
    jumpMove = vec3(0, 0, 0);
    speed = 0.1f;
    radius = 1.0f;
    alpha = 1.0f;

    // クォータニオンの初期化
    vec3 eulerAngles(radians(rot.x), radians(rot.y), radians(rot.z));
    myQuaternion = quat(eulerAngles);

}
void Explosion::Init(GLuint texNo, int mdNo, int shaderNo)
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


// 爆発発生処理
void Explosion::Ignition(Base* base)
{
    // 各データの取り出し
    // ポジション
    this->pos = base->pos + base->centerPos;

    // 表示ON
    this->dispFlag = true;

    // アルファ値の設定
    this->alpha = 1.0f;

    // スケール修正
    this->scale = vec3(0.1f, 0.1f, 0.1f);

    // 爆発時間設定
    this->uTime = 0;
}


// 移動処理
void Explosion::Move()
{
    // サイズ変更
    scale *= 1.03f;

    // アルファ値の変更
    this->alpha -= 0.01f;

    // 時間設定
    this->uTime += 0.02f;

    // 表示OFFチェック
    if (this->alpha < 0)
    {
        this->dispFlag = false;
    }
}

// 表示処理
void Explosion::Disp()
{
    // 生存チェック
    if (dispFlag == false)
    {
        return;
    }

    // カリングOFF
    glDisable(GL_CULL_FACE);

    // シェーダー選択
    shaderObj->ShaderOn(shaderNo);
    shaderObj->SetFloat("u_time", uTime);

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


    // カリングON
    glEnable(GL_CULL_FACE);

}
