#include "TargetCircle.h"
#include "Look.h"

// コンストラクタ
TargetCircle::TargetCircle()
{
	texNo = 0;
	modelNo = 0;
	targetTime = 0;
	enemyBase = nullptr;
	lookObj = nullptr;
	missileBase = nullptr;
	mode = TargetMode::Non;
	color = vec4(1, 0, 0, 1);
}

// デストラクタ
TargetCircle::~TargetCircle()
{

}

// セッター
void TargetCircle::SetEnemy(Base* bs)
{
	// エラーチェック
	if (bs == nullptr)
	{
		cout << "TargetCircle::SetEnemyの引数が不正です" << endl;
		return;
	}

	enemyBase = bs;

	// ターゲット開始
	targetTime = timeGetTime(); // 現在時間の取得
	dispFlag = true;			// 表示フラグON
	color = vec4(0, 1, 0, 1);	// 緑:ターゲットON
	mode = TargetMode::TargetOn;
}
void TargetCircle::SetLook(Look* look)
{
	// エラーチェック
	if (look == nullptr)
	{
		cout << "TargetCircle::SetLookの引数が不正です" << endl;
		return;
	}

	lookObj = look;
}
void TargetCircle::SetMissile(Base* bs)
{
	// エラーチェック
	if (bs == nullptr)
	{
		cout << "TargetCircle::SetMissleの引数が不正です" << endl;
		return;
	}

	missileBase = bs;

	// ミサイル追尾状態
	mode = TargetMode::MissileOn;
	color = vec4(1, 1, 0, 1);	// 黄色:ミサイル追尾
}

void TargetCircle::SetLockOn()
{
	mode = TargetMode::LockOn;
	color = vec4(1, 0, 0, 1); // 赤色：ロックオン状態
}

// ターゲット状態
TargetMode TargetCircle::GetMode()
{
	return mode;
}

// エネミーアドレスチェック
bool TargetCircle::CheckEnemy(Base* enemy)
{
	if (enemyBase == enemy)
	{
		return true;
	}
	return false;
}

// 初期化
void TargetCircle::Init()
{
	dispFlag = false;

	// ポジション
	pos = vec3(0, 0, 0);
	// 回転
	rot = vec3(0, 0, 0);
	// スケール
	scale = vec3(1, 1, 1);
	// カラー
	color = vec4(1, 1, 1, 1);
	// ポインタ
	enemyBase = nullptr;
	missileBase = nullptr;
	// フラグ

	targetTime = 0;

}

void TargetCircle::Init(GLuint texNo, int mdNo, int shaderNo)
{
	// モデルデータの設定
	this->modelNo = mdNo;

	// テクスチャ番号保存
	this->texNo = texNo;

	// シェーダー番号
	this->shaderNo = shaderNo;
}

// 移動処理
void TargetCircle::Move()
{
	// 分岐処理
	switch (mode)
	{
	case TargetMode::Non:
		dispFlag = false;
		return;
		break;
	case TargetMode::TargetOn:
	{
		// 時間のチェック
		int workTime = timeGetTime() - targetTime;
		if (workTime > TARGET_SET_TIME)
		{
			SetLockOn();
		}
	}
	break;
	case TargetMode::LockOn:
		// エネミーチェック
		if (enemyBase->dispFlag == false)
		{
			// 初期化してループをぬける
			Init();
			return;
		}
		break;
	case TargetMode::MissileOn:
		// ミサイルチェック
		if (missileBase->dispFlag == false)
		{
			// 初期化してループをぬける
			Init();
			return;
		}
		break;
	default:
		break;
	}

	// 視点方向へ回転(ビルボード)
	pos = enemyBase->pos;
	// 視点方向ベクトル
	vec3 lookVec = lookObj->pos - pos;
	// 単位ベクトルへ変換
	lookVec = glm::normalize(lookVec);
	// 回転
	myQuaternion = glm::quatLookAt(lookVec, lookObj->upVec);


}

// 表示処理
void TargetCircle::Disp()
{
	// 生存チェック
	if (dispFlag == false)
	{
		return;
	}


	// シェーダー選択
	shaderObj->ShaderOn(shaderNo);

	// カラーデータのセット
	shaderObj->SetVec4("uColor", color);


	// ポリゴンモデルの表示
	// マトリックスの保存
	glPushMatrix();

	mat4 transform = toMat4(myQuaternion);

	// 平行移動
	transform[3][0] = pos.x;
	transform[3][1] = pos.y;
	transform[3][2] = pos.z;

	glMultMatrixf((float*)&transform);

	glScalef(scale.x, scale.y, scale.z);

	// モデル表示
	ModelData* modelData = modelSysObj->GetModelData(modelNo);
	// テクスチャセット
	modelData->textureNo = texNo;
	// POLオブジェクト表示
	ModelDataDisp(modelData);


	// マトリックスの取得
	glPopMatrix();
}
