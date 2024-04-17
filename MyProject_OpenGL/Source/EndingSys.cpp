#include "EndingSys.h"



// コンストラクタ
EndingSys::EndingSys(MainSys *ms)
{
	// メインシステムセット
	mainSys = ms;

    endingFlag = EndingFlag::INIT;
	endingKeyIn = 0;
	endingKeyFlag = true;
}



// エンディングシステム
void EndingSys::Ending(void)
{
	switch (endingFlag)
	{
	case EndingFlag::INIT:
		// 初期化
		EndingInit();
		break;
	case EndingFlag::MAIN:
		// メイン
		EndingMain();
		break;
	case EndingFlag::END:
		// 終了処理
		EndingEnd();
		break;
	default:
		break;
	}

	return;
}



// 初期化
void EndingSys::EndingInit(void)
{
	// メインに切り替え
 	endingFlag = EndingFlag::MAIN;

	if (dataFlag == false)
	{
		bgImageNo = TextureLoad("dat/Ending.png");

		dataFlag = true;
	}

	// スプライトセット
	SpriteParam(0, bgImageNo, 0, 0, 0, 0, 0, 0);
	SpriteScale(0, 1024, 600, 1);
	SpriteFlag(0, true);

	// 最初はキーを押せないようにする。
	endingKeyFlag = true;


	printf("エンディングチェック\n");
}



// メイン
void EndingSys::EndingMain(void)
{
	// キー入力
	endingKeyIn = GetAsyncKeyState(VK_SPACE);
	if (endingKeyIn & 0x8000)
	{
		if (endingKeyFlag == false)
		{
			endingKeyFlag = true;
			endingFlag = EndingFlag::END;
		}
	}
	else
	{
		// 押してないときの処理
		endingKeyFlag = false;
	}


	// 描画処理
	SysDraw3D();
}


//終了処理
void EndingSys::EndingEnd(void)
{
	// フラグ切り替え
	endingFlag = EndingFlag::INIT;

	// スプライトOFF
	SpriteFlag(0, false);

	// ゲームフラグ切り替え
	mainSys->SetSceneFlag(SceneFlag::TITLE);
}



//----------------------------------------------------------------------
// 描画処理
//----------------------------------------------------------------------
void EndingSys::SysDraw3D(void)
{
	// 背景色
	glClearColor(0.0f, 0.5f, 0.0f, 1.0f);

	// メインループ
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(1.0f, 1.0f, 1.0f);

	// ライトON
	glDisable(GL_LIGHTING);
	//	glEnable(GL_LIGHTING);

	// 行列の初期化
	glLoadIdentity();


	// 視点設定
	gluLookAt(
		0.0f, 0.0f, 10.0f,  // 視点 
		0.0f, 0.0f, 0.0f,  // 注視点
		0.0f, 1.0f, 0.0f   // アップベクトル
	);

	// スプライト表示
	SpriteDraw();

	// バッファスワップ
	glfwSwapBuffers(GlfwWindow);
	// イベントのポーリング
	glfwPollEvents();


}



