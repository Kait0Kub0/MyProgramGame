#include "Player.h"
#include "BulletSys.h"
#include "GameMainSys.h"
#include "Map.h"
#include "EnemySys.h";
#include "BulletSys.h"

// キーテーブル
unsigned char keyTable[2][9] =
{
    { 'A',    'D',      'W',    'S',      VK_LSHIFT, 'F', 'Q', 'E',VK_SPACE},
    { VK_LEFT, VK_RIGHT, VK_UP  ,VK_DOWN, VK_RSHIFT},
};

// プレイヤー初期ポジション
vec3 defPlayPos[2] =
{
    vec3( 5,0,0),
    vec3(-5,0,0)
};

// コンストラクタ
Player::Player(GameMainSys* gms)
{
    // ゲームメインシステム設定
    gameMainSysObj = gms;

    // 位置情報
    pos = vec3(0, 0, 0);
    // 回転情報
    rot = vec3(0, 0, 0);

    // データフラグ
    dataFlag = false;
    // 表示フラグ
    dispFlag = false;
    // ロックオンフラグ
    lockOnFlag = false;

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

    enemySysObj = nullptr;
    lockOnNo = -1;
}


// デストラクタ
Player::~Player()
{

}

// エネミーシステム
void Player::SetEnemySys(EnemySys* es)
{
    // エラーチェック
    if (es == nullptr)
    {
        cout << "Player::SetEnemySysの引数が不正です" << endl;
        return;
    }

    enemySysObj = es;

    // エネミーベース作成
    enemyMax = enemySysObj->GetEnemyMax();
}

void Player::SetEnemy(Base* bs)
{
    // エラーチェック
    if (bs == nullptr)
    {
        cout << "Player::SetEnemyの引数が不正です。" << endl;
        return;
    }

    // アドレス登録
    int size = enemyBase.size();
    enemyBase.resize(size + 1);
    enemyBase[size] = bs;
}


// プレイヤー番号
void Player::SetPlayerNo(int pno)
{
    // エラー処理
    if (pno < 0 || pno >= 2)
    {
        cout << "Player::SetPlayerNoの引数が不正です" << endl;
        return;
    }

    playerNo = pno;
}


// マップデータセット
void Player::SetMap(Map* map)
{
    // エラーチェック
    if (map == nullptr)
    {
        cout << "Player::SetMapの引数が不正です" << endl;
        return;
    }

    mapObj = map;
}

void Player::SetBulletSys(BulletSys* bs)
{
    // エラーチェック
    if (bs == nullptr)
    {
        cout << "Player::SetBulletSysの引数が不正です" << endl;
        return;
    }

    bulletSysObj = bs;
}


// 初期化
void Player::Init(bool dispFlag)
{
    this->dispFlag = dispFlag;

    // スタートポジションセット
    pos = defPlayPos[playerNo];

    rot = vec3(0, 0, 0);
    mov = vec3(0, 0, 0);
    scale = vec3(1, 1, 1);
    jumpMove = vec3(0, 0, 0);
    speed = PLAYER_SPEED_MAX;
    radius = 0.8f;
    alpha = 1.0f;

    // ロックオンフラグ
    lockOnFlag = false;

    enemySysObj = nullptr;
    lockOnNo = -1;

    // 中心座標
    centerPos = vec3(0, radius, 0);

    // クォータニオンの初期化
    vec3 eulerAngles(radians(rot.x), radians(rot.y), radians(rot.z));
    myQuaternion = quat(eulerAngles);
    q1 = myQuaternion; // 平行回転
    q2 = myQuaternion; // ジャンプ時の回転

    // 現在時間取得
    shotTimeStart = timeGetTime();
}
void Player::Init(GLuint texNo, int mdNo, int shaderNo)
{
    // モデルデータの設定
    this->modelNo = mdNo;

    // テクスチャ番号保存
    this->texNo = texNo;

    // シェーダー番号
    this->shaderNo = shaderNo;

    // ステータス初期化
    Init(true);

}


