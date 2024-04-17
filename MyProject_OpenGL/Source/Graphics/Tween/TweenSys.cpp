#include "TweenSys.h"


// 実体
Tween* tweenObj = new Tween();


// コンストラクタ
Tween::Tween()
{
    tweenCount = 0;

    exitFlag = false;

    // スレッド用初期化
    this->ThreadInit(this);
}



// データセット
int Tween::SetTween(float* pos, float* movPos, int count,
    float tweenTime, bool loopFlag, bool dispflag)
{
    TweenFloat* tweenFloat = nullptr;

    // 同一アドレスチェック
    int no = AddressCheck((void*)pos);
    if (no != -1)
    {
        // アドレスの再利用
        tweenFloat = (TweenFloat *)tbaseList[no];
    }
    else
    {
        // TweenFlat の実体作成
        tweenFloat = new TweenFloat();
        no = tbaseList.size();
        tbaseList.resize(no + 1);
        // リスト番号セット
        tweenFloat->listNo = tweenCount;
        tweenCount++;
    }


    // pos のセット
    tweenFloat->pos = pos;

    // サイズ変更
    tweenFloat->maxCount = count;
    tweenFloat->count = 0;
    tweenFloat->floatList.resize(count + 1);

    // movPosのセット
    tweenFloat->distance = 0;
    float oldPos = *pos;
    int i = 0;
    for (i = 0; i < count; i++)
    {
        tweenFloat->floatList[i] = movPos[i];
        // 距離の計算
        float lengthPos = movPos[i] - oldPos;
        tweenFloat->distance += abs(lengthPos);
        // 前座標更新
        oldPos = movPos[i];
    }
    // ループで戻る座標追加
    tweenFloat->floatList[i] = movPos[0];
    if (loopFlag == true)
    {
        // ループ用の距離計算
        float lengthPos = movPos[0] - oldPos;
        tweenFloat->distance += abs(lengthPos);
    }

    // パラメータセット
    tweenFloat->mode = TweenMode::Float;    // モードセット
    tweenFloat->dispFlag = dispflag;        // フラグセット
    tweenFloat->maxTime = tweenTime;        // 時間セット
    tweenFloat->time = 0;                   // 時間カウンタ初期化
    tweenFloat->loop = loopFlag;            // ループフラグ

    // tbaseList へのセット
    tbaseList[no] = (TBase*)tweenFloat;

    return tweenFloat->listNo;
}


// データセット
int Tween::SetTween(vec2* pos, vec2* movPos, int count,
    float tweenTime, bool loopFlag, bool dispflag)
{
    // TweenVec2 の実体
    TweenVec2* tweenVec2 = nullptr;

    // 同一アドレスチェック
    int no = AddressCheck((void*)pos);
    if (no != -1)
    {
        // アドレスの再利用
        tweenVec2 = (TweenVec2*)tbaseList[no];
    }
    else
    {
        // TweenVec2 の実体作成
        tweenVec2 = new TweenVec2();

        no = tbaseList.size();
        tbaseList.resize(no + 1);
        // リスト番号セット
        tweenVec2->listNo = tweenCount;
        tweenCount++;
    }

    // pos のセット
    tweenVec2->pos = pos;

    // サイズ変更
    tweenVec2->maxCount = count;
    tweenVec2->count = 0;
    tweenVec2->vec2List.resize(count + 1);

    // movPosのセット
    tweenVec2->distance = 0;
    vec2 oldPos = *pos;
    int i = 0;
    for (i = 0; i < count; i++)
    {
        tweenVec2->vec2List[i] = movPos[i];
        // 距離計算　(移動座標)2　-　（移動前）1
        vec2 lengthPos = movPos[i] - oldPos;
        tweenVec2->distance += glm::length(lengthPos);
        // 前座標更新
        oldPos = movPos[i];
    }
    // ループで戻る座標追加
    tweenVec2->vec2List[i] = movPos[0];
    if (loopFlag == true)
    {
        // ループ用の距離計算
        vec2 lengthPos = movPos[0] - oldPos;
        tweenVec2->distance += glm::length(lengthPos);
    }


    // パラメータセット
    tweenVec2->mode = TweenMode::Vec2;     // モードセット
    tweenVec2->dispFlag = dispflag;        // フラグセット
    tweenVec2->maxTime = tweenTime;        // 時間セット
    tweenVec2->time = 0;                   // 時間カウンタ初期化
    tweenVec2->loop = loopFlag;            // ループフラグ


    // tbaseList へのセット
    tbaseList[no] = (TBase*)tweenVec2;

    return tweenVec2->listNo;
}


