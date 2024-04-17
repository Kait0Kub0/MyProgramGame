#pragma once

#include "system.h"


enum class SceneFlag
{
	TITLE,
    SELECT,
	GAME,
	ENDING,
	EXIT,
	MAX
};



#define FPSCOUNT (1000/60)

// 前方宣言
class TitleSys;
class SelectSys;
class GameMainSys;
class EndingSys;


// メインシステムクラス
// FPS管理
// タイトル・ゲームメイン・エンディング管理
class MainSys
{
private:
	// システム起動フラグ
	bool systemFlag = true;

	// ゲームシーン
	SceneFlag sceneFlag = SceneFlag::TITLE;

	// FPS 管理
	DWORD fpsChk1; // スタート時間
	DWORD fpsChk2; // 経過時間

	// タイトル
	TitleSys*       titleSys;
    // セレクト
    SelectSys*      selectSys;
    // ゲームメイン
	GameMainSys*    gameMainSys;
	// エンディング
	EndingSys*      endingSys;

public:

	// コンストラクタ
	MainSys();

	// ゲームシステム変更処理
	void SetSceneFlag(SceneFlag sf);

	// ゲームシステム
	bool GameSystem();

};