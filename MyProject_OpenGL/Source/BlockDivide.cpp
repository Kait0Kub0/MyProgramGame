#include "BlockDivide.h"


// ブロック内移動データ
ivec3 defBlockMov[27] =
{
    {-1, 0, -1},    // 左前
    { 0, 0, -1},    // 前
    { 1, 0, -1},    // 右前
    {-1, 0,  0},    // 左
    { 0, 0,  0},    // 中央
    { 1, 0,  0},    // 右
    {-1, 0,  1},    // 左後ろ
    { 0, 0,  1},    // 後ろ
    { 1, 0,  1},    // 右後ろ

    {-1, -1, -1},    // 左前
    { 0, -1, -1},    // 前
    { 1, -1, -1},    // 右前
    {-1, -1,  0},    // 左
    { 0, -1,  0},    // 中央
    { 1, -1,  0},    // 右
    {-1, -1,  1},    // 左後ろ
    { 0, -1,  1},    // 後ろ
    { 1, -1,  1},    // 右後ろ

    {-1, 1, -1},    // 左前
    { 0, 1, -1},    // 前
    { 1, 1, -1},    // 右前
    {-1, 1,  0},    // 左
    { 0, 1,  0},    // 中央
    { 1, 1,  0},    // 右
    {-1, 1,  1},    // 左後ろ
    { 0, 1,  1},    // 後ろ
    { 1, 1,  1},    // 右後ろ
};




void BlockDivide::Init()
{
    // サイズ計算
    if (divideCount.x == 0
        || divideCount.y == 0
        || divideCount.z == 0)
    {
        return;
    }

    // 半分のサイズ
    halfSize = size / 2.0f;

    // ブロックサイズ
    blockSize.x = size.x / divideCount.x;
    blockSize.y = size.y / divideCount.y;
    blockSize.z = size.z / divideCount.z;

    // 領域確保
    blockMax = divideCount.x * divideCount.y * divideCount.z;
    block = new BlockPoint[blockMax];
}


// ポジション計算(ブロック番号の計算)
bool BlockDivide::AroundBlockNo(ivec3 pos)
{
    // ポジション計算
    blockPos = pos;

    // 周囲９マスのポジション計算
    for (int i = 0; i < 9; i++)
    {
        ivec3 workPos = blockPos + defBlockMov[i];

        // ブロック外チェック
        if (workPos.x < 0 || workPos.x >= divideCount.x
         || workPos.y < 0 || workPos.y >= divideCount.y
         || workPos.z < 0 || workPos.z >= divideCount.z)
        {
            blockNo[i] = -1;
            continue;   // ループの先頭へ戻る
        }

        // ポインタのポジション計算
        blockNo[i] = (workPos.z * (divideCount.x * divideCount.y))
                   + (workPos.y * (divideCount.x))
                   + (workPos.x);
    }



    return true;
}



// ポリゴン頂点登録
bool BlockDivide::SetPoint(vec3 fpos, vec3 bpos)
{
    // 現在のブロックを計算
    int bno = GetBlockNo(fpos);

    // エラーチェック
    if (bno == -1)
    {
        return false;
    }

    // ブロックの移動チェック
    if( bno != oldBlockNo )
    {
        if (oldBlockNo != -1)
        {
            // 移動していた場合の処理
            // 前のブロックにも登録
            BlockPoint* bp = (block + oldBlockNo);
            bp->point[bp->pointCount][0] = fpos;
            bp->point[bp->pointCount][1] = bpos;
            bp->pointCount++;
        }
    }

    // 現在のブロックへ登録
    BlockPoint* bp = (block + bno);
    bp->point[ bp->pointCount ][0] = fpos;
    bp->point[ bp->pointCount ][1] = bpos;
    bp->pointCount++;

    // ブロック番号更新
    oldBlockNo = bno;

    return true;
}


// ブロックデータの取得
BlockPoint* BlockDivide::GetBlockPoint(int blockNo)
{
    // エラーチェック
    if (blockNo < 0 || blockNo >= blockMax)
    {
        return nullptr;
    }

    BlockPoint* bp = (block + blockNo);
    return bp;
}


