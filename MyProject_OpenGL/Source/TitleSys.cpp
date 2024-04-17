#include "TitleSys.h"
#include "Music/MusicSys.h"
#include "Look.h"
#include "FadeInOut.h"
#include "ThreadManager.h"

#include "Graphics/Tween/TweenSys.h"

// タイトル移動データ
vec3 titleMov[4] = {
    { 2000,  400, 0 },
    {    0, -400, 0 },
    { -500,  400, 0 },
    {    0,  400, 0 },
};


// スタートボタン拡大縮小
vec2 startScale[2] =
{
    { 0.6f, 0.6f },
    { 1.4f, 1.4f,},
};



// コンストラクタ
TitleSys::TitleSys( MainSys *ms )
{
	// メインシステムセット
	mainSys = ms;

	// ミュージック管理システム
	musicSys = new MusicSys();

	// テクスチャ番号
	bgImageNo = 0;

}



// タイトルシステム
void TitleSys::Title(void)
{
	switch ( titleFlag )
	{
	case TitleFlag::INIT:
		// タイトル初期化
		TitleInit();
		break;
	case TitleFlag::MAIN:
		// タイトルメイン
		TitleMain();
		break;
	case TitleFlag::END:
		// タイトル終了処理
		TitleEnd();
		break;
	default:
		break;
	}

	// 強制終了
	//sceneFlag = SceneFlag::EXIT;

	return;
}



// 初期化
void TitleSys::TitleInit(void)
{
	// タイトルメインに切替
	titleFlag = TitleFlag::MAIN;


	// カメラの初期化
	lookObj->Init();
	lookObj->pos = vec3(0, 0, 10);


	// データ登録
	if (dataFlag == false)
	{

		// 画像データ読み込み
		bgImageNo    = imageSysObj->SetImage("Title_A.png");
        int damyNo01 = imageSysObj->SetImage("Title_B.png");
        int damyNo02 = imageSysObj->SetImage("PushF1.png");
        int fadeNo   = imageSysObj->SetImage("BlackStar.png");

		// モデルデータの読み込み
		//int testModelNo = modelSysObj->LoadModel("UihitPlane_org.pol", "ui");

		// 音データ読み込み処理
		musicSys->SetMusicOgg("DAT/SOUND/mist.ogg");

        // フェードイン初期化
        fadeInOutObj->Init(-1, fadeNo);

		dataFlag = true;
	}


	// スプライトセット
    SpriteParam(0, bgImageNo, 0, 0, 0, 0, 0, 0);
    SpriteScale(0, 1920, 1080, 1);
    SpriteFlag(0, true);

    // タイトル
    int titleImg = imageSysObj->GetImageNo("Title_B.png");
    SpriteParam(1, titleImg, 0, 400, 0, 0, 0, 0);
    SpriteScale(1, 1024, 256, 1);
    SpriteFlag (1, true);

    // Tween 初期設定
    logoPos = titleMov[0];
    tweenObj->SetTween(&logoPos, &titleMov[0], 4, 3);


    // スタートキー
    int startImg = imageSysObj->GetImageNo("PushF1.png");
    SpriteParam(2, startImg, 0, -400, 0, 0, 0, 0);
    SpriteScale(2, 500, 256, 1);
    SpriteFlag (2, true);
    buttonScale = vec3(1, 1, 1);

    // Tween 初期設定
    buttonScale = startScale[0];
    tweenObj->SetTween(&buttonScale, &startScale[0], 2, 3, true);


	// 音楽再生
	// ループ再生
	musicSys->Loop(true);
	musicSys->Play();


	// 最初はキーを押せないようにする。
	titleKeyFlag = true;


    // フェードイン初期化
    fadeInOutObj->SetColor( vec4(1,1,1,1) );
    fadeInOutObj->SetSpeed(0.5f);
    fadeInOutObj->Init(FadeMode::FadeInTex);


    // スレッド登録
    threadManagerObj->SetThrade(fadeInOutObj->ThreadFadeMove, fadeInOutObj, "finout");
    threadManagerObj->SetThrade(tweenObj->ThreadTweenMove,    tweenObj, "tween");


    printf("タイトルチェック\n");
}



// メイン
void TitleSys::TitleMain(void)
{
    // キー入力
    titleKeyIn = GetAsyncKeyState(VK_SPACE);
    titleKeyIn |= GetAsyncKeyState(VK_F1);
    if (titleKeyIn & 0x8000)
    {
        if (titleKeyFlag == false)
        {
            titleKeyFlag = true;
            titleFlag = TitleFlag::END;
        }
    }
    else
    {
        // 押してないときの処理
        titleKeyFlag = false;
    }


    // DEBUG TweenMove のリセット
    short akey = GetAsyncKeyState('A');
    if (akey & 0x8000)
    {
        tweenObj->ResetMove(&logoPos);
    }


    // スタートボタンの拡大縮小
    SpriteScale(2, 500* buttonScale.x, 256* buttonScale.y, 1);

    // タイトルの移動処理
    SpritePos(1, logoPos);

    // Tween 処理
    //tweenObj.TweenMove();

    // フェードイン処理
    //fadeInOutObj->FadeMove();


	// 描画処理
	SysDraw3D();

}




// 終了処理
void TitleSys::TitleEnd(void)
{
	// タイトルフラグ切り替え
	titleFlag = TitleFlag::INIT;

	// ゲームフラグ切り替え
	mainSys->SetSceneFlag(SceneFlag::SELECT);

	// 音楽停止
	musicSys->StopAll();


    // スレッドの停止
    threadManagerObj->Delete("finout");
    threadManagerObj->Delete("tween");


	// 画像停止
	SpriteFlag(0, false);
	SpriteFlag(1, false);
	SpriteFlag(2, false);

}


//----------------------------------------------------------------------
// 描画処理
//----------------------------------------------------------------------
void TitleSys::SysDraw3D(void)
{
	// 背景色
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);

	// メインループ
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(1.0f, 1.0f, 1.0f);

	// ライトON
	glDisable(GL_LIGHTING);
	//	glEnable(GL_LIGHTING);

	// 行列の初期化
	glLoadIdentity();

	// 視点設定
	lookObj->Disp();


	// スプライト表示
	SpriteDraw();

    // フェードインアウト（表示）
    fadeInOutObj->Disp();

	// バッファスワップ
	glfwSwapBuffers(GlfwWindow);
	// イベントのポーリング
	glfwPollEvents();

}
