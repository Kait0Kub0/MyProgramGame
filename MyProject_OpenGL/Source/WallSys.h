#pragma once

#include "Base.h"
#include "BlockDivide.h"

#define NonHitPoint 40

// 壁の頂点データ
class WallPoint
{
public:
    vec2    t;
    vec4    c;
    vec3    n;
    vec3    v;
};

// 2頂点 * 60フレーム * 60秒 * 5分 + 予備
#define WallPointMAX 38000

#define WALL_HEIGHT  1.0    // 壁の高さ

// フレームのスキップ値
#define SKIP_POINT  3


class WallSys : public Base, public BlockDivide
{
private:

    // テクスチャ番号
    int texNo;

    // カラー情報
    vec4 color;

    // 壁の頂点データ
    WallPoint* wallPoint;
    WallPoint* linePoint;

    // 使用中の頂点の数
    int pointCount;

    // 壁の作成スキップ
    int skipFlame;
    int skipFlameMax;

    // 当たり判定のオブジェクト
    vector<Base*> hitObj;

    // 方向データ
    vec3 targetRot;

    // 対象オブジェクト
    Base* targetObj;

    // 前のポイント
    vec3 oldPoint;

public:

    // コンストラクタ
    WallSys();

    // デストラクタ
    ~WallSys();

    // 初期化処理
    void Init(int tNo, int sNo);
    void Init(bool dispFlag);

    // テクスチャ変更
    void SetTexNo(int tNo);

    // カラー設定
    void SetColor(float r, float g, float b, float a);
    void SetColor(vec4 color);

    // 壁用ポイントセット
    void SetWallPoint(vec3 point);

    // 頂点の追加処理(足元の座標)
    bool SetPoint(vec3 point, bool keyFlag);

    // ヒットオブジェクト登録
    void SetHitObj(Base* base);

    // ターゲットオブジェクトのセット
    void SetTarget(Base* tb);

    // ヒットチェック
    void HitCheck();
    bool PointHitObj(Base* hitObj, BlockPoint *bp);
    bool WallHitCheck( vec3 sPos, vec3 ePos );

    // 表示処理
    void Disp();
};

