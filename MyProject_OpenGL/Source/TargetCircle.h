#pragma once

#include "Base.h"
#include "TargetCircleSys.h"

#define TARGET_SET_TIME 1000

enum class TargetMode
{
	Non,
	TargetOn,
	LockOn,
	MissileOn
};

class Look;

class TargetCircle : public Base
{
private:

	// テクスチャ番号
	int texNo;
	// モデル番号
	int modelNo;

	// エネミーオブジェクト
	Base* enemyBase;

	// 視点座標
	Look* lookObj;

	// ターゲットタイム
	int targetTime;

	// ターゲットモード
	TargetMode mode;

	// ミサイルオブジェクト
	Base* missileBase;

	// カラー
	// 緑	:サイトON
	// 黄	:ターゲットロック
	// 赤	:ミサイルロック
	vec4 color;

public:
	// コンストラクタ
	TargetCircle();
	// デストラクタ
	~TargetCircle();

	// セッター・ゲッター
	void SetEnemy(Base* bs);
	void SetLook(Look* look);
	void SetMissile(Base* bs);
	void SetLockOn();

	TargetMode GetMode();

	// エネミーアドレスチェック
	bool CheckEnemy(Base* enemy);

	// 初期化
	void Init();
	void Init(GLuint texNo, int mdNo, int shaderNo);

	// 移動処理
	void Move();

	// 表示処理
	void Disp();

};