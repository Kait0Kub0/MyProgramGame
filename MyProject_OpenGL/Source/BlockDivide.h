#pragma once

#include "Base.h"

#define BLOCK_POINT_MAX 38000


class BlockPoint
{
public:
    // ポイント数
    int pointCount = 0;
    
    vec3 point[BLOCK_POINT_MAX][2];

};


// ブロック分割処理
class BlockDivide
{
public:

    // 全体サイズ
    vec3 size;

    // 半分のサイズ
    vec3 halfSize;

    // 分割数(横・縦)
    ivec3 divideCount;

    // ブロックサイズ
    vec3 blockSize;

    // 頂点データ登録
    BlockPoint* block;
    // ブロックの数
    int blockMax = 0;

    // ブロックポジション
    ivec3 blockPos;

    // チェックブロックNo
    int blockNo[9];

    // 前のブロックNo
    int oldBlockNo = -1;

    // 初期化
    void Init();

    // ポジション計算(ブロック番号の計算)
    bool AroundBlockNo(ivec3 pos);

    // ポリゴン頂点登録
    bool SetPoint(vec3 fpos, vec3 bpos);

    // ポリゴンデータの取得
    BlockPoint* GetBlockPoint(int blockNo);

    // ブロックセンター取得
    vec3 GetBlockCenter(int blockNo);

    // ブロック番号取得
    int GetBlockNo(vec3 pos);

    // 分割領域表示
    void BlockDisp();

    // ブロックを４分割した時のポイント数をカウント
    int PointCount5x5(int offsetX, int offsetY);

};