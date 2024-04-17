#pragma once

#include "system.h"
#include "Base.h"

// マップのポリゴンサイズ -10, 10
#define MAP_SCALE_X 2
#define MAP_SCALE_Y 2
#define MAP_SCALE_Z 2
#define DEF_MAP_SIZE 20.0f

class Map : public Base {
private:
    // データフラグ
    bool dataFlag = false;

    // テクスチャ番号
    int sphereTexNo;
    int groundTexNo;
    // モデルデータ番号
    int sphereModelNo;
    int groundModelNo;

    // 外周サイズ
    vec3 mapSize;

    // プレイヤー情報
    Base* playerBase;

    // マップの座標
    vec3 spherePos;

public:

    // コンストラクタ
    Map();
    // デストラクタ
    ~Map();

    // 初期化
    void Init();
    void SphereInit(GLuint texNo, int mdNo, int shaderNo);
    void GroundInit(GLuint texNo, int mdNo, int shaderNo);

    // マップサイズ取得
    void SetMapScale(vec3 scale);
    void SetPlayer(Base* player);
    vec3 GetMapSize();

    // 移動処理
    void Move();

    // 表示処理
    void Disp();
    void SphereDisp();
    void GroundDisp();
};
