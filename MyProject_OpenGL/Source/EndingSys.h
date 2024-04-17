#pragma once

#include "MainSys.h"

// システム
enum class EndingFlag
{
    INIT,
    MAIN,
    END,
    MAX,
};



// エンディング処理クラス
class EndingSys
{
private:
    // シーンフラグ
    MainSys* mainSys;

    // エンディング管理フラグ
    EndingFlag endingFlag = EndingFlag::INIT;


    bool dataFlag = false;


    // キー入力
    short endingKeyIn;
    int   endingKeyFlag; //押しっぱなしフラグ

    GLuint bgImageNo;

public:
    // コンストラクタ
    EndingSys(MainSys* ms);


    // タイトル関数
    void Ending(void);
    void EndingInit(void);
    void EndingMain(void);
    void EndingEnd(void);
    void SysDraw3D(void);



};