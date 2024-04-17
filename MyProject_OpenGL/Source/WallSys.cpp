
#include "WallSys.h"
#include "Hitchk.h"

// コンストラクタ
WallSys::WallSys()
{
    // テクスチャ番号
    this->texNo = 0;

    // カラー情報
    this->color = vec4(1,1,1,1);

    // 壁の頂点データ
    this->wallPoint = new WallPoint[WallPointMAX];
    this->linePoint = new WallPoint[WallPointMAX];

    // 使用中の頂点の数
    this->pointCount = 0;

    // ブロックデータ初期化
    BlockDivide::size        = vec3(40, 3, 40);
    BlockDivide::divideCount = ivec3(10, 1, 10);
    BlockDivide::Init();

}


// デストラクタ
WallSys::~WallSys()
{
    // 壁データの開放
    if (this->wallPoint != nullptr)
    {
        delete[] this->wallPoint;
        this->wallPoint = nullptr;
    }

    // ラインデータ開放
    if (this->linePoint != nullptr)
    {
        delete[] this->linePoint;
        this->linePoint = nullptr;
    }

}


// 初期化処理
void WallSys::Init(int tNo, int sNo)
{
    // テクスチャ番号セット
    this->texNo = tNo;
    // シェーダー番号セット
    this->shaderNo = sNo;
}
void WallSys::Init(bool dispFlag)
{
    // ポイントカウント初期化
    pointCount = 0;

    // スキップポイント初期化
    skipFlame = 0;
    skipFlameMax = SKIP_POINT;

    // 表示フラグセット
    this->dispFlag = dispFlag;

    // ヒットオブジェクトの初期化
    hitObj.clear();


    // ブロックデータの初期化
    for (int bLoop = 0; bLoop < blockMax; bLoop++)
    {
        BlockPoint* bp = (this->block + bLoop);
        bp->pointCount = 0;
    }


}

// テクスチャ変更
void WallSys::SetTexNo(int tNo)
{
    // テクスチャ番号：（-1:テクスチャOFF）
    this->texNo = tNo;
}

// カラー設定
void WallSys::SetColor(float r, float g, float b, float a)
{
    this->color = vec4(r, g, b, a);
}
void WallSys::SetColor(vec4 color)
{
    this->color = color;
}


// スタートポイントセット
void WallSys::SetWallPoint(vec3 point)
{
    // テクスチャ座標ベース
    vec2 texPos[2] = { {0,0}, {0,1} };
    float texS = 1.0f;

    // ポリゴン面の S 座標を一つ飛ばしで設定する
    if ((pointCount % 4) == 0)
    {
        texS = 0.0f;
    }


    // ポイントアドレス計算
    int pc = this->pointCount - 2; 

    for (int i = 0; i < 2; i++)
    {
        // ポインタのアドレス計算
        WallPoint* wp = (this->wallPoint + pc);

        // 頂点データセット
        wp->v = point;
        // 高さ設定（ジャンプ処理追加）
        wp->v.y = WALL_HEIGHT * i + point.y;

        // カラー情報セット
        wp->c = this->color;

        // テクスチャ情報セット
        wp->t = texPos[i];
        wp->t.s = texS;

        // ポイントカウントアップ
        pc++;
    }


    // ラインの追加
    // ポインタのアドレス計算
    pc = this->pointCount - 2;
    int addPoint = (pc / 2);
    WallPoint* lp = (this->linePoint + addPoint);

    // 頂点データセット
    lp->v = point;
    // 高さ設定
    lp->v.y = WALL_HEIGHT + point.y;

    // カラー情報セット
    lp->c = this->color;
    lp->c.a = 1.0f;
}


// 頂点の追加処理(足元の座標)
// point:追加座標
// keyFlag:方向変更フラグ
bool WallSys::SetPoint(vec3 point, bool keyFlag)
{

    // 方向変換のチェック
    if (keyFlag == true)
    {
        pointCount += 2;
    }

    // 壁データ登録
    SetWallPoint(point);


    // スキップフレーム処理
    skipFlame = (skipFlame + 1) % skipFlameMax;
    if (skipFlame != 0)
    {
        return false;
    }

    // ブロックデータの書き込み
    point.y += WALL_HEIGHT;
    vec3 fpos = point;
    vec3 bpos = oldPoint;
    BlockDivide::SetPoint(fpos, bpos);

    // 旧ポイント書き換え
    oldPoint = point;

    return true;
}


// ヒットオブジェクト登録
void WallSys::SetHitObj(Base* base)
{
    // 登録数の取得
    int size = hitObj.size();
    // サイズ変更（登録数+1）
    hitObj.resize(size + 1);
    // ベース登録
    hitObj[size] = base;
}


