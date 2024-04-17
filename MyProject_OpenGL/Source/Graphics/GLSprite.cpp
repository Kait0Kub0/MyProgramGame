//----------------------------------------------------------------------
//  Copyright (C) 2021 by Human
//----------------------------------------------------------------------

#include "../system.h"
#include "./Shader/Shader.h"

// デフォルトスプライト
Sprite	sprite[SPRITE_MAX];
Sprite* pSprite = sprite;

int spriteMax = SPRITE_MAX;

// スプライト変数切り替え
void SpriteSet(Sprite* p, int max )
{
	pSprite = p;
	spriteMax = max;
}


// 初期化
void SpriteInit(Sprite* pSprite, int max)
{
	for (int i=0; i<max; i++)
	{
		(pSprite + i)->flag = false;
		(pSprite + i)->pos.x = 0;
		(pSprite + i)->pos.y = 0;
		(pSprite + i)->pos.z = 0;
		(pSprite + i)->texNo = 0;
		(pSprite + i)->scale.x = 1;
		(pSprite + i)->scale.y = 1;
		(pSprite + i)->scale.z = 1;
		(pSprite + i)->uv.s = 0;
		(pSprite + i)->uv.t = 0;
		(pSprite + i)->uv.p = 1;
		(pSprite + i)->uv.q = 1;

		(pSprite + i)->shaderNo = -1;

	}
}


// パラメータセット
void SpriteParam(int no, int tno, float px, float py, float pz, float rx, float ry, float rz)
{
	if (no < 0 || no >= spriteMax)
	{
		return;
	}

	// テクスチャ番号
	(pSprite + no)->texNo = tno;

	// ポジション
	(pSprite + no)->pos.x = px;
	(pSprite + no)->pos.y = py;
	(pSprite + no)->pos.z = pz;
	// 回転量
	(pSprite + no)->rot.x = rx;
	(pSprite + no)->rot.y = ry;
	(pSprite + no)->rot.z = rz;
	// カラーデータの初期化
	(pSprite + no)->color = vec4(1, 1, 1, 1);

}

// スプライト表示フラグ
void SpriteFlag(int no, int flag)
{
	if (no < 0 || no >= spriteMax)
	{
		return;
	}

	(pSprite+no)->flag = flag;
}


// 全スプライトの設定
void SpriteFlagAll(int flag, int sNo, int eNo)
{
    // 初期化なし
    if (sNo == -1)
    {
        sNo = 0;
    }
    if (eNo == -1 || eNo >= spriteMax)
    {
        eNo = spriteMax - 1;
    }

    // エラーチェック
    if (sNo < 0 || eNo < 0)
    {
        return;
    }

    // 上記の if 文で 最大値-1 の処理を行う（ <= でもOK）
    for (int i = sNo; i <= eNo; i++)
    {
        (pSprite + i)->flag = flag;
    }
}


// スプライトテクスチャー
void SpriteTex(int no, int tno)
{
	if (no < 0 || no >= spriteMax)
	{
		return;
	}

	(pSprite + no)->texNo = tno;
}


// スプライト回転
void SpriteRot(int no, float rx, float ry, float rz)
{
	if (no < 0 || no >= spriteMax)
	{
		return;
	}

	(pSprite + no)->rot.x = rx;
	(pSprite + no)->rot.y = ry;
	(pSprite + no)->rot.z = rz;
}


// スプライトポジション(vec3型)
void SpritePos(int no, vec3 pos)
{
	if (no < 0 || no >= spriteMax)
	{
		return;
	}
	(pSprite + no)->pos = pos;
}

// スプライトポジション(x,y,z型)
void SpritePos(int no, float px, float py, float pz)
{
	if (no < 0 || no >= spriteMax)
	{
		return;
	}

(pSprite + no)->pos.x = px;
(pSprite + no)->pos.y = py;
(pSprite + no)->pos.z = pz;
}


// スプライト拡大縮小
void SpriteScale(int no, float sx, float sy, float sz)
{
	if (no < 0 || no >= spriteMax)
	{
		return;
	}

	(pSprite + no)->scale.x = sx;
	(pSprite + no)->scale.y = sy;
	(pSprite + no)->scale.z = sz;
}


// スプライトUV座標
void SpriteUV(int no, float u0, float v0, float u1, float v1)
{
	if (no < 0 || no >= spriteMax)
	{
		return;
	}

	(pSprite + no)->uv.s = u0;
	(pSprite + no)->uv.t = v0;
	(pSprite + no)->uv.p = u1;
	(pSprite + no)->uv.q = v1;
}


// カラー設定
void SpriteColor(int no, float r, float g, float b, float a)
{
	vec4 color = vec4(r, g, b, a);
	SpriteColor(no, color);
}
void SpriteColor(int no, vec4 color)
{
	if (no < 0 || no >= spriteMax)
	{
		return;
	}

	(pSprite + no)->color = color;
}



// スプライト表示（テクスチャ指定）
void SpriteDraw(Sprite* p, int max)
{
	pSprite = p;
	spriteMax = max;
	SpriteDraw();
}


