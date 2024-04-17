#include "FadeInOut.h"


// コンストラクタ
FadeInOut::FadeInOut()
{
	// テクスチャ番号
	texNo = -1;

	// カラー
	vec4 color = vec4(1, 1, 1, 1);

	// シェーダーNo
	shaderNo = -1;
}

// セッター
void FadeInOut::SetColor(vec4 c4)
{
	this->color = c4;
}
void FadeInOut::SetShader(GLint sn)
{
	this->shaderNo = sn;
}

// 初期化
void FadeInOut::Init(int mNo, int tNo)
{

}
void FadeInOut::Init()
{
    
}

// 表示処理
void FadeInOut::Disp()
{
	// シェーダープログラム設定
	if (shaderNo == -1)
	{
		shaderObj->ShaderOff();
	}
	else
	{
		shaderObj->ShaderOn(shaderNo);
		// カラーのセット
		shaderObj->SetColor(color);
	}
	// テクスチャON・OFFセット
	shaderObj->SetInt("textureFlag", texNo);


	// テクスチャON・OFF
	if (texNo == -1)
	{
		glDisable(GL_TEXTURE);
	}
	else
	{
		glEnable(GL_TEXTURE);
		glBindTexture(GL_TEXTURE_2D, texNo);
	}


	// マトリックスの保存
	glPushMatrix();
	
	// マトリックスの初期化
	glLoadIdentity();

	glTranslatef(0, 0, -1);

	vec4 uv = vec4(0, 0, 1, 1);

	glBegin(GL_QUADS);    // 4つ１組の頂点で四角形を描画
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


	// マトリックス取得
	glPopMatrix();
}
