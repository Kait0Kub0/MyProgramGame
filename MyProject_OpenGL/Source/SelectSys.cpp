#include "SelectSys.h"
#include "Music/MusicSys.h"
#include "GameParam.h"
#include "Look.h"
#include "Graphics/Tween/TweenSys.h"

vec3 tweenMoveP1[2] = { vec3(-3,0,0), vec3(0,0,0) };

// コンストラクタ
SelectSys::SelectSys( MainSys *ms )
{
	// メインシステムセット
	mainSys = ms;

	// ミュージック管理システム
	musicSys = new MusicSys();

    // テクスチャ番号
	bgImageNo = 0;
}



// セレクトシステム
void SelectSys::Select(void)
{
	switch ( selectFlag )
	{
	case SelectFlag::INIT:
		// セレクト初期化
		SelectInit();
		break;
	case SelectFlag::MAIN:
		// セレクトメイン
		SelectMain();
		break;
	case SelectFlag::END:
		// セレクト終了処理
		SelectEnd();
		break;
	default:
		break;
	}

	// 強制終了
	//sceneFlag = SceneFlag::EXIT;

	return;
}



// 初期化
void SelectSys::SelectInit(void)
{
	// セレクトメインに切替
	selectFlag = SelectFlag::MAIN;


	// カメラの初期化
	lookObj->Init();
	lookObj->pos = vec3(0, 0, 10);


	// データ登録
	if (dataFlag == false)
	{

		// 画像データ読み込み
        imageSysObj->SetImage("SelectBG.png");

        texNo[0] = imageSysObj->SetImage("miku.png");
        texNo[1] = imageSysObj->SetImage("dat2.bmp");

		// モデルデータの読み込み
        modelNo[0] = modelSysObj->LoadModel("miku.pol",  "char00");
        modelNo[1] = modelSysObj->LoadModel("dat2.pol",  "char01");

		// 音データ読み込み処理
		musicSys->SetMusicOgg("DAT/SOUND/mist.ogg");

		dataFlag = true;
	}


    //===========ループ時の初期化 ===========================
    // 

    // キャラクタモデル
    gameParam->charModeP1 = CharMode::Fighter01;

    offsetMov = vec3(0, 0, 0);
    // オブジェクトの初期化
    for (int i = 0; i < 2; i++)
    {
        p1Obj[i].pos = vec3(0 + (3 * i), -1.5, 0);
        p1Obj[i].dispFlag = true;
        p1Obj[i].scale = vec3(2, 2, 2);
    }

    selectObj[0] = 0;
    selectObj[1] = 0;

    vec3 oPos[3] = {
        { 0, -2,  -2},
        { 0, -2,  -2},
        { 0, -2,  -2},
    };

    vec3 oRot[3] = {
        {0, 0,   0},
        {0, 120, 0},
        {0, 240, 0},
    };

	// 音楽再生
	// ループ再生
	musicSys->Loop(true);
	musicSys->Play();


	// 最初はキーを押せないようにする。
	selectKeyFlag = true;


	printf("セレクトチェック\n");
}



// メイン
void SelectSys::SelectMain(void)
{
    // キー入力
   // selectKeyIn = GetAsyncKeyState(VK_SPACE);
    selectKeyIn = GetAsyncKeyState(VK_F1);
    if (selectKeyIn & 0x8000)
    {
        if (selectKeyFlag == false)
        {
            selectKeyFlag = true;
            selectFlag = SelectFlag::END;
        }
    }
    else
    {
        // 押してないときの処理
        selectKeyFlag = false;
    }


    // キー入力
    KeyIn();

    // tween の実行
    tweenObj->TweenMove();


	// 描画処理
	SysDraw3D();

}