// スプライト表示
void SpriteDraw()
{
	// ディプスバッファOFF
	glDisable(GL_DEPTH_TEST);

	// テクスチャ環境パラメータ設定
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// シェーディングモード
	glShadeModel(GL_SMOOTH);


	//  色属性（材質色）
	glMaterialfv(GL_FRONT, GL_AMBIENT, surface_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, surface_diffuse);


	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();


#if 0
	// 画面サイズの影響を受けない
	GLint viewport[4];							//ビューポート
	glGetIntegerv(GL_VIEWPORT, viewport);		//ビューポート取得
	float sizew = (float)viewport[2];
	float sizeh = (float)viewport[3];
	glOrtho(-sizew, sizew, -sizeh, sizeh, -1000, 1000); //左、右、下、上、手前、奥で囲まれる立方体の範囲を並行投影
#else
	// 初期画面サイズを基準とする
	float sizew = SCREEN_WIDTH;
	float sizeh = SCREEN_HEIGHT;
	// 平行投影
    glOrtho(-sizew, sizew, -sizeh, sizeh, -1000, 1000); //左、右、下、上、手前、奥で囲まれる立方体の範囲を並行投影
#endif


	glMatrixMode(GL_MODELVIEW);

    glPushMatrix();     // 現在のマトリックスの保存

    glLoadIdentity();   // マトリックスの初期化

	for (int lp = 0; lp < spriteMax; lp++)
	{
		if ((pSprite + lp)->flag == FALSE)
		{
			continue;
		}

		glPushMatrix();
		{
			int TexNo  = (pSprite + lp)->texNo;
			vec3 pos   = (pSprite + lp)->pos;
			vec3 rot   = (pSprite + lp)->rot;
			vec3 scale = (pSprite + lp)->scale;
			vec4 uv    = (pSprite + lp)->uv;
			vec4 color = (pSprite + lp)->color;

			// シェーダー番号取得
			int shaderNo = (pSprite + lp)->shaderNo;

			// テクスチャセット（シェーダー対応）
			if (shaderNo == -1 )
			{
				shaderObj->ShaderOff();
			}
			else
			{
				// シェーダセット
				shaderObj->ShaderOn(shaderNo);
				// シェーダーテマルチクスチャセット
				shaderObj->SetTexture(0, 0);
				// カラーデータセット
				shaderObj->SetColor(color);
			}

			// ポリゴンに貼り付けるテクスチャの選択
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, TexNo);


			// 平行移動
			glTranslatef(pos.x, pos.y, pos.z);


			// 回転処理 角度・X・Y・Z
			glRotatef(rot.x, 1.0f, 0.0f, 0.0f);
			glRotatef(rot.y, 0.0f, 1.0f, 0.0f);
			glRotatef(rot.z, 0.0f, 0.0f, 1.0f);


			glScalef(scale.x, scale.y, scale.z);

			glBegin(GL_QUADS);    // 4つ１組の頂点で四角形を描画

			// s[0]
			// t[1]
			// p[2]
			// q[3]

											  // 第一頂点
			glTexCoord2f(1.0f, 0.0f);         // テクスチャ座標 S T 座標
			glTexCoord2f(uv.p, uv.t);         // テクスチャ座標 S T 座標
			glVertex3f(1.0f, -1.0f, 0.0f);    // 頂点座標
											  // 第二頂点
			glTexCoord2f(1.0f, 1.0f);         // テクスチャ座標 S T 座標
			glTexCoord2f(uv.p, uv.q);         // テクスチャ座標 S T 座標
			glVertex3f(1.0f, 1.0f, 0.0f);     // 頂点座標
											  // 第三頂点
			glTexCoord2f(0.0f, 1.0f);         // テクスチャ座標 S T 座標
			glTexCoord2f(uv.s, uv.q);         // テクスチャ座標 S T 座標
			glVertex3f(-1.0f, 1.0f, 0.0f);    // 頂点座標
											  // 第四頂点
			glTexCoord2f(0.0f, 0.0f);         // テクスチャ座標 S T 座標
			glTexCoord2f(uv.s, uv.t);         // テクスチャ座標 S T 座標
			glVertex3f(-1.0f, -1.0f, 0.0f);   // 頂点座標
			glEnd();
		}
		glPopMatrix();
	}

    glPopMatrix(); // マトリックスの取り出し

    // プロジェクションマトリックスを戻す
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

    // マトリックスの切り替え
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_DEPTH_TEST);

}


// シェーダー番号セット
void SpriteShader(int no, GLint sNo )
{

	if (no < 0 || no >= spriteMax || sNo < 0 )
	{
		return;
	}

	(pSprite + no)->shaderNo = sNo;


}


// 全スプライトにシェーダーセット
void SpriteShaderAll(GLint sNo)
{

	if (sNo < 0)
	{
		return;
	}

	for (int i = 0; i < spriteMax; i++)
	{
		(pSprite + i)->shaderNo = sNo;
	}

}