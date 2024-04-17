#include "GameMainSys.h"
#include "Music/MusicSys.h"
#include "Look.h"
#include "Map.h"
#include "Player.h"
#include "EnemySys.h"
#include "ExplosionSys.h"
#include "GameParam.h"
#include "Hitchk.h"
#include "BulletSys.h"


// メイン
void GameMainSys::GameMain(void)
{

    // キー入力
    gameKeyIn = GetAsyncKeyState(VK_ESCAPE);
    if (gameKeyIn & 0x8000)
    {
        gameMainFlag = GameMainFlag::EXIT;
    }


    gameKeyIn = GetAsyncKeyState(VK_F1);
    if (gameKeyIn & 0x8000)
    {
        if (gameKeyFlag == false)
        {
            gameKeyFlag = true;
            gameMainFlag = GameMainFlag::END;
        }
    }
    else
    {
        // 押してないときの処理
        gameKeyFlag = false;
    }


    // キー入力
    player01Obj->KeyIn();
    //player01Obj->KeyIn_Debug();
    //enemyObj->KeyIn();

    // エネミーAI
    //enemySysObj->Move();

    // 移動処理
    player01Obj->LockOn();
    player01Obj->Move();

    // 視点の移動
    lookObj->Move();

    // 弾の移動処理
    pBulletSysObj->Move();

    // 当たり判定

    // マップの移動
    mapObj->Move();

    // 画面分割表示
    SplitScreen();

    // 勝敗チェック
    //VictoryCheck();

    // ダブルバッファ画面切り替え
    SwapBuffer();
}


// 画面分割表示
void GameMainSys::SplitScreen()
{

    // 画面クリア
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    ClearScreen();

    // スプライトOFF
    SpriteFlagAll(false, 4, 6);

    // プレイヤー01の勝敗
    if ( victoryFlag == (int)VictoryParam::P1_WIN )
    {
        SpriteFlag(4, true); // 勝ち
    }
    if (victoryFlag == (int)VictoryParam::ENEMY_WIN)
    {
        SpriteFlag(5, true); // 負け
    }

    // 描画処理(画面１)
    SysDraw3D();

}


// 画面クリア処理
void GameMainSys::ClearScreen()
{
    // バッファのクリア
    // カラーバッファ：ダブルバッファの描画面
    // ディプスバッファ：1ドット毎の深さ情報を持つバッファ
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}


// バッファの切替処理
void GameMainSys::SwapBuffer()
{
    // バッファスワップ
    glfwSwapBuffers(GlfwWindow);
    // イベントのポーリング
    glfwPollEvents();

}