// ブロック番号取得
int BlockDivide::GetBlockNo(vec3 pos)
{
    // 範囲チェック
    if (pos.x > halfSize.x || pos.x < -halfSize.x
     || pos.y > halfSize.y || pos.y < -halfSize.y
     || pos.z > halfSize.z || pos.z < -halfSize.z)
    {
        return -1;
    }

    // マップの半径を足す(オフセット計算)
    pos += halfSize;

    // ポジション計算
    blockPos = ivec3(
        (int)(pos.x / blockSize.x),
        (int)(pos.y / blockSize.y),
        (int)(pos.z / blockSize.z)
    );

    // ポジション計算
    ivec3 workPos = blockPos;

    // ブロック外チェック
    if (workPos.x < 0 || workPos.x >= divideCount.x
     || workPos.y < 0 || workPos.y >= divideCount.y
     || workPos.z < 0 || workPos.z >= divideCount.z)
    {
        return -1;
    }

    // ポインタのポジション計算
    int bno = (workPos.z * (divideCount.x * divideCount.y))
            + (workPos.y * (divideCount.x))
            + (workPos.x);

    // 周囲9ブロック取得
    AroundBlockNo(workPos);

    return bno;
}


vec3 BlockDivide::GetBlockCenter(int blockNo)
{
    // ブロック座標計算
    ivec3 bpos = ivec3(blockNo % divideCount.x, 0, blockNo / divideCount.z);
    // ３Ｄ座標へ変換
    vec3 centerPos = vec3(blockSize.x * bpos.x, 0, blockSize.z * bpos.z);
    // 中央座標計算
    centerPos.x += blockSize.x / 2.0f;
    centerPos.z += blockSize.z / 2.0f;

    return centerPos;
}


// デバッグ表示
void BlockDivide::BlockDisp()
{

    glDisable(GL_TEXTURE);
    glDisable(GL_LIGHTING);

    shaderObj->ShaderOff();

    glLineWidth(5.0f);
    glPointSize(3.0f);

    // 枠の表示
    // XLine の表示
    glColor4f(0, 1, 0, 1);
    for (int xLoop = 0; xLoop < divideCount.x+1; xLoop++)
    {
        float xPos = halfSize.x - (blockSize.x * xLoop);

        glBegin(GL_LINES);
            glVertex3f(xPos, 0.1f,  halfSize.z);
            glVertex3f(xPos, 0.1f, -halfSize.z);
        glEnd();

    }


    // ZLine の表示
    for (int zLoop = 0; zLoop < divideCount.z + 1; zLoop++)
    {
        float zPos = halfSize.z - (blockSize.z * zLoop);

        glBegin(GL_LINES);
            glVertex3f( halfSize.x, 0.1f, zPos );
            glVertex3f(-halfSize.x, 0.1f, zPos );
        glEnd();
    }


    // 色設定
    const int cMax = 3;
    vec4 pColor[cMax] =
    {
        {1,0,0,1},
        {0,1,0,1},
        {0,0,1,1},
    };

    int cNo = 0;

    // ポイント表示
    for (int bLoop = 0; bLoop < blockMax; bLoop++)
    {
        glColor4fv((float*)&pColor[cNo]);
        cNo = (cNo + 1) % cMax;

        // ブロック内のポイント表示
        BlockPoint* bp = (this->block + bLoop);
        for (int pLoop = 0; pLoop < bp->pointCount ; pLoop++)
        {
            glBegin(GL_POINTS);
                glVertex3fv( (float *)(bp->point + pLoop) );
            glEnd();
        }

    }



    glEnable(GL_TEXTURE);
    glEnable(GL_LIGHTING);

}


// ブロックを４分割した時のポイント数をカウント
//                                  0 or 5      0 or 5    
int BlockDivide::PointCount5x5(int offsetX, int offsetY )
{
    int pointCount = 0;

    // ２重ループ
    for (int yloop = 0; yloop < 5; yloop++)
    {
        for (int xloop = 0; xloop < 5; xloop++)
        {
            //cout << ((yloop + offsetY) * 10) + xloop + offsetX << ",";        
            int bno = ((yloop + offsetY) * 10) + xloop + offsetX;
            BlockPoint* bp = (this->block + bno);
            pointCount += bp->pointCount;
        }
    }
    //cout << pointCount << " : ";
    return pointCount;
}