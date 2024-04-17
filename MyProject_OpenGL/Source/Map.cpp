
#include "Map.h"


// コンストラクタ
Map::Map()
{
    // データフラグ
    dataFlag = false;

    // テクスチャ番号
    sphereTexNo = 0;
    groundTexNo = 0;
    // モデルデータ
    sphereModelNo = 0;
    groundModelNo = 0;
}

// デストラクタ
Map::~Map()
{
    // 今回は見せ場なし
}

// 初期化
void Map::Init()
{
    dispFlag = true;
    pos = vec3(0, 0, 0);
    rot = vec3(0, 0, 0);
    mov = vec3(0, 0, 0);
    scale = vec3(MAP_SCALE_X, MAP_SCALE_Y, MAP_SCALE_Z);
    jumpMove = vec3(0, 0, 0);
    speed = 0;
    radius = 1.0f;
    alpha = 1.0f;
    spherePos = vec3(0, 0, 0);

    // クォータニオンの初期化
    vec3 eulerAngles(radians(rot.x), radians(rot.y), radians(rot.z));
    myQuaternion = quat(eulerAngles);

    // 外周サイズ計算
    mapSize = scale * DEF_MAP_SIZE;

}

void Map::SphereInit(GLuint texNo, int mdNo, int shaderNo)
{
    // モデルデータの設定
    this->sphereModelNo = mdNo;

    // テクスチャ番号保存
    this->sphereTexNo = texNo;

    // シェーダー番号
    this->shaderNo = shaderNo;
}

void Map::GroundInit(GLuint texNo, int mdNo, int shaderNo)
{
    // モデルデータの設定
    this->groundModelNo = mdNo;

    // テクスチャ番号保存
    this->groundTexNo = texNo;

    // シェーダー番号
    this->shaderNo = shaderNo;
}


void Map::SetMapScale(vec3 scale)
{
    // エラーチェック
    if (scale.x <= 0 || scale.z <= 0 )
    {
        scale = vec3(1, 1, 1);
    }

    // スケール値の設定
    this->scale = scale;

    // マップサイズの計算
    this->mapSize = scale * DEF_MAP_SIZE;
}

void Map::SetPlayer(Base* player)
{
    // エラーチェック
    if (player == nullptr)
    {
        cout << "Map::SetPlayerの引数が不正です" << endl;
        return;
    }

    playerBase = player;
}

// マップサイズ取得
vec3 Map::GetMapSize()
{
    return mapSize;
}


// 移動処理
void Map::Move()
{
    // プレイヤーのポジションセット
    spherePos = playerBase->pos;
    // 高さ変更は無視
    spherePos.y = 0;
}

void Map::Disp()
{
    // 地面の表示処理
    GroundDisp();
    // 天球の表示処理
    SphereDisp();
}

// 天球表示処理
void Map::SphereDisp()
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
    transform[3][0] = spherePos.x;
    transform[3][1] = spherePos.y;
    transform[3][2] = spherePos.z;

    glMultMatrixf((float*)&transform);

    glScalef(scale.x, scale.y, scale.z);

    // モデル表示
    ModelData* modelData = modelSysObj->GetModelData(sphereModelNo);
    // テクスチャセット
    modelData->textureNo = sphereTexNo;
    // POLオブジェクト表示
    ModelDataDisp(modelData);


    // マトリックスの取得
    glPopMatrix();
}
// 地面表示処理
void Map::GroundDisp()
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
    ModelData* modelData = modelSysObj->GetModelData(groundModelNo);
    // テクスチャセット
    modelData->textureNo = groundTexNo;
    // POLオブジェクト表示
    ModelDataDisp(modelData);

    // マトリックスの取得
    glPopMatrix();
}

