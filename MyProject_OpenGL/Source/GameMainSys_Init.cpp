
#include "GameMainSys.h"
#include "Music/MusicSys.h"
#include "Look.h"
#include "Map.h"
#include "Player.h"
#include "EnemySys.h"
#include "ExplosionSys.h"
#include "GameParam.h"
#include "BulletSys.h"

// 初期化
void GameMainSys::GameInit(void)
{
    // モード切り替え
    gameMainFlag = GameMainFlag::SET_INIT;

    // 最初はキーを押せないようにする。
    gameKeyFlag = true;

    // 視点初期
    lookObj->Init();
    lookObj->gazePos = vec3(0, 0, 0);	// 注視点
    lookObj->pos = vec3(0, 5, 10);	// 視点の位置


    if (dataFlag == false)
    {
        // データの二重読み込みの防止フラグ
        dataFlag = true;

        // 画像データ読み込み
        charImage[0] = imageSysObj->SetImage("miku.png");
        charImage[1] = imageSysObj->SetImage("dat2.bmp");
        charImage[2] = imageSysObj->SetImage("UFO.png");

        int sphereImg = imageSysObj->SetImage("SphereMap2.png");
        int mapImg = imageSysObj->SetImage("map.png");
        int pBulletImg = imageSysObj->SetImage("dat0r.bmp");

        winImg  = imageSysObj->SetImage("YouWin.png");
        loseImg = imageSysObj->SetImage("YouLose.png");

        countImg[0] = imageSysObj->SetImage("Count_3.png");
        countImg[1] = imageSysObj->SetImage("Count_2.png");
        countImg[2] = imageSysObj->SetImage("Count_1.png");
        countImg[3] = imageSysObj->SetImage("START_1.png");

        //moji[0].BackTexture(true, flameImg);


        // モデルデータ読み込み
        charModel[0] = modelSysObj->LoadModel("miku.pol", "Fighter01");
        charModel[1] = modelSysObj->LoadModel("dat2.pol", "Fighter02");
        charModel[2] = modelSysObj->LoadModel("ufo_GL.pol", "enemy");

        int sphereModel = modelSysObj->LoadModel("SphereMap.pol", "miku");
        int mapModel = modelSysObj->LoadModel("map_test.pol", "map");
        int bulletModel = modelSysObj->LoadModel("Bullet.pol", "Bullet");
        int weaponModel1 = modelSysObj->LoadModel("weapon_1.pol", "weapon_1");

        // シェーダー読み込み
        defShaderNo
            = shaderObj->Load(
                "GLSL/Vsp.txt",
                "GLSL/Fsp.txt");

        GLint explosionShaderNo
            = shaderObj->Load(
                "GLSL/ExplosionVsp.txt",
                "GLSL/ExplosionFsp.txt"
            );

        GLint wallShaderNo
            = shaderObj->Load(
                "GLSL/WallVsp.txt",
                "GLSL/WallFsp.txt"
            );

        // 爆発の初期化
        //explosionSysObj->Init(explosionImg,
        //    explosionModel,
        //    explosionShaderNo);

        // マップモデル
        // 画像No, ポリゴンモデルNo, シェーダーNo
        mapObj->SphereInit(sphereImg, sphereModel, defShaderNo);
        mapObj->GroundInit(mapImg, mapModel, defShaderNo);

        // エネミー初期化
        enemySysObj->Init(charImage[2], charModel[2], defShaderNo);
    
        // 弾の初期化
        pBulletSysObj->Init(pBulletImg, bulletModel, defShaderNo);
    }

    //============= ループ時の初期化 =====================

    // セレクト処理対応初期化
    int objNo = (int)gameParam->charModeP1;;

    // エネミー初期化
    enemySysObj->Init();

    // プレイヤー初期化
    player01Obj->Init(charImage[objNo], charModel[objNo], defShaderNo);
    player01Obj->SetEnemySys(enemySysObj);
    player01Obj->SetBulletSys(pBulletSysObj);

    lookObj->SetBase(player01Obj, player01Obj->radius / 2, -0.1f);

    enemySysObj->SetPlayer(player01Obj);

    // マップの初期化
    mapObj->Init();
    // プレイヤー情報セット
    mapObj->SetPlayer(player01Obj);

    // エネミー撃破数初期化
    killsCount = 0;
    // プレイヤー残機
    playerCount = 1;


    // 文字表示設定

    // 使い方表示
    moji[0].FontName(L"HGP創英角ﾎﾟｯﾌﾟ体");					// フォント選択
    moji[0].FontSize(32, 1000);							    // 文字サイズ・太さ(0～1000)
    moji[0].FrontColor(0, 0, 1.0f, 1.0f);				    // 文字の色
    moji[0].Parameteri(TTF_TEXT_BACK, false);				// 背景枠	ON・OFF
    moji[0].BackPoint (TTF_TEXT_DISP_TOP_LEFT);			    // 表示ポイント左上
    //moji[0].BackPoint (TTF_TEXT_DISP_OFFSET, -500, -20);	// 表示ポイント 設定
    moji[0].BackSize(900, 300);							    // 枠サイズ
    moji[0].BackColor(1.0f, 1.0f, 1.0f, 0.3f);				// 背景カラー
    moji[0].Padding(30, 30, 50, 10);						// 背景との隙間

    int a = 500;
    moji[0].String(L"文字表示 %d", a);

    // スプライトセット
    // 読み込んだ画像の管理番号取得
    // カウントダウンの初期化
    for (int i = 0; i < 4; i++)
    {
        startPos = vec3(0, 0, 0);
        SpriteParam(i, countImg[i], 0, 0, 0, 0, 0, 0);
        SpritePos  (i, startPos);
        SpriteScale(i, 256, 256, 1);
        SpriteColor(i, 1, 1, 1, 1);
        SpriteFlag (i, false);
    }


    // WINの初期化
    endingWinPos = vec3(0, 0, 0);
    SpriteParam(4, winImg, 0, 0, 0, 0, 0, 0);
    SpritePos  (4, endingWinPos);
    SpriteScale(4, 512, 256, 1);
    SpriteFlag (4, false);


    // Loseの初期化
    endingLosePos = vec3(0, 0, 0);
    SpriteParam(5, loseImg, 0, 0, 0, 0, 0, 0);
    SpritePos  (5, endingLosePos);
    SpriteScale(5, 512, 256, 1);
    SpriteFlag (5, false);

} // GameInit ここまで初期化処理
