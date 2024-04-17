#pragma once

#include "Base.h"
#include "MainSys.h"

// 前方宣言
class MusicSys;
class HitModel;
class FadeInOut;
class CharPol;

// システム
enum class SelectFlag
{
	INIT,
	MAIN,
	END,
	MAX,
};


class SelectSys
{
private:
	// シーンフラグ
	MainSys *mainSys;

	// ミュージック
	MusicSys* musicSys;

	// セレクト管理フラグ
	SelectFlag selectFlag = SelectFlag::INIT;

	// テクスチャ番号
	GLint bgImageNo;
	GLint logoImageNo;
	GLint startImageNo;
	GLint testImageNo;

    // セレクトオブジェクト用モデル
    Base p1Obj[2];

	float selectObj[2] = { 0.0f, 0.0f };

	// セレクトキャラクタの移動
	vec3 offsetMov = vec3(0,0,0);

	int   tweenNoP1 = -1;
	int   tweenNoP2 = -1;

    vec4 selectBgColor[3];
    vec4 p2cpuBgColor[2];


	// データ管理フラグ（二重読み込み防止）
	bool dataFlag = false;
	
	// ロゴの位置
	vec3 logoPos;
	vec4 logoColor;

	// スタートボタン
	vec2  buttonScale;
	float buttonRot;
    float updownFlag = 1.0f;

	// フェードイン・アウト
	FadeInOut* fadeInOutObj;
	vec4 fadeColor;

	// キー入力
	short selectKeyIn;
	int   selectKeyFlag; //押しっぱなしフラグ

    // キー情報
    short keyFlag[256];

    // モデルデータNo
    int modelNo[2];
    int texNo[2];

public:
	// コンストラクタ
	SelectSys( MainSys* ms );

	// セレクト関数
	void Select(void);
	void SelectInit(void);
	void SelectMain(void);
	void SelectEnd(void);

    void KeyIn();

    void ModelDisp(Base* base, float blockRotY, vec3 offset);

	void SysDraw3D(void);

};