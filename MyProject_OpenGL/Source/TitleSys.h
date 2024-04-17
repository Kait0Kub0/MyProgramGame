#pragma once

#include "Base.h"
#include "MainSys.h"

#include <cassert>
#include <thread>

// 前方宣言
class MusicSys;
class HitModel;
class FadeInOut;
class CharPol;

// システム
enum class TitleFlag
{
	INIT,
	MAIN,
	END,
	MAX,
};


// イベント
enum class TitleEvent
{
	TitleScrollIn,
	StartUp,
	KeyIn,
	TitleScrollOut,
	MAX,
};


class TitleSys
{
private:
	// シーンフラグ
	MainSys *mainSys;

	// ミュージック
	MusicSys* musicSys;

	// タイトル管理フラグ
	TitleFlag titleFlag = TitleFlag::INIT;

	// イベント管理フラグ
	TitleEvent titleEvent = TitleEvent::TitleScrollIn;

	// テクスチャ番号
	GLint bgImageNo;
	GLint logoImageNo;
	GLint startImageNo;
	GLint testImageNo;

	// データ管理フラグ（二重読み込み防止）
	bool dataFlag = false;
	
	// ロゴの位置
	vec3 logoPos;
	vec4 logoColor;

	// スタートボタン
	vec2  buttonScale;
	float buttonRot;
    float updownFlag = 1.0f;

    // フェードカラー
	vec4 fadeColor;

	// キー入力
	short titleKeyIn;
	int   titleKeyFlag; //押しっぱなしフラグ


	// テストキャラクタ
	CharPol* charPolObj;

public:
	// コンストラクタ
	TitleSys( MainSys* ms );

	// タイトル関数
	void Title(void);
	void TitleInit(void);
	void TitleMain(void);
	void TitleEnd(void);
	void SysDraw3D(void);

};