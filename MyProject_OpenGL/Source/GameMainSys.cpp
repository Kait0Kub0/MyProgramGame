#include "GameMainSys.h"
#include "Music/MusicSys.h"
#include "Look.h"
#include "Map.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemySys.h"
#include "ExplosionSys.h"
#include "GameParam.h"
#include "Victory.h"
#include "GameParam.h"
#include "BulletSys.h"


// 文字出力用変数
TTF_TEXT moji[10];


// コンストラクタ
GameMainSys::GameMainSys( MainSys *ms)
{
	// メインシステムセット
	mainSys = ms;

	// フラグ初期化
	gameMainFlag = GameMainFlag::INIT;
	gameKeyFlag = 0;
	gameKeyFlag = true;

	// マップ実体化
	mapObj = new Map();

	// プレイヤー実体化
    player01Obj = new Player(this);

    // エネミー実体化
    enemySysObj = new EnemySys(this);

	// 爆発の実体化
	explosionSysObj = new ExplosionSys();

    // 弾の実体化
    pBulletSysObj = new BulletSys();

    // 勝敗表示実体化
    victoryObj = new Victory();
}


// システム
void GameMainSys::Game(void)
{
	switch (gameMainFlag)
	{
	case GameMainFlag::INIT:
		// ゲームメイン初期化
		GameInit();
		break;

    case GameMainFlag::SET_INIT:
        // ポイント取得のループ初期化
        GameSetInit();
        break;

    case GameMainFlag::COUNT:
        // スタートカウント
        GameCount();
        break;

    case GameMainFlag::MAIN:
		// ゲームメイン
		GameMain();
		break;

    case GameMainFlag::RESURT:
        // 結果表示
        GameResurt();
        break;

	case GameMainFlag::END:
		// ゲームメイン終了処理
		GameEnd();
		break;

	case GameMainFlag::EXIT:
		// 強制終了
		windowsLoop = false;
		break;

	default:
		break;
	}

	return;
}



// 初期位置設定
void GameMainSys::GameSetInit(void)
{

    // 勝敗フラグ
    victoryFlag = (int)VictoryParam::NON;

    // カウント初期化
    countValue = 0;
    countTime = timeGetTime();

    // モード切り替え
    gameMainFlag = GameMainFlag::MAIN;
}


// カウントダウン
void GameMainSys::GameCount(void)
{
    // カウント表示
    SpriteFlag(countValue, true);

    // 時間のチェック
    int stTime = timeGetTime();
    
    if ((stTime - countTime) > 1000)
    {
        // 時間の初期化
        countTime = stTime;

        // スプライトOFF
        SpriteFlag(countValue, false);

        // カウントアップ
        countValue++;

        // カウント終了
        if (countValue > 3)
        {
            // モード切り替え
            gameMainFlag = GameMainFlag::MAIN;
        }

    }
     
    // 表示処理

    // 画面分割表示
    SplitScreen();

    // ダブルバッファ画面切り替え
    SwapBuffer();

}


// 結果表示
void GameMainSys::GameResurt(void)
{

    // キー入力
    gameKeyIn = GetAsyncKeyState(VK_RETURN);
    if (gameKeyIn & 0x8000)
    {
        // モード切り替え
        gameMainFlag = GameMainFlag::END;

    }

    // 描画処理
    SplitScreen();

    // ダブルバッファ画面切り替え
    SwapBuffer();
}



//終了処理
void GameMainSys::GameEnd(void)
{
	// ゲームメインフラグ切り替え
	gameMainFlag = GameMainFlag::INIT;

    // スプライト初期化
    SpriteFlagAll(false);

    // 画面初期化
    SetView(0);
    
	// ゲームフラグ切り替え
	mainSys->SetSceneFlag(SceneFlag::ENDING);
}


// 勝敗チェック
// 0: 戦闘中
// 1: プレイヤー01の勝利
// 2: プレイヤー02の勝利
// 3: エネミーの勝利
// 4: ドロー
void GameMainSys::VictoryCheck()
{
    if (player01Obj->dispFlag == true
        && enemyObj->dispFlag == false)
    {
        victoryFlag = (int)VictoryParam::P1_WIN;
    }

    if (player01Obj->dispFlag == false
        && enemyObj->dispFlag == true)
    {
        victoryFlag = (int)VictoryParam::ENEMY_WIN;
    }

    // 勝敗決定
    if (victoryFlag != (int)VictoryParam::NON)
    {
        // モード切り替え
        gameMainFlag = GameMainFlag::RESURT;
    }

}


//----------------------------------------------------------------------
// 描画処理
//----------------------------------------------------------------------
void GameMainSys::SysDraw3D(void)
{
	// 背景色
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    ClearScreen();

	glColor3f(1.0f, 1.0f, 1.0f);

	// ライトOFF
	//glDisable(GL_LIGHTING);
	glEnable(GL_LIGHTING);

	// 行列の初期化
	glLoadIdentity();

	// 視点の設定
	lookObj->Disp();

	// マップの表示
	mapObj->Disp();

	// キャラクタの表示
    //player01Obj->Disp();

	// エネミー表示
	enemySysObj->Disp();

	// 爆発の表示
	//explosionSysObj->Disp();

    // 弾の表示
    pBulletSysObj->Disp();

    // スプライトの表示
    SpriteDraw();

	// 文字表示
    vec4 mPos = paramPos;
    float xPos = mPos.z / 4.0;  // 幅
    float yPos = mPos.w / 2.0;  // 高さ
    //moji[0].String(L"ポイント %d", gameParam->p1Point);
    //moji[0].Print(-xPos, yPos);  // 説明

}


// ビューポートセット
void GameMainSys::SetView(int no)
{
    vec4 view = vec4(0, 0, clientW, clientH);

    switch (no)
    {
    case 0: // 全画面
        viewSize[0].x = (int)view.x; // 開始座標
        viewSize[0].y = (int)view.y; // 開始座標
        viewSize[0].z = (int)view.z; // 横幅
        viewSize[0].w = (int)view.w; // 縦幅

        // 文字表示場所設定
        paramPos = viewSize[0];

        break;
    case 1: // プレイヤー１
        viewSize[1].x = (int)view.x; // 開始座標
        viewSize[1].y = (int)(view.y + (view.w/2.0f));  // 開始座標
        viewSize[1].z = (int)view.z; // 横幅
        viewSize[1].w = (int)(view.w / 2.0f) - SCREEN_OFFSET; // 縦幅

        // 文字表示場所設定
        paramPos = viewSize[1];

        break;
    case 2: // プレイヤー２
        viewSize[2].x = (int)view.x; // 開始座標
        viewSize[2].y = (int)view.y; // 開始座標
        viewSize[2].z = (int)view.z; // 横幅
        viewSize[2].w = (int)(view.w / 2.0f) - SCREEN_OFFSET; // 縦幅
        break;
    }

    // ビューポート設定
    glViewport( viewSize[no].x,
                viewSize[no].y,
                viewSize[no].z,
                viewSize[no].w );

    // シザー（描画範囲の設定）
    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSize[no].x,
              viewSize[no].y,
              viewSize[no].z,
              viewSize[no].w);

    //マトリックス変更（プロジェクションマトリックス）
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // パース設定
    gluPerspective(
        45.0,				                                //画角
        (float)viewSize[no].z / (float)viewSize[no].w,      //アスペクト比
        0.1,				                                //Near クリップ
        2000.0				                                //Far  クリップ
    );

    //マトリックス変更（モデルビューマトリックス）
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}
