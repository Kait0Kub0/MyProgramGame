#include "CharPol.h"

// コンストラクタ
CharPol::CharPol()
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
CharPol::~CharPol()
{

}



// 初期化
void CharPol::Init()
{
    dispFlag = true;
    pos = vec3(0, 0, 0);
    rot = vec3(0, 0, 0);
    mov = vec3(0, 0, 0);
    scale = vec3(1, 1, 1);
    jumpMove = vec3(0, 0, 0);
    speed = 0.1f;
    radius = 1.0f;
    alpha = 1.0f;

    // クォータニオンの初期化
    vec3 eulerAngles(radians(rot.x), radians(rot.y), radians(rot.z));
    myQuaternion = quat(eulerAngles);

}
void CharPol::Init(GLuint texNo, int mdNo, int shaderNo)
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


// キー入力
void CharPol::KeyIn()
{
    short keyData;  // キー情報

    mov = vec3(0, 0, 0);

    keyData = GetAsyncKeyState( 'W' );
    if (keyData & 0x8000)
    {
        mov = vec3(0, 0, -1);
    }

    keyData = GetAsyncKeyState('S');
    if (keyData & 0x8000)
    {
        mov = vec3(0, 0, 1);
    }


    keyData = GetAsyncKeyState('A');
    if (keyData & 0x8000)
    {
        rot += vec3(0, 2.5, 0);
    }

    keyData = GetAsyncKeyState('D');
    if (keyData & 0x8000)
    {
        rot += vec3(0, -2.5, 0);
    }

    // ジャンプ処理
    keyData = GetAsyncKeyState(VK_SPACE);
    if (keyData & 0x8000)
    {
        jumpMove = vec3( 0, 0.1, 0);
    }



}


// マップの当たり判定
void CharPol::HitMap()
{

}



// 移動処理
void CharPol::Move()
{
    // 前方向ベクトル
    //vec3 defMov = vec3(0, 0, -1);
    mov = myQuaternion * mov;

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
void CharPol::Disp()
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

    // 遅延処理の入れ替え
    for (int i = (DELAY_MAX-1); i > 0 ; i--)
    {
        // ひとつ上のデータをコピー
        delayQuaternion[i] = delayQuaternion[i-1];
    }

    // 最新情報上書き
    delayQuaternion[0] = myQuaternion;



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