// キー入力
void SelectSys::KeyIn()
{
    short keydat;

    // 右矢印キー
    keydat = GetAsyncKeyState(VK_RIGHT);
    if (keydat & 0x8000)
    {
        if (keyFlag[VK_RIGHT] == false)
        {
            keyFlag[VK_RIGHT] = true;

            tweenMoveP1[0] = offsetMov;
            tweenMoveP1[1] = vec3(0, 0, 0);

            if (tweenMoveP1[0] == tweenMoveP1[1])
            {
                return;
            }

            // キャラクタ切り替え
            tweenObj->SetTween(&offsetMov, tweenMoveP1, 2, 2);

            gameParam->charModeP1 = CharMode::Fighter01;

        }
    }
    else
    {
        keyFlag[VK_RIGHT] = false;
    }


    // 左矢印キー
    keydat = GetAsyncKeyState(VK_LEFT);
    if (keydat & 0x8000)
    {
        if (keyFlag[VK_LEFT] == false)
        {
            keyFlag[VK_LEFT] = true;
            tweenMoveP1[0] = offsetMov;
            tweenMoveP1[1] = vec3(-3, 0, 0);

            if (tweenMoveP1[0] == tweenMoveP1[1])
            {
                return;
            }

            // キャラクタ切り替え
            tweenObj->SetTween(&offsetMov, tweenMoveP1, 2, 2);
            gameParam->charModeP1 = CharMode::Fighter02;
        }
    }
    else
    {
        keyFlag[VK_LEFT] = false;
    }

    // シーン切り替え
    // スペースキー
    keydat = GetAsyncKeyState(VK_SPACE);
    if (keydat & 0x8000)
    {
        if (keyFlag[VK_SPACE] == false)
        {
            keyFlag[VK_SPACE] = true;

            selectFlag = SelectFlag::END;
        }
    }
    else
    {
        keyFlag[VK_SPACE] = false;
    }

}

// 終了処理
void SelectSys::SelectEnd(void)
{
	// セレクトフラグ切り替え
	selectFlag = SelectFlag::INIT;

	// ゲームフラグ切り替え
	mainSys->SetSceneFlag(SceneFlag::GAME);

	// 音楽停止
	musicSys->StopAll();

	// 画像停止
	SpriteFlag(0, false);

    // Tweenの削除
    tweenObj->Delet((void*)&offsetMov);
}


// モデル表示
void SelectSys::ModelDisp( Base *base , float blockRotY, vec3 offset)
{

    // シェーダー選択（off）
    shaderObj->ShaderOff();

    glDisable(GL_LIGHTING);

    // モデル表示
    for (int i = 0; i < 2; i++)
    {
        // マトリックスの保存
        glPushMatrix();

        // 平行移動
        mat4 transform0 = mat4(1.0);
        transform0[3][0] = (base + i)->pos.x + offset.x;
        transform0[3][1] = (base + i)->pos.y + offset.y;
        transform0[3][2] = (base + i)->pos.z + offset.z;

        // 回転
        float rotY = (base + i)->rot.y + 90.0f;
        (base + i)->rot.y += 0.5f;

        vec3 EulerAngles(0, radians(rotY), 0);
        quat modelQuaternion = quat(EulerAngles);
        mat4 transform1 = glm::toMat4(modelQuaternion);

        // 回転位置を変更する移動
        mat4 transform2 = mat4(1.0);
        transform2[3][0] = 0;
        transform2[3][1] = 0;
        transform2[3][2] = 1.0f;

        // 移動 * 回転 * 軸をずらす移動
        mat4 transform3 = transform0 * transform1 * transform2;

        glMultMatrixf((float*)&transform3);

        glScalef(base->scale.x, base->scale.y, base->scale.z);

        ModelData* modelData = modelSysObj->GetModelData(modelNo[i]);
        // テクスチャセット
        modelData->textureNo = texNo[i];
        // POLオブジェクト表示
        ModelDataDisp(modelData);


        // マトリックスの取得
        glPopMatrix();

    }

}


//----------------------------------------------------------------------
// 描画処理
//----------------------------------------------------------------------
void SelectSys::SysDraw3D(void)
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

    // モデル表示
    ModelDisp(p1Obj, selectObj[0], offsetMov);

	// バッファスワップ
	glfwSwapBuffers(GlfwWindow);
	// イベントのポーリング
	glfwPollEvents();

}
