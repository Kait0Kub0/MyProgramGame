#pragma once

#include "MainSys.h"


// 前方宣言
class Map;
class Player;
class Enemy;
class EnemySys;
class ExplosionSys;
class TTF_TEXT;
class WallSys;
class Victory;
class BulletSys;

#define DEF_KILLS_MAX   3
#define MODEL_MAX       10

#define SCREEN_OFFSET   2

// システム
enum class GameMainFlag
{
	INIT,
    SET_INIT,
	COUNT,
    MAIN,
    RESURT,
	END,
	EXIT,	// 強制終了
	MAX,
};


// 勝敗フラグ
enum class VictoryParam
{
    NON,
    P1_WIN,
    P2_WIN,
    ENEMY_WIN,
    DRAW,
    MAX
};


// ゲームメイン処理クラス
class GameMainSys
{
private:
	// シーンフラグ
	MainSys* mainSys;

	// ゲームメインフラグ
	GameMainFlag gameMainFlag = GameMainFlag::INIT;

	bool dataFlag = false;

	// マップ
	Map* mapObj = nullptr;

	// プレイヤー
    Player* player01Obj = nullptr;

	// エネミー
    Enemy* enemyObj;
	EnemySys* enemySysObj;

	// 爆発システム
	ExplosionSys* explosionSysObj;

    // 弾システム
    BulletSys* pBulletSysObj; // プレイヤー

    // スタート・エンディング
    vec3 startPos;
    vec3 endingWinPos;
    vec3 endingLosePos;
    vec3 endingDrawPos;


    // 壁のテスト
    WallSys* wall01SysObj;
    WallSys* wall02SysObj;
    WallSys* wall03SysObj;


    // 画像データ
    int charImage[MODEL_MAX];

    // モデルデータ
    int charModel[MODEL_MAX];

    // シェーダ番号
    GLint defShaderNo;

    // 勝敗のフラグ
    // 1: プレイヤー01の勝利
    // 2: プレイヤー02の勝利
    // 3: エネミーの勝利
    // 4: ドロー
    int victoryFlag = -1;

    // 勝敗表示
    Victory* victoryObj;
    int winImg;
    int loseImg;
    int drawImg;


    // カウントダウン
    int countImg[4];
    int countValue;
    int countTime;

public:

    // 撃破数
    int killsCount;

    // プレイヤーLows
    int playerCount;

	int gameKeyIn;
	int gameKeyFlag;
	int gameTestFlag;

    ivec4 viewSize[3];  // [0] 全画面
                        // [1] プレイヤー1
                        // [2] プレイヤー2


    vec4 paramPos;      // ポイント表示の基本座標


	GameMainSys(MainSys* ms);

	// タイトル関数
	void Game(void);
	void GameInit(void);
    void GameSetInit(void);
    void GameCount(void);
	void GameMain(void);
    void GameResurt(void);
	void GameEnd(void);

    void SysDraw3D(void);

    // 画面分割表示
    void SplitScreen();

    void ClearScreen(void);     // 画面クリア
    void SwapBuffer(void);      // ダブルバッファ切り替え

    // 勝敗チェック
    void VictoryCheck();

    // 画面設定
    void SetView(int no);


};


extern TTF_TEXT moji[10];
