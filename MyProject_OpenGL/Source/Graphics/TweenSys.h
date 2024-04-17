#pragma once

#include "system.h"


// 扱うデータの種類
enum class TweenMode
{
    Float,
    Vec2,
    Vec3,
    MAX
};


// Tweenのコマンド
// 初期化（初期位置へ戻す）
// 最終位置（最終位置へ移動）
// 時間指定位置（指定された時間の位置へ移動）
// データ削除
enum class TweenCommand
{
    Init,       // 初期化（初期位置へ戻す）
    EndPara,    // 最終位置（最終位置へ移動）
    TimePara,   // 時間指定位置（指定された時間の位置へ移動）
    Delete,     // データ削除
};


enum class TweenFlag
{
    LoopFlag,       // ループ ON/OFF
    PlayFlag,       // 再生 ON/OFF
    AbsoluteFlag,   // 絶対座標 ON(絶対座標)/OFF(相対座標)
};


// ベースプログラム（データ管理）
class TweenBase
{
private:
public:
    // Tweenの種類  0:flaot  1:vec2  2:vec3
    TweenMode mode;

    // TweenNo (検索に使用)
    int no = -1;

    // 移動データ
    void* pos;                  // float vec2 vec3
    void* orgPos;               // float vec2 vec3 (相対座標用)
    std::vector<void*> list;    // float vec2 vec3

    // 変化座標の数
    int maxCount = 0;
    int count = 0;

    // 時間管理
    float maxTime = 0;
    float countTime = 0;

    // 距離
    float distance = 0;

    // 再生フラグ
    bool playFlag = false;

    // ループフラグ
    bool loopFlag = false;

    // 移動終了フラグ
    bool moveFlag = false;

    // 絶対座標・相対座標フラグ
    bool absoluteFlag = true;

};


// Tween全体管理
class Tween
{
private:
    int tweenCount = 0;
public:
    std::vector<TweenBase*>tbaseList;

    // コンストラクタ
    Tween();

    // デストラクタ
    ~Tween();


    // データセット float vec2 vec3
    int SetTween(float* pos, float* movPos, int count,
        float time, bool loopFlag = false, bool dispFlag = true);

    int SetTween(vec2* pos, vec2* movPos, int count,
        float time, bool loopFlag = false, bool dispFlag = true);

    int SetTween(vec3* pos, vec3* movPos, int count,
        float time, bool loopFlag = false, bool dispFlag = true);


    // フラグセット
    void SetFlag();


    // 情報の取得
    // 管理番号の取得
    int GetTweenNo(void* pos);

    // 移動終了取得
    bool GetMoveFlag(void* pos);
    bool GetMoveFlag(int no);

    // 再生フラグ取得
    bool GetPlayFlag(void* pos);
    bool GetPlayFlag(int no);

    // コマンド設定
    bool SetCommand(TweenCommand tc, int no, float time = 0);


    // 再生処理
    void TweenPlay();           // 全データの再生
    void TweenPlay(void* pos);  // 指定データの再生
    void TweenPlay(int no);     // 指定データの再生

private:
    float FloatPlay(TweenBase *tb);  // float 再生
    vec2  Vec2Play(TweenBase* tb);   // vec2 再生
    vec3  Vec3Play(TweenBase* tb);   // vec3 再生


    TweenBase* CreateTbase(void* pos);  // TweenBase の作成

    int AddressCheck(void* pos);    // 同一アドレスチェック
    int AddressCheck(int no);       // 同一管理番号チェック

};



// 実体へのアクセス
extern Tween* tweenObj;
