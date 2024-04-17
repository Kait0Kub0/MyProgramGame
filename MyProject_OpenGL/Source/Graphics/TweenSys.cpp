
#include "TweenSys.h"

Tween *tweenObj = new Tween();

// コンストラクタ
Tween::Tween()
{
    tweenCount = 0;
}

// デストラクタ
Tween::~Tween()
{

}


// データセット float vec2 vec3
int Tween::SetTween(float* pos, float* movPos, int maxPos,
    float time, bool loopFlag, bool playFlag)
{
    // TweenBase の作成
    // 同一アドレスの再利用（二重登録禁止）
    TweenBase* tweenBase = CreateTbase((void*)pos);

    // パラメータセット
    tweenBase->mode = TweenMode::Float;     // モードセット
    tweenBase->playFlag = playFlag;         // 再生フラグセット
    tweenBase->loopFlag = loopFlag;         // ループフラグ
    tweenBase->moveFlag = true;             // 移動フラグ（移動終了：false）
    tweenBase->maxTime = time;              // 時間セット
    tweenBase->countTime = 0;               // 時間カウンタ初期化

    // サイズ変更
    // maxCount + 1 ： ループ用に戻る座標を追加
    tweenBase->maxCount = maxPos;
    tweenBase->count = 0;
    tweenBase->list.resize(maxPos + 1);

    // movPosのセット
    tweenBase->distance = 0;
    float oldPos = movPos[0];
    int i = 0;
    for (i = 0; i < maxPos; i++)
    {
        tweenBase->list[i] = (void *)&movPos[i];
        // 距離の計算
        float lengthPos = movPos[i] - oldPos;
        // 距離なのでマイナスが発生しないように変換
        tweenBase->distance += abs(lengthPos);
        // 前座標更新
        oldPos = movPos[i];
    }

    // ループ用ポジションセット
    tweenBase->list[i] = (void*)&movPos[0];

    // ループ用距離の更新
    if (loopFlag == true)
    {
        // 距離の計算
        float lengthPos = movPos[0] - oldPos;
        // 距離なのでマイナスが発生しないように変換
        tweenBase->distance += abs(lengthPos);
    }


    // 管理番号返却
    return tweenBase->no;
}


int Tween::SetTween(vec2* pos, vec2* movPos, int maxPos,
    float time, bool loopFlag, bool playFlag)
{
    // TweenBase の作成
    // 同一アドレスの再利用（二重登録禁止）
    TweenBase* tweenBase = CreateTbase((void*)pos);

    // パラメータセット
    tweenBase->mode = TweenMode::Vec2;      // モードセット
    tweenBase->playFlag = playFlag;         // 再生フラグセット
    tweenBase->loopFlag = loopFlag;         // ループフラグ
    tweenBase->moveFlag = true;             // 移動フラグ（移動終了：false）
    tweenBase->maxTime = time;              // 時間セット
    tweenBase->countTime = 0;               // 時間カウンタ初期化

    // サイズ変更
    // maxCount + 1 ： ループ用に戻る座標を追加
    tweenBase->maxCount = maxPos;
    tweenBase->count = 0;
    tweenBase->list.resize(maxPos + 1);

    // movPosのセット
    tweenBase->distance = 0;
    vec2 oldPos = movPos[0];
    int i = 0;
    for (i = 0; i < maxPos; i++)
    {
        tweenBase->list[i] = (void*)&movPos[i];
        // 距離の計算
        vec2 lengthPos = movPos[i] - oldPos;
        // 距離を加算
        tweenBase->distance += glm::length(lengthPos);
        // 前座標更新
        oldPos = movPos[i];
    }

    // ループ用ポジションセット
    tweenBase->list[i] = (void*)&movPos[0];

    // ループ用距離の更新
    if (loopFlag == true)
    {
        vec2 lengthPos = movPos[0] - oldPos;
        // 距離を加算
        tweenBase->distance += glm::length(lengthPos);
    }


    // 管理番号返却
    return tweenBase->no;
}


