
#include "Bar.h"
#include "Look.h"


// コンストラクタ
Bar::Bar()
{
    // 最大値
    maxGauge = 100;
    // ダメージ
    damage = 0;
    // uvOffset
    uvOffset = vec2(0,0);
    // テクスチャ番号
    texNo = -1;
    // モデル番号
    modelNo = -1;

}

// デストラクタ
Bar::~Bar()
{

}

// 視点の設定
void Bar::SetLook(Look* look)
{
    // エラーチェック
    if (look == nullptr)
    {
        cout << "Bar::SetLookで引数の値が不正です。";
        return;
    }

    lookObj = look;
}

// ゲージ最大値
void Bar::SetMaxGauge(float max)
{
    // エラーチェック
    if (max <= 0)
    {
        cout << "Bar::SetMaxGaugeで引数の値が不正です。";
        return;
    }

    maxGauge = max;
}

// ゲージダメージ
void Bar::SetDamage(float dm)
{
    damage = dm;

    // パラメータ計算
    DamageCalc();
}

// サイズ
void Bar::SetSize(float w, float h)
{
    scale.x = w;
    scale.y = h;
}

// 表示位置
void Bar::SetPos(vec3 pos)
{
    this->pos = pos;
}


// 初期化
void Bar::Init()
{
    dispFlag = true;
    damage = 0;
    // uvOffset
    uvOffset = vec2(0, 0);
    // 拡大縮小
    scale = vec3(1, 1, 1);
    // ポジション
    pos = vec3(0, 0, 0);
}
void Bar::Init(GLuint texNo, int mdNo, int shaderNo)
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


// ダメージ計算処理
void Bar::DamageCalc()
{
    // テクスチャ座標の計算
    // damage = 0 : uvOffset.s = 0 : maxGauge = 100 
    // s = 0.5 * 100 / 0 = 0;
    // damage = 100 : uvOffset.s = 0.5 : maxGauge = 100 
    // s = 0.5 * 100 / 100 = 0.5 
    // テクスチャの横座標
    this->uvOffset.s = 0.5f * (this->damage / this->maxGauge);
}


// 表示処理
void Bar::Disp()
{
    // 生存チェック
    if (dispFlag == false)
    {
        return;
    }


    // シェーダー選択
    shaderObj->ShaderOn(shaderNo);
    shaderObj->SetVec2( "uTexOffset", uvOffset );

    // ポリゴンモデルの表示
    // マトリックスの保存
    glPushMatrix();

    // ビルボード
    // 視点方向ベクトル
    vec3 lookVec = this->pos - lookObj->pos;
    // 単位ベクトル
    lookVec = glm::normalize(lookVec);
    // ビルボードクォータニオン
    quat billboardQuat =
        glm::quatLookAt(lookVec, lookObj->upVec);

    //vec3 EulerAngles(radians(rot.x), radians(rot.y), radians(rot.z));
    //myQuaternion = quat(EulerAngles);
    // 視点方向へ回転
    mat4 transform = toMat4(billboardQuat);

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
