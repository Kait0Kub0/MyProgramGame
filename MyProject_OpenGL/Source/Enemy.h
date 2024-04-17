#pragma once

#include "system.h"
#include "Base.h"

enum class EnemyMode
{
    Non,
    Right,
    Left,
    Jump,
    Sup,
    Sdown,
    MAX
};




// 前方宣言
class GameMainSys;
class WallSys;
class Map;


class Enemy : public Base
{
private:
    // データフラグ
    bool dataFlag = false;

    // テクスチャ番号
    int texNo;
    // モデルデータ
    int modelNo;

    // キー情報保存
    bool keyFlag[256];

    // ゲームメインシステム
    GameMainSys* gameMainSysObj;

    // マップ情報
    Map* mapObj = nullptr;

    // AIモード
    EnemyMode mode;

    // 移動距離
    float movDistance = 0;

    // 外周座標
    vec3 leftPos;
    float mapWidth;
    float mapHeight;

    // 方向変換のフラグ
    bool keyInFlag = false;

    // プレイヤーオブジェクト
    Base* playerObj;

public:

    // コンストラクタ
    Enemy();

    // デストラクタ
    ~Enemy();

    // アクセサーメソッド（セッター・ゲッター）
    void SetMap(Map* map);
    void SetGameMainSys(GameMainSys* gms);
    void SetPlayer(Base* base);

    // 初期化
    void Init();
    void Init(GLuint texNo, int mdNo, int shaderNo);

    // キー入力
    void KeyIn();

    // マップの当たり判定
    bool HitMap( vec3 pos );

    // 移動処理
    void Move();

    // AI処理
    void Ai();

    // 思考処理
    void Think();

    // 表示処理
    void Disp();

    // 当たり判定で使用する共有処理
    virtual void CommonHit();

};


