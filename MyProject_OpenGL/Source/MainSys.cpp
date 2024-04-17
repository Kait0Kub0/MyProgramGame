#include "MainSys.h"

#include "TitleSys.h"
#include "SelectSys.h"
#include "GameMainSys.h"
#include "EndingSys.h"

// コンストラクタ
MainSys :: MainSys()
{
	// 各システム初期化
	titleSys    = new TitleSys( this );
    selectSys   = new SelectSys( this );
	gameMainSys = new GameMainSys( this );
	endingSys   = new EndingSys( this );

	// FPS管理（スタート時間取得）
	fpsChk1 = timeGetTime();

}


// アクセサメソッド
void MainSys::SetSceneFlag(SceneFlag sf)
{
	sceneFlag = sf;
}


// ゲームシステム
bool MainSys :: GameSystem()
{

	// 経過時間の取得
	fpsChk2 = timeGetTime();

	// スタートからの経過時間計算
	DWORD chkTime = fpsChk2 - fpsChk1;

	// FPS時間経過チェック
	if (chkTime < FPSCOUNT)
	{
		// 1/60 経過していないので
		// 処理を抜ける
		return systemFlag;
	}

	// スタートタイムの更新
	fpsChk1 = fpsChk2;


	// ゲーム処理分岐
	switch (sceneFlag)
	{
	case SceneFlag::TITLE:
		titleSys->Title();
		break;

    case SceneFlag::SELECT:
        selectSys->Select();
        break;

	case SceneFlag::GAME:
		gameMainSys->Game();
		break;

	case SceneFlag::ENDING:
		endingSys->Ending();
		break;

	case SceneFlag::EXIT:
		systemFlag = false;
		break;
	}


	return systemFlag;
};