// データセット
int Tween::SetTween(vec3* pos, vec3* movPos, int count,
    float tweenTime, bool loopFlag, bool dispflag)
{
    // TweenVec3 の実体
    TweenVec3* tweenVec3 = nullptr;

    // 同一アドレスチェック
    int no = AddressCheck((void*)pos);
    if (no != -1)
    {
        // アドレスの再利用
        tweenVec3 = (TweenVec3*)tbaseList[no];
    }
    else
    {
        // TweenVec3 の実体作成
        tweenVec3 = new TweenVec3();

        // tbaseList へのセット
        no = tbaseList.size();
        tbaseList.resize(no + 1);
        // リスト番号セット
        tweenVec3->listNo = tweenCount;
        tweenCount++;
    }


    // pos のセット
    tweenVec3->pos = pos;

    // サイズ変更
    tweenVec3->maxCount = count;
    tweenVec3->count = 0;
    tweenVec3->vec3List.resize(count + 1);

    tweenVec3->distance = 0;
    vec3 oldPos = movPos[0];

    // movPosのセット
    int i = 0;
    for (i = 0; i < count; i++)
    {
        tweenVec3->vec3List[i] = movPos[i];
        // 距離計算　(移動座標)2　-　（移動前）1
        vec3 lengthPos = movPos[i] - oldPos;
        tweenVec3->distance += glm::length(lengthPos);
        // 前座標更新
        oldPos = movPos[i];
    }
    // ループで戻る座標追加
    tweenVec3->vec3List[i] = movPos[0];
    if (loopFlag == true)
    {
        // ループ用の距離計算
        vec3 lengthPos = movPos[0] - oldPos;
        tweenVec3->distance += glm::length(lengthPos);
    }

    // パラメータセット
    tweenVec3->mode = TweenMode::Vec3;     // モードセット
    tweenVec3->dispFlag = dispflag;        // フラグセット
    tweenVec3->maxTime = tweenTime;        // 時間セット
    tweenVec3->time = 0;                   // 時間カウンタ初期化
    tweenVec3->loop = loopFlag;            // ループフラグ

    tbaseList[no] = (TBase*)tweenVec3;

    return tweenVec3->listNo;
}


// スキップ処理
bool Tween::SetSkip(int listNo, float time)
{
    // リストNoの検索
    int no = Search(listNo);
    if (no == -1)
    {
        return false;
    }

    // スキップタイムの計算
    TBase* tb = tbaseList[no];
    // 最小移動量 距離 / (1000 * 移動時間)
    float maxMMSec = (1000.0f * tb->maxTime);
    float minMov = tb->distance / maxMMSec;

    float distance = minMov * (time * 1000.0f);
    float sec = time * 1000.0f;

    // 座標をセット
    TweenFloat* tf  = (TweenFloat*)tbaseList[no];
    TweenVec2*  tv2 = (TweenVec2*)tbaseList[no];
    TweenVec3*  tv3 = (TweenVec3*)tbaseList[no];

    float fpos;
    vec2  v2pos;
    vec3  v3pos;

    // データの分岐
    switch (tbaseList[no]->mode)
    {
    case TweenMode::Float:
        // 移動後の座標取得
        fpos = FloatPos(no, sec);
        *tf->pos = fpos;
        break;

    case TweenMode::Vec2:
        // 移動後の座標取得
        v2pos = Vec2Pos(no, sec);
        *tv2->pos = v2pos;
        break;

    case TweenMode::Vec3:
        // 移動後の座標取得
        v3pos = Vec3Pos(no, sec);
        *tv3->pos = v3pos;
        break;
    }

}


// 状態取得
bool Tween::GetTweenDisp(int listNo)
{
    // リスト番号のチェック
    int ret = Search(listNo);
    if (ret == -1)
    {
        return false;
    }

    return tbaseList[ret]->dispFlag;
}


// 移動処理(全登録データの移動)
void Tween::TweenMove()
{
    // 登録された数ループ
    for (int i = 0; i < tbaseList.size(); i++)
    {
        switch (tbaseList[i]->mode)
        {
        case TweenMode::Float:
            FlaotMov(i);
            break;
        case TweenMode::Vec2:
            Vec2Mov(i);
            break;
        case TweenMode::Vec3:
            Vec3Mov(i);
            break;
        }
    }
}