int Tween::SetTween(vec3* pos, vec3* movPos, int maxPos,
    float time, bool loopFlag, bool playFlag)
{
    // TweenBase の作成
    // 同一アドレスの再利用（二重登録禁止）
    TweenBase* tweenBase = CreateTbase((void*)pos);

    // パラメータセット
    tweenBase->mode = TweenMode::Vec3;      // モードセット
    tweenBase->playFlag = playFlag;         // 再生フラグセット
    tweenBase->loopFlag = loopFlag;         // ループフラグ
    tweenBase->moveFlag = true;             // 移動フラグ（移動終了：false）
    tweenBase->maxTime = time;              // 時間セット
    tweenBase->countTime = 0;               // 時間カウンタ初期化

    // サイズ変更
    // maxCount + 1 ： ループ用に戻る座標を追加
    tweenBase->maxCount = maxPos;
    tweenBase->count = 0;
    tweenBase->list.resize(maxPos + 1);

    // movPosのセット
    tweenBase->distance = 0;
    vec3 oldPos = movPos[0];
    int i = 0;
    for (i = 0; i < maxPos; i++)
    {
        tweenBase->list[i] = (void*)&movPos[i];
        // 距離の計算
        vec3 lengthPos = movPos[i] - oldPos;
        // 距離を加算
        tweenBase->distance += glm::length(lengthPos);
        // 前座標更新
        oldPos = movPos[i];
    }

    // ループ用ポジションセット
    tweenBase->list[i] = (void*)&movPos[0];

    // ループ用距離の更新
    if (loopFlag == true)
    {
        vec3 lengthPos = movPos[0] - oldPos;
        // 距離を加算
        tweenBase->distance += glm::length(lengthPos);
    }

    // 管理番号返却
    return tweenBase->no;
}



// フラグセット
void Tween::SetFlag()
{

}


// 情報の取得
// 管理番号の取得
int Tween::GetTweenNo(void* pos)
{
    int no = -1;
    int listno = AddressCheck(pos);
    if (listno != -1)
    {
        // 管理番号セット
        no = tbaseList[listno]->no;
    }

    return no;
}

// 移動終了取得
bool Tween::GetMoveFlag(void* pos)
{
    bool mFlag = false;
    int listno = AddressCheck(pos);
    if (listno != -1)
    {
        // 移動フラグセット
        mFlag = tbaseList[listno]->moveFlag;
    }

    return mFlag;
}
bool Tween::GetMoveFlag(int no)
{
    bool mFlag = false;
    int listno = AddressCheck(no);
    if (listno != -1)
    {
        // 移動フラグセット
        mFlag = tbaseList[listno]->moveFlag;
    }

    return mFlag;
}

// 再生フラグ取得
bool Tween::GetPlayFlag(void* pos)
{
    bool pFlag = false;
    int listno = AddressCheck(pos);
    if (listno != -1)
    {
        // 移動フラグセット
        pFlag = tbaseList[listno]->playFlag;
    }

    return pFlag;
}
bool Tween::GetPlayFlag(int no)
{
    bool pFlag = false;
    int listno = AddressCheck(no);
    if (listno != -1)
    {
        // 移動フラグセット
        pFlag = tbaseList[listno]->playFlag;
    }

    return pFlag;
}


// コマンド設定
bool Tween::SetCommand(TweenCommand tc, int no, float time)
{
    int listno = AddressCheck(no);
    if (listno != -1)
    {
        return false;
    }

    TweenBase* tb = tbaseList[listno];

    // 時間の計算
    float maxMMSec = (1000.0f * tb->maxTime);
    float minMov = tb->distance / maxMMSec;


    switch (tc)
    {
    case TweenCommand::Init:
        // 初期化（初期位置へ戻す）
        tb->count = 0;
        break;

    case TweenCommand::EndPara:
        // 最終位置（最終位置へ移動）
        tb->count = (int)(maxMMSec / 16.67f);
        break;

    case TweenCommand::TimePara:
        // 時間指定位置（指定された時間の位置へ移動）
        if (maxMMSec < time)
        {
            tb->count = (int)(maxMMSec / 16.67f);
        }
        else
        {
            tb->count = (int)(time / 16.67f);
        }

        break;

    case TweenCommand::Delete:
        // データ削除
        tbaseList[listno]->list.clear();        // 移動リストデータ解放
        erase(tbaseList, tbaseList[listno]);    // ベース削除
        break;

    }

    return true;
}


