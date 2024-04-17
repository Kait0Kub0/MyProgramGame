//----------------------------------------------------------------------
//  Copyright (C) 2021 by Human
//----------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "main.h"

#include "game.h"

#include "MainSys.h"

#include "BlockDivide.h"

//----------------------------------------------------------------------
// メインプログラム
//----------------------------------------------------------------------
int main(void)
{
    //BlockDivide bd;

    //bd.size = vec3(100, 1, 100);
    //bd.divideCount = ivec3(5, 1, 5);
    //bd.Init();
    //bd.BlockNo( vec3( 0, 0, 0) );


	// 乱数のシード設定（時間）
	srand((unsigned)time(NULL));


	// グラフィック処理初期化
	sysInit();


	// ゲームメインシステム
	MainSys mainSys = MainSys();

	// ゲームメインループ
	while (windowsLoop)
	{

		mainSys.GameSystem();
	}

	// 終了処理
	sysEnd();


	return 0;
}