// スレッド登録用移動処理
unsigned Tween::ThreadTweenMove(void* p)
{
    // キャスト変換
    Tween* tweenObj = (Tween*)p;

    // スレッド内でループ
    while (true)
    {
        // 強制終了チェック
        if (tweenObj->exitFlag == true)
        {
            break;
        }

        // フレーム待ち
        Sleep(tweenObj->fcount);


        // 登録された数ループ
        for (int i = 0; i < tweenObj->tbaseList.size(); i++)
        {
            switch (tweenObj->tbaseList[i]->mode)
            {
            case TweenMode::Float:
                tweenObj->FlaotMov(i);
                break;
            case TweenMode::Vec2:
                tweenObj->Vec2Mov(i);
                break;
            case TweenMode::Vec3:
                tweenObj->Vec3Mov(i);
                break;
            }
            
//            cout << tweenObj->tbaseList[i]->listNo << ":" << tweenObj->tbaseList[i]->count << endl;
        }

    }

    // 終了パラメータ
    _endthreadex(false);

    return 0;
}



// float 変化
void Tween::FlaotMov(int i)
{
    // キャスト変換
    TweenFloat* tf = (TweenFloat*)tbaseList[i];

    // フラグチェック
    if (tf->dispFlag == false)
    {
        return;
    }


    // フレームカウントを秒へ変換
    float sec = tf->count * 16.67f;
    // 移動後の座標取得
    float pos = FloatPos(i, sec);

    // 座標をセット
    *tf->pos = pos;

    // カウントアップ
    tf->count++;

}


// 秒からポジション
float Tween::FloatPos(int no, float sec)
{
    // キャスト変換
    TweenFloat* tf = (TweenFloat*)tbaseList[no];

    // 最小移動量 距離 / (1000 * 移動時間)
    float maxMMSec = (1000.0f * tf->maxTime);
    float minMov = tf->distance / maxMMSec;


    // ループ数
    int loopMax = tf->maxCount;

    // ループフラグON
    if (tf->loop == true)
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
    float oldPos = tf->floatList[0];
    int i = 1;
    for (i = 1; i < loopMax; i++)
    {
        // ポイント間の距離計算
        absMov = abs(tf->floatList[i] - oldPos);
        if (distance <= sumMov + absMov)
        {
            // ポイントの間にいる
            break;
        }

        // 座標更新
        oldPos = tf->floatList[i];
        sumMov += absMov;

    }

    // ループ終了
    float pos = 0;

    // 時間でチェック
    if (maxMMSec <= sec)
    {
        // ポジションセット（最終座標）
        pos = tf->floatList[loopMax-1];
        // ループのチェック
        if (tf->loop == false)
        {
            // 処理の終了
            tf->dispFlag = false;
        }
    }
    else
    {
        // 中間座標計算
        float ePos = (tf->floatList[i] - oldPos) / absMov;
        pos = oldPos + (ePos * (distance - sumMov));
    }

    return pos;
}


// vec2 変化
void Tween::Vec2Mov(int i)
{
    // キャスト変換
    TweenVec2* tv2 = (TweenVec2*)tbaseList[i];

    // フラグチェック
    if (tv2->dispFlag == false)
    {
        return;
    }


    // フレームカウントを秒へ変換
    float sec = tv2->count * 16.67f;
    // 移動後の座標取得
    vec2 pos = Vec2Pos(i, sec);

    // 座標をセット
    *tv2->pos = pos;

    // カウントアップ
    tv2->count++;

}


// 秒からポジション
vec2 Tween::Vec2Pos(int no, float sec)
{
    // キャスト変換
    TweenVec2* tv2 = (TweenVec2*)tbaseList[no];

    // 最小移動量 距離 / (1000 * 移動時間)
    float maxMMSec = (1000.0f * tv2->maxTime);
    float minMov = tv2->distance / maxMMSec;


    // ループ数
    int loopMax = tv2->maxCount;

    // ループフラグON
    if (tv2->loop == true)
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
    vec2 oldPos = tv2->vec2List[0];
    int i = 1;
    for (i = 1; i < loopMax; i++)
    {
        // ポイント間の距離計算
        absMov = glm::length(tv2->vec2List[i] - oldPos);
        if (distance <= sumMov + absMov)
        {
            // ポイントの間にいる
            break;
        }

        // 座標更新
        oldPos = tv2->vec2List[i];
        sumMov += absMov;

    }

    // ループ終了
    vec2 pos = vec2(0, 0);

    // ループ最大チェック（）
    if (i == loopMax)
    {
        // ポジションセット（最終座標）
        pos = tv2->vec2List[loopMax-1];

        // ループのチェック
        if (tv2->loop == false)
        {
            // 処理の終了
            tv2->dispFlag = false;
        }

    }
    else
    {
        // 中間座標計算
        vec2 ePos = glm::normalize( tv2->vec2List[i] - oldPos );
        pos = oldPos + (ePos * (distance - sumMov));
    }

    return pos;
}