// キー入力
void Player::KeyIn()
{
    short keyData;       // キー情報
    unsigned char keyNo; // キー番号
    float workTime;

    // 入力フラグ
    keyInFlag = false;

    keyMov = vec3(0, 0, 0);
    rot = vec3(0, 0, 0);

    // スピードアップ
    keyNo = keyTable[playerNo][2];
    keyData = GetAsyncKeyState(keyNo);
    if (keyData & 0x8000)
    {
        keyMov = vec3(0, 0, -1);
    }

    // スピードダウン
    keyNo = keyTable[playerNo][3];
    keyData = GetAsyncKeyState(keyNo);
    if (keyData & 0x8000)
    {
        keyMov = vec3(0, 0, 1);
    }        

    // 左移動
    keyNo = keyTable[playerNo][0];
    keyData = GetAsyncKeyState(keyNo);
    if (keyData & 0x8000)
    {
        keyMov = vec3(-1, 0, 0);
    }

    // 右移動
    keyNo = keyTable[playerNo][1];
    keyData = GetAsyncKeyState(keyNo);
    if (keyData & 0x8000)
    {
        keyMov = vec3(1, 0, 0);
    }

    // 左回転
    keyNo = keyTable[playerNo][6];
    keyData = GetAsyncKeyState(keyNo);
    if (keyData & 0x8000)
    {
        rot.y += 2.5f;
    }
    // 右回転
    keyNo = keyTable[playerNo][7];
    keyData = GetAsyncKeyState(keyNo);
    if (keyData & 0x8000)
    {
        rot.y -= 2.5f;
    }

    // ジャンプ
    keyNo = keyTable[playerNo][4];
    keyData = GetAsyncKeyState(keyNo);
    if (keyData & 0x8000)
    {
        // 連打禁止処理
        if (keyFlag[keyNo] == false)
        {
            keyFlag[keyNo] = true;

            // ジャンプ中のチェック
            if (jumpFlag == false)
            {
                // フラグを ON 
                jumpFlag = true;

                // ジャンプベクトル設定
                jumpMove = vec3(0, PLAYER_SPEED_JUMP, 0);

                // 方向変換
                keyInFlag = true;
            }
        }
    }
    else
    {
        keyFlag[keyNo] = false;
    }

    // ロックオン
    keyNo = keyTable[playerNo][5];
    keyData = GetAsyncKeyState(keyNo);
    if (keyData & 0x8000)
    {
        // 連打禁止処理
        if (keyFlag[keyNo] == false)
        {
            keyFlag[keyNo] = true;
            lockOnFlag = !lockOnFlag;
            this->LockSearch();
        }
    }
    else
    {
        keyFlag[keyNo] = false;
    }

    // 射撃
    keyNo = keyTable[playerNo][8];
    keyData = GetAsyncKeyState(keyNo);
    if (keyData & 0x8000)
    {
        // 現在時間取得
        int workTime = timeGetTime();
        if (workTime - shotTimeStart > PLAYER_RELOAD_TIME)
        {
            vec3 workVec = vec3(0);
            if (lockOnFlag == true)
            {
                workVec = glm::normalize(enemyBase[lockOnNo]->pos - this->pos);
            }

            bulletSysObj->Shot(this, workVec);
            shotTimeStart = workTime;
        }
    }
}

// キー入力デバッグ用
void Player::KeyIn_Debug()
{
    bool keyInFlag = false;
    short keyData;  // キー情報

    mov = vec3(0, 0, 0);

    keyData = GetAsyncKeyState('W');
    if (keyData & 0x8000)
    {
        keyInFlag = true;
        this->pos += vec3(0, 0, -0.1f);
    }

    keyData = GetAsyncKeyState('S');
    if (keyData & 0x8000)
    {
        keyInFlag = true;
        this->pos += vec3(0, 0, 0.1f);
    }


    keyData = GetAsyncKeyState('A');
    if (keyData & 0x8000)
    {
        keyInFlag = true;
        
        // 視点の遅延書き込み
        
        
        this->pos += vec3(-0.1f, 0, 0);
    }

    keyData = GetAsyncKeyState('D');
    if (keyData & 0x8000)
    {
        keyInFlag = true;

        // 視点の遅延書き込み


        this->pos += vec3(0.1f, 0, 0);
    }


    // ジャンプ処理(押しっぱなし禁止)
    keyData = GetAsyncKeyState(VK_SPACE);
    if (keyData & 0x8000)
    {
        // キーフラグのチェック
        if (keyFlag[VK_SPACE] == false
         && jumpFlag == false)
        {
            keyInFlag = true;

            // フラグを ON 
            keyFlag[VK_SPACE] = true;
            jumpFlag = true;

            // ジャンプベクトル設定
            jumpMove = vec3(0, 0.1f, 0);
        }
    }
    else
    {
        keyFlag[VK_SPACE] = false;
    }
}

