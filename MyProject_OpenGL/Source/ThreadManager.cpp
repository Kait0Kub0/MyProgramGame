#include "ThreadManager.h"

// スレッド管理システム実体
ThreadManager* threadManagerObj = new ThreadManager();


// スレッドの追加
HANDLE ThreadManager::SetThrade(
    _beginthreadex_proc_type pSub,  // スレッド処理する関数
    ThreadData* threadData,         // スレッド処理するデータクラス
    string name,                    // 名前
    int fcount                      // フレームレート
)
{
    // 名前の検索
    int ret = Search(name);
    if (ret != -1)
    {
        // 名前のエラー
        return 0;
    }

    // 初期化チェック
    // 全体アドレスが入っているかチェック
    if (threadData->pClass == nullptr)
    {
        return 0;
    }

    // 全体クラスのアドレス
    void* pClass = threadData->pClass;

    // スレッドの追加
    UINT threadId = 0;
    HANDLE handle = (HANDLE)_beginthreadex(
        NULL,           // セキュリティ構造体
        0,              // スタックサイズ
        pSub,           // スレッドの関数         ★
        pClass,         // スレッド関数へ渡す値   ★
        0,              // 初期化のパラメータ
        &threadId       // スレッドID             ★
    );


    // 名前の作成
    if (name == "")
    {
        name = "_threadNo" + to_string(count);
    }

    // リスト追加
    // サイズ追加処理修正　2023/11/04
    int size = threadList.size();
    threadList.resize(size+1);
    UINT listNo = size;

    ThreadData* td = threadData;

    td->no = count;          // リスト番号
    td->name = name;         // リストネーム
    td->handle = handle;     // スレッドハンドル
    td->threadId = threadId; // スレッドId
    td->fcount = fcount;     // フレームカウント

    // スレッド初期化
    // 2023/11/04
    td->exitFlag = false;
    td->endFlag = false;

    threadList[listNo] = td;

    // リスト番号追加
    count++;

    return handle;
}

// スレッドの終了(強制終了)
void ThreadManager::Exit(int listNo)
{
    // スレッドサーチ
    int ret = Search(listNo);
    if (ret == -1)
    {
        return;
    }

    // フラグセット
    threadList[ret]->exitFlag = true;
}

bool ThreadManager::Delete(string name)
{
    // スレッドサーチ
    int ret = Search(name);
    if (ret == -1)
    {
        return false;
    }

    threadList[ret]->endFlag = true;
    threadList[ret]->exitFlag = true;

    // 削除処理
    threadList.erase(threadList.begin() + ret);

    return true;
}


// スレッド削除
bool ThreadManager::Delete(int listNo)
{
    // スレッドサーチ
    int ret = Search(listNo);
    if (ret == -1)
    {
        return false;
    }

    // スレッド終了チェック
    //if (threadList[ret]->endFlag != true)
    //{
    //    // 強制終了
    //    threadList[ret]->exitFlag = true;

    //    // スレッド未終了
    //    return false;
    //}


    threadList[ret]->endFlag = true;
    threadList[ret]->exitFlag = true;

    // 削除処理
    threadList.erase(threadList.begin() + ret);

    return true;
}


// スレッド検索（名前の検索）
int ThreadManager::Search(string name)
{
    // 検索処理
    for (int i = 0; i < threadList.size(); i++)
    {
        if (name == threadList[i]->name)
        {
            return i;
        }
    }

    return -1;
}

// リスト番号検索
int ThreadManager::Search(int listNo)
{
    // 検索処理
    for (int i = 0; i < threadList.size(); i++)
    {
        if (listNo == threadList[i]->no)
        {
            return i;
        }
    }

    return -1;
}