// vec3 変化
void Tween::Vec3Mov(int i)
{
    // キャスト変換
    TweenVec3* tv3 = (TweenVec3*)tbaseList[i];

    // フラグチェック
    if (tv3->dispFlag == false)
    {
        return;
    }


    // フレームカウントを秒へ変換
    float sec = tv3->count * 16.67f;
    // 移動後の座標取得
    vec3 pos = Vec3Pos(i, sec);

    // 座標をセット
    *tv3->pos = pos;

    // カウントアップ
    tv3->count++;
}


// 秒からポジション
vec3 Tween::Vec3Pos(int no, float sec)
{
    // キャスト変換
    TweenVec3* tv3 = (TweenVec3*)tbaseList[no];

    // 最小移動量 距離 / (1000 * 移動時間)
    float maxMMSec = (1000.0f * tv3->maxTime);
    float minMov = tv3->distance / maxMMSec;


    // ループ数
    int loopMax = tv3->maxCount;

    // ループフラグON
    if (tv3->loop == true)
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
    vec3 oldPos = tv3->vec3List[0];
    int i = 1;
    for (i = 1; i < loopMax; i++)
    {
        // ポイント間の距離計算
        absMov = glm::length(tv3->vec3List[i] - oldPos);
        if (distance <= sumMov + absMov)
        {
            // ポイントの間にいる
            break;
        }

        // 座標更新
        oldPos = tv3->vec3List[i];
        sumMov += absMov;

    }

    // ループ終了
    vec3 pos = vec3(0, 0, 0);

    // ループ最大チェック（）
    if (i == loopMax)
    {
        // ポジションセット（最終座標）
        pos = tv3->vec3List[loopMax-1];

        // ループのチェック
        if (tv3->loop == false)
        {
            // 処理の終了
            tv3->dispFlag = false;
        }
    }
    else
    {
        // 中間座標計算
        vec3 ePos = glm::normalize(tv3->vec3List[i] - oldPos);
        pos = oldPos + (ePos * (distance - sumMov));
    }

    return pos;
}


// アドレスチェック
int Tween::AddressCheck(void* pos)
{
    //ループ
    for (int i = 0; i < tbaseList.size(); i++)
    {
        void* pt = nullptr;
        switch (tbaseList[i]->mode)
        {
        case TweenMode::Float:
            pt = (void*)((TweenFloat*)tbaseList[i])->pos;
            break;
        case TweenMode::Vec2:
            pt = (void*)((TweenVec2*)tbaseList[i])->pos;
            break;
        case TweenMode::Vec3:
            pt = (void*)((TweenVec3*)tbaseList[i])->pos;
            break;
        }

        // 同一アドレスチェック
        if (pos == pt)
        {
            // アドレス再利用
            return i;
        }
    }

    return -1;
}


// 検索処理
// リスト番号検索
int Tween::Search(int listNo)
{
    // 検索処理
    for (int i = 0; i < tbaseList.size(); i++)
    {
        if (listNo == tbaseList[i]->listNo)
        {
            return i;
        }
    }

    return -1;
}


// Tweenの移動処理初期化
bool Tween::ResetMove(int listNo )
{
    // データチェック
    int no = Search( listNo );
    if (no == -1)
    {
        // データなし
        return false;
    }

    // 初期化
    tbaseList[no]->count = 0;
}

bool Tween::ResetMove(void* pos)
{
    // データチェック
    int no = AddressCheck(pos);
    if (no == -1)
    {
        // データなし
        return false;
    }

    // 初期化
    tbaseList[no]->count = 0;
}


// tween から削除
bool Tween::Delet(int listNo)
{
    // データチェック
    int no = Search(listNo);
    if (no == -1)
    {
        // データなし
        return false;
    }

    // リストの削除処理
    tbaseList.erase(tbaseList.begin() + no);
}
bool Tween::Delet(void* pos)
{
    // データチェック
    int no = AddressCheck(pos);
    if (no == -1)
    {
        // データなし
        return false;
    }

    // リストの削除処理
    tbaseList.erase(tbaseList.begin() + no);
}