// 再生処理
// 全データの再生
void Tween::TweenPlay()
{
    // 登録された数ループ
    for (int i = 0; i < tbaseList.size(); i++)
    {
        TweenBase* tb = tbaseList[i];

        // 再生フラグチェック
        if (tb->playFlag == false)
        {
            continue;
        }

        switch (tb->mode)
        {
        case TweenMode::Float:
            FloatPlay(tb);
            break;
        case TweenMode::Vec2:
            Vec2Play(tb);
            break;
        case TweenMode::Vec3:
            Vec3Play(tb);
            break;
        }
    }

}
// 指定データの再生
void Tween::TweenPlay(void* pos)
{
    // 同一アドレスチェック
    int listNo = AddressCheck(pos);
    if (listNo != -1)
    {
        return;
    }

    // データの種類別再生
    TweenBase* tb = tbaseList[listNo];

    // 再生フラグチェック
    if (tb->playFlag == false)
    {
        return;
    }

    switch (tb->mode)
    {
    case TweenMode::Float:
        FloatPlay(tb);
        break;
    case TweenMode::Vec2:
        Vec2Play(tb);
        break;
    case TweenMode::Vec3:
        Vec3Play(tb);
        break;
    }
}
// 指定データの再生
void Tween::TweenPlay(int no)
{
    // 同一アドレスチェック
    int listNo = AddressCheck(no);
    if (listNo != -1)
    {
        return;
    }

    // データの種類別再生
    TweenBase* tb = tbaseList[listNo];

    // 再生フラグチェック
    if (tb->playFlag == false)
    {
        return;
    }

    switch (tb->mode)
    {
    case TweenMode::Float:
        FloatPlay(tb);
        break;
    case TweenMode::Vec2:
        Vec2Play(tb);
        break;
    case TweenMode::Vec3:
        Vec3Play(tb);
        break;
    }
}


// float 再生
float Tween::FloatPlay(TweenBase* tb)
{
    // フレームカウントを秒へ変換
    float sec = tb->count * 16.67f;

    // 最小移動量 距離 / (1000 * 移動時間)
    float maxMMSec = (1000.0f * tb->maxTime);
    float minMov = tb->distance / maxMMSec;

    // ループ数
    int loopMax = tb->maxCount;

    // ループフラグON
    if (tb->loopFlag == true)
    {
        // ループ数(Max+1)
        loopMax++;

        // 移動時間を超えたかチェック
        if (sec > maxMMSec)
        {
            // float のあまり算
            int num = (int)(sec / maxMMSec);
            sec -= maxMMSec * num;
        }

    }

    // 移動距離の計算（最小移動距離 * 秒）
    float distance = minMov * sec;

    // ポイント間の移動量
    float absMov = 0;
    float sumMov = 0;

    // 自分のリスト配列の位置計算
    float* pFloat = (float*)tb->list[0];   // キャスト変換
    float oldPos = *pFloat;
    int i = 1;
    for (i = 1; i < loopMax; i++)
    {
        // ポイント間の距離計算
        pFloat = (float*)tb->list[i];   // キャスト変換
        absMov = abs(*pFloat - oldPos);
        if (distance <= sumMov + absMov)
        {
            // ポイントの間にいる
            break;
        }

        // 座標更新
        oldPos = *pFloat;
        sumMov += absMov;

    }

    // ループ終了
    float pos = 0;

    // ループ最大チェック
    if (i == loopMax)
    {
        // ポジションセット（最終座標）
        pos = oldPos;

        // ループフラグチェック
        if (tb->loopFlag == false)
        {
            // ループで無いとき
            tb->moveFlag = false;
        }

    }
    else
    {
        // 中間座標計算
        pFloat = (float*)tb->list[i];   // キャスト変換

        float ePos = (*pFloat - oldPos) / absMov;
        pos = oldPos + (ePos * (distance - sumMov));
    }

    // 座標をセット
    float* floatPos = (float*)tb->pos;
    *floatPos = pos;

    // カウントアップ
    tb->count++;

    return pos;
}


// vec2 再生
vec2 Tween::Vec2Play(TweenBase* tb)
{
    // フレームカウントを秒へ変換
    float sec = tb->count * 16.67f;

    // 最小移動量 距離 / (1000 * 移動時間)
    float maxMMSec = (1000.0f * tb->maxTime);
    float minMov = tb->distance / maxMMSec;

    // ループ数
    int loopMax = tb->maxCount;

    // ループフラグON
    if (tb->loopFlag == true)
    {
        // ループ数(Max+1)
        loopMax++;

        // 移動時間を超えたかチェック
        if (sec > maxMMSec)
        {
            // float のあまり算
            int num = (int)(sec / maxMMSec);
            sec -= maxMMSec * num;
        }

    }

    // 移動距離の計算（最小移動距離 * 秒）
    float distance = minMov * sec;

    // ポイント間の移動量
    float absMov = 0;
    float sumMov = 0;

    // 自分のリスト配列の位置計算
    vec2* pVec = (vec2*)tb->list[0];   // キャスト変換
    vec2 oldPos = *pVec;
    int i = 1;
    for (i = 1; i < loopMax; i++)
    {
        // ポイント間の距離計算
        pVec = (vec2*)tb->list[i];   // キャスト変換
        absMov = glm::length(*pVec - oldPos);
        if (distance <= sumMov + absMov)
        {
            // ポイントの間にいる
            break;
        }

        // 座標更新
        oldPos = *pVec;
        sumMov += absMov;

    }

    // ループ終了
    vec2 pos = vec2(0, 0);

    // ループ最大チェック
    if (i == loopMax)
    {
        // ポジションセット（最終座標）
        pos = oldPos;

        // ループフラグチェック
        if (tb->loopFlag == false)
        {
            // ループで無いとき
            tb->moveFlag = false;
        }

    }
    else
    {
        // 中間座標計算
        pVec = (vec2*)tb->list[i];   // キャスト変換

        vec2 ePos = glm::normalize(*pVec - oldPos);
        pos = oldPos + (ePos * (distance - sumMov));
    }

    // 座標をセット
    vec2* vecPos = (vec2*)tb->pos;
    *vecPos = pos;

    // カウントアップ
    tb->count++;

    return pos;
}