// ロックオン対象サーチ
void Player::LockSearch()
{
    // ロックオン対象初期化
    lockOnNo = -1;

    // 仮数値設定
    float workLength = 10000.0f;

    // プレイヤーの前方向計算
    vec3 defMov = vec3(0, 0, -1);
    vec3 pmov = myQuaternion * defMov;
    // プレイヤー座標から高さ要素をなくす
    vec3 workPos = this->pos;
    workPos.y = 0;

    // エネミーの数分ループ
    for (int i = 0; i < enemyMax; i++)
    {
        // 生存チェック
        if (enemyBase[i]->dispFlag == false)
        {
            continue;   // ループの先頭へ戻る
        }

        // 距離計算
        float length = glm::length(enemyBase[i]->pos - workPos);

        // 距離チェック
        if (length < workLength)
        {
            // 方向ベクトル
            vec3 rvec = enemyBase[i]->pos - workPos;
            // 単位ベクトルに変換
            rvec = glm::normalize(rvec);
            // 内積計算
            float dot = glm::dot(pmov, rvec);
            // 誤差修正(-1から1)
            dot = glm::clamp(dot, -1.0f, 1.0f);
            // 角度に戻す
            float dotRot = acos(dot);
            // 度数に戻す
            dotRot = glm::degrees(dotRot);

            // 角度チェック
            if (dotRot < LOCK_ON_DEGREE)
            {
                // 対象更新
                workLength = length;
                lockOnNo = i;
            }
        }
    }
    // 対象がいなければ
    if (lockOnNo == -1)
    {
        // ロックオンしない
        lockOnFlag = false;
    }
}

// ロックオン
void Player::LockOn()
{
    // フラグチェック
    if (lockOnFlag == false)
    {
        q2 = quat(vec3(0));
        return;
    }

    // 横回転
    // プレイヤーの前方向計算
    vec3 defMov = vec3(0, 0, -1);
    vec3 pmov = q1 * defMov;
    // プレイヤー座標から高さ要素をなくす
    vec3 workPos = this->pos;
    workPos.y = 0;

    // 方向ベクトル
    vec3 dvec = enemyBase[lockOnNo]->pos - workPos;
    // 単位ベクトルに変換
    dvec = glm::normalize(dvec);
    // 内積計算
    float dot = glm::dot(pmov, dvec);
    // 誤差修正
    dot = glm::clamp(dot, -1.0f, 1.0f);
    // ラディアンに変換
    float rad = acos(dot);
    // 度数に変換
    float deg = glm::degrees(rad) + rot.y;
    //// 回転量調整
    float rotMax = deg / PLAYER_ROT_MAX;
     // 外積計算
    vec3 crossVec = glm::cross(pmov, dvec);
    if (crossVec.y < 0)
    {
        rotMax *= -1.0f;
    }
    rot.y += rotMax;

    // 縦回転
    vec3 pePos = enemyBase[lockOnNo]->pos;
    pePos.y = this->pos.y;
    vec3 workVec = glm::normalize(pePos - this->pos);

    vec3 q2Vec = enemyBase[lockOnNo]->pos - this->pos;
    q2Vec = glm::normalize(q2Vec);
    dot = glm::dot(workVec, q2Vec);
    dot = glm::clamp(dot, -1.0f, 1.0f);
    rad = acos(dot);
    crossVec = glm::cross(pmov, q2Vec);
    crossVec = glm::normalize(crossVec);
    q2 = glm::angleAxis(rad, crossVec);
}


// マップの当たり判定
void Player::HitMap()
{

}



// 移動処理
void Player::Move()
{
    // 前方向ベクトル
    vec3 defMov = vec3(0, 0, -1);

    // Y軸回転クォータニオン
    vec3 rotAngle = vec3(0, glm::radians(rot.y), 0);
    quat workQuat = quat(rotAngle);
    q1 = workQuat * q1;

    keyMov = q1 * keyMov;
    keyMov.y = 0;

    // 移動処理
    vec3 pmov = (keyMov * speed) + jumpMove;
    pos += pmov;

    myQuaternion = q2 * q1;

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

    // ジャンプ中のチェック
    if (jumpFlag == true)
    {
        keyInFlag = true;
    }

    // 外周チェック
    //if ( pos.x < -mapSizeHalf.x || pos.x > mapSizeHalf.x
    //  || pos.z < -mapSizeHalf.z || pos.z > mapSizeHalf.z )
    //{
    //    this->dispFlag = false;
    //}
}


// 表示処理
void Player::Disp()
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

    //vec3 EulerAngles(radians(rot.x), radians(rot.y), radians(rot.z));
    //myQuaternion = quat(EulerAngles);

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
void Player::CommonHit()
{
    // 表示フラグOFF
    this->dispFlag = false;

    // プレイヤー残機マイナス
    gameMainSysObj->playerCount--;
}
