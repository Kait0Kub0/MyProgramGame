#pragma once

#include "system.h"

class ThreadData
{
public:
    // 全体クラス
    void* pClass;

    // 管理番号
    UINT no;
    // 名前
    string name;
    // スレッドハンドル
    HANDLE handle;
    // スレッドID
    UINT threadId;

    // 強制終了フラグ
    bool exitFlag;

    // フレームレート
    int fcount;

    // スレッド終了フラグ
    bool endFlag;

    // コンストラクタ
    ThreadData()
    {
        pClass = nullptr;
        no = 0;
        name = "test";
        handle = 0;
        threadId = 0;
        exitFlag = false;
        endFlag = false;
        fcount = 1000 / 60; // 1/60秒（デフォルト）
    }

    // 初期化用関数(全体アドレスの取得)
    void ThreadInit(void* p)
    {
        pClass = p;
    }
};



class ThreadManager
{
private:

    // スレッドの管理変数
    vector<ThreadData *> threadList;

    // スレッドリストカウンター
    UINT count = 0;

public:

    // スレッドの追加
    HANDLE SetThrade(
        _beginthreadex_proc_type pSub,  // スレッド処理する関数
        ThreadData* threadData,         // スレッド処理するデータクラス
        string name = "",               // 名前
        int fcount = (1000 / 60)        // フレームレート
    );

    // スレッドの終了(強制終了)
    void Exit(int listNo);

    // スレッド削除
    bool Delete(int listNo);
    bool Delete(string name);


    // スレッド検索（名前の検索）
    int Search(string name);

    // リスト番号検索
    int Search(int listNo);
};


// プロトタイプ宣言
extern ThreadManager* threadManagerObj;