// vec3 再生
vec3 Tween::Vec3Play(TweenBase* tb)
{
    // フレームカウントを秒へ変換
    float sec = tb->count * 16.67f;

    // 最小移動量 距離 / (1000 * 移動時間)
    float maxMMSec = (1000.0f * tb->maxTime);
    float minMov = tb->distance / maxMMSec;

    // ループ数
    int loopMax = tb->maxCount;

    // ループフラグON
    if (tb->loopFlag == true)
    {
        // ループ数(Max+1)
        loopMax++;

        // 移動時間を超えたかチェック
        if (sec > maxMMSec)
        {
            // float のあまり算
            int num = (int)(sec / maxMMSec);
            sec -= maxMMSec * num;
        }

    }

    // 移動距離の計算（最小移動距離 * 秒）
    float distance = minMov * sec;

    // ポイント間の移動量
    float absMov = 0;
    float sumMov = 0;

    // 自分のリスト配列の位置計算
    vec3* pVec = (vec3*)tb->list[0];   // キャスト変換
    vec3 oldPos = *pVec;
    int i = 1;
    for (i = 1; i < loopMax; i++)
    {
        // ポイント間の距離計算
        pVec = (vec3*)tb->list[i];   // キャスト変換
        absMov = glm::length(*pVec - oldPos);
        if (distance <= sumMov + absMov)
        {
            // ポイントの間にいる
            break;
        }

        // 座標更新
        oldPos = *pVec;
        sumMov += absMov;

    }

    // ループ終了
    vec3 pos = vec3(0, 0, 0);

    // ループ最大チェック
    if (i == loopMax)
    {
        // ポジションセット（最終座標）
        pos = oldPos;

        // ループフラグチェック
        if (tb->loopFlag == false)
        {
            // ループで無いとき
            tb->moveFlag = false;
        }

    }
    else
    {
        // 中間座標計算
        pVec = (vec3*)tb->list[i];   // キャスト変換

        vec3 ePos = glm::normalize(*pVec - oldPos);
        pos = oldPos + (ePos * (distance - sumMov));
    }

    // 座標をセット
    vec3* vecPos = (vec3*)tb->pos;
    *vecPos = pos;

    // カウントアップ
    tb->count++;

    return pos;
}


// TweenBase の作成
TweenBase* Tween::CreateTbase(void* pos)
{
    TweenBase* tweenBase = nullptr;
    // 同一アドレスチェック
    int no = AddressCheck(pos);
    if (no != -1)
    {
        // アドレスの再利用
        tweenBase = tbaseList[no];
    }
    else
    {
        // TweenFlat の実体作成
        tweenBase = new TweenBase();
        // 管理番号登録
        tweenBase->no = tweenCount;
        // アドレスセット
        tweenBase->pos = pos;

        // リストへ追加
        tweenCount++;
        tbaseList.resize(tweenCount);
        tbaseList[tweenCount - 1] = (TweenBase*)tweenBase;
    }

    return tweenBase;
}


// 同一アドレスチェック
int Tween::AddressCheck(void* pos)
{
    //ループ
    for (int i = 0; i < tweenCount; i++)
    {
        void* basePos = tbaseList[i]->pos;

        // 同一アドレスチェック
        if (pos == basePos)
        {
            // アドレス再利用
            return i;
        }
    }

    return -1;
}
int Tween::AddressCheck(int no)
{
    //ループ
    for (int i = 0; i < tweenCount; i++)
    {
        int baseNo = tbaseList[i]->no;

        // 同一管理番号
        if (no == baseNo)
        {
            // リスト番号返却
            return i;
        }
    }

    return -1;
}