// ターゲットオブジェクトセット
void WallSys::SetTarget(Base* tb)
{
    // エラー処理
    if (tb == nullptr)
    {
        cout << "WallSys::SetTargetの引数にエラーがあります" << endl;
        return;
    }

    targetObj = tb;
    targetRot = tb->rot;
}


// ヒットチェック
void WallSys::HitCheck()
{
    // 登録数取得
    int size = hitObj.size();

    // 登録数ループ
    for (int i = 0; i < size; i++)
    {
        // 周囲9ブロックの取得
        int bno = GetBlockNo( hitObj[i]->pos );

        // 9 ブロック分ループ
        for (int bLoop = 0; bLoop < 9; bLoop++)
        {
            // ブロックデータの取得
            int chkNo = BlockDivide::blockNo[bLoop];
            if (chkNo == -1)
            {
                continue;
            }
            BlockPoint* bp = (BlockDivide::block + chkNo);

            // ブロックに登録された頂点と
            // オブジェクトの当たり判定
            PointHitObj( hitObj[i], bp);
        }
    }
}



// 頂点との当たり判定
bool WallSys::PointHitObj( Base *hitObj, BlockPoint* bp )
{
    // ブロックに登録された頂点の数ループ
    for (int wloop = 0; wloop < bp->pointCount; wloop++)
    {
        // 壁の座標
        vec3 pPos = (bp->point[wloop][0]);
        bool passFlag = false;

        // オブジェクトに登録された無効座標のチェック
        for (int bpLoop = 0; bpLoop < DELAY_MOVE_POINT_MAX; bpLoop++)
        {
            // ポイントチェック
            if (hitObj->delayPoint[bpLoop].x == pPos.x
             && hitObj->delayPoint[bpLoop].z == pPos.z)
            {
                passFlag = true;
                break; // bpLoop を抜ける
            }
        }

        if (passFlag == true)
        {
            continue; // wloop の先頭へ戻る
        }


        // 高さの修正
        vec3 hitPos = hitObj->pos;
        hitPos.y += hitObj->radius;

        // 円と点の当たり判定
        bool ret = Hitchk::CircleHitPoint(
            hitPos, hitObj->radius,
            pPos);
        // ヒットチェック
        if (ret == true)
        {
            // 表示OFF
            hitObj->dispFlag = false;
            break;
        }
    }

    return true;
}




// 壁と線の当たり判定
bool WallSys::WallHitCheck(vec3 sPos, vec3 ePos)
{
    int size = pointCount - 4;

    // 壁の数ループ
    for (int i = 0; i < size; i++)
    {
        // 頂点データ取り出し
        vec3 pp[3];
        for (int loop = 0; loop < 3; loop++)
        {
            pp[loop] = ( wallPoint + i + loop  )->v;
        }

        // 面と線の当たり判定
        bool ret = Hitchk::LineHitPoly(&pp[0], sPos, ePos);
        if (ret == true)
        {
            // あたり
            return ret;
        }
    }

    // ハズレ
    return false;

}



// 表示処理
void WallSys::Disp()
{
    // 表示処理チェック
    if (dispFlag == false)
    {
        return;
    }

    // シェーダー選択
    shaderObj->ShaderOn(shaderNo);
    shaderObj->SetInt("uTexFlag", 0);   // テクスチャフラグ設定


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

    // 環境設定
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);	// テクスチャ座標データ使用
    glEnableClientState(GL_COLOR_ARRAY);            // カラーデータ使用
    glDisableClientState(GL_NORMAL_ARRAY);          // 法線データ使用停止
    glEnableClientState(GL_VERTEX_ARRAY);           // 頂点座標データ使用

    // テクスチャ設定
    if (this->texNo == -1)
    {
        // テクスチャを使用しない
        glDisable(GL_TEXTURE);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else
    {
        // テクスチャを使用
        glEnable(GL_TEXTURE);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texNo);
    }

    // 線の太さ
    glLineWidth(5.0f);

    // ラインの表示
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glInterleavedArrays(GL_T2F_C4F_N3F_V3F, 0, this->linePoint);
    int lineCount = this->pointCount / 2;
    glDrawArrays(GL_LINE_STRIP, 0, lineCount);


    // 裏面非表示 OFF
    glDisable(GL_CULL_FACE);

    // 頂点表示処理
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glInterleavedArrays(GL_T2F_C4F_N3F_V3F, 0, this->wallPoint);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, this->pointCount);

    glEnable(GL_CULL_FACE);


    // マトリックスの取得
    glPopMatrix();


}


