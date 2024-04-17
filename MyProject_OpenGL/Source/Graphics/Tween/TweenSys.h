#include "../../system.h"
#include "../../ThreadManager.h"

enum class TweenMode
{
    Float,
    Vec2,
    Vec3,
};

enum class TweenCount
{
    First,
    Middle,
    Final,
    Max
};


class TBase{
public:
    // Tweenの種類 0:float 1:vec2  2:vec3
    TweenMode mode;
    // 変化座標の数
    int maxCount = 0;
    int count = 0;

    // 時間
    float maxTime = 0;
    float time = 0;

    // 距離
    float distance = 0;

    // ループフラグ
    bool loop = false;

    // フラグ ON・OFF
    bool dispFlag = false;

    // リスト番号
    int listNo = -1;
};


class TweenFloat : public TBase {
public:
    float* pos;
    std::vector<float> floatList;
};

class TweenVec2 : public TBase {
public:
    vec2* pos;
    std::vector<vec2> vec2List;
};

class TweenVec3 : public TBase {
public:
    vec3* pos;
    std::vector<vec3> vec3List;
};


class Tween : public ThreadData {
private:
    int tweenCount;
public:
    // 強制終了用のフラグ
    //bool exitFlag = false;

    std::vector<TBase *> tbaseList;



    Tween();

    // データセット
    int SetTween(float* pos, float* movPos, int count,
        float tweenTime, bool loopFlag = false, bool dispflag = true);

    // データセット
    int SetTween(vec2* pos, vec2* movPos, int count,
        float tweenTime, bool loopFlag = false, bool dispflag = true);

    // データセット
    int SetTween(vec3* pos, vec3* movPos, int count,
        float tweenTime, bool loopFlag = false, bool dispflag = true);

    // スキップ処理
    bool SetSkip(int listNo, float time);

    // 状態取得
    bool GetTweenDisp(int listNo);


    // 移動処理(全登録データの移動)
    void TweenMove();
    // スレッド登録用移動処理
    static unsigned ThreadTweenMove(void* p);

    // float 変化
    void FlaotMov(int i);
    // ve2 変化
    void Vec2Mov(int i);
    // vec3 変化
    void Vec3Mov(int i);

    // 秒からポジション
    float FloatPos(int no, float sec);
    // 秒からポジション
    vec2 Vec2Pos(int no, float sec);
    // 秒からポジション
    vec3 Vec3Pos(int no, float sec);


    // 同一アドレスチェック
    int AddressCheck(void* pos);

    // 検索処理
    int Search(int listNo);

    // tween 位置の初期化
    bool ResetMove(int listNo); //リストNoを使用
    bool ResetMove(void* pos);  //アドレスを使用

    // tween から削除
    bool Delet(int listNo);
    bool Delet(void* pos);


};


// 実体のプロトタイプ宣言
extern Tween* tweenObj;

