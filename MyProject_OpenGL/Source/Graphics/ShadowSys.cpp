#include "../system.h"
#include "ShadowSys.h"

// シャドウ処理実体
ShadowSys* shadowSysObj = new ShadowSys();


ShadowSys::ShadowSys()
{
    dynamicTextureID = 0;
    frameBuffersID   = 0;
    viewport         = ivec4(0, 0, 0, 0);
    projection       = dmat4(0);

    lightPos         = vec3(0, 5, 0);

    mapDispFlag      = false;
}


void ShadowSys::SetLightPos(vec3 pos)
{
    lightPos = pos;
}


// シャドウマップ（シャドウテクスチャ）のセット
void ShadowSys::SetShadowShader(GLint shaderNo)
{
    if (mapDispFlag == false)
    {
        // シェーダーON
        shaderObj->ShaderOn(shaderNo);
        // シェーダーテクスチャセット
        shaderObj->SetShadowTexture(7);
    }
    else
    {
        // シェーダーOFF
        shaderObj->ShaderOff();
    }
}



// 初期化
void ShadowSys::Init()
{
    // ダイナミックテクスチャ（シャドウマップ登録）

    // 空テクスチャ登録
    glGenTextures(1, &dynamicTextureID);
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, dynamicTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 FBUFFER_WIDTH, FBUFFER_HEIGHT,
                 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // テクスチャ座標値の R(GLSL:P) とテクスチャとの比較 ON
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

    // RR(GLSL:P) の値がテクスチャの値以下なら真
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

    // 比較の結果をディプス値として得る
    glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);

    // フレームバッファの登録
    glGenFramebuffers(1, &frameBuffersID);              // 管理番号作成
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffersID);  // フレームバッファ選択

    // フレームバッファオブジェクトにディプスバッファ用のテクスチャを結合
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, dynamicTextureID, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, NULL); // フレームバッファ OFF

    // シャドウマップ表示フラグ
    mapDispFlag = false;

}




// シャドウスタート
void ShadowSys::ShadowStart()
{
    // シェーダプログラム停止
    glUseProgram(NULL);

    // テクスチャへ描画
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.1f, 4.0f);

    // 現在のビューポート・プロジェクションマトリックスを保存しておく
    glGetIntegerv(GL_VIEWPORT, (GLint *)&viewport);
    glGetDoublev(GL_PROJECTION_MATRIX, (GLdouble*)&projection);

    // テクスチャの選択
    //glEnable(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D, dynamicTextureID);
    // フレームバッファオブジェクトへのレンダリング開始
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffersID);

    // カラーバッファへの書込み禁止
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    // ライト OFF
    glDisable(GL_LIGHTING);

    // バッファをクリアする
    glClear(GL_DEPTH_BUFFER_BIT);

    // パース設定の保存
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glViewport(0, 0, FBUFFER_WIDTH, FBUFFER_HEIGHT);
    gluPerspective(
        45.0, 
        (GLdouble)FBUFFER_WIDTH / (GLdouble)FBUFFER_HEIGHT,
        0.1, 1000.0);

    gluLookAt( lightPos.x, lightPos.y, lightPos.z,
               0.0, 0.0, 0.0,
               0.0, 1.0, 0.0);
    
    glGetDoublev(GL_MODELVIEW_MATRIX, (GLdouble*)&modelview);

    // シャドウマップ描画フラグ
    mapDispFlag = true;

    // 深さ上方を書き込みするから
    // 裏ポリゴンのみ表示する
    // 注意：１枚ポリゴン（裏のないポリゴン）は
    //       影が発生しない
    glCullFace(GL_FRONT);
}

// シャドウエンド
void ShadowSys::ShadowEnd()
{
    // 表ポリゴンの表示へ戻す
    glCullFace(GL_BACK);

    // フレームバッファオブジェクトへのレンダリング終了
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // カラーバッファへの書き込み許可
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    // 通常の描画の設定に戻す
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
    // プロジェクションマトリックスの設定
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd((GLdouble*)&projection);

    // モデルビューマトリックスに変更
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // シャドウマップ描画フラグ
    mapDispFlag = false;


    glActiveTexture(GL_TEXTURE0);
}

// テクスチャマトリックスセット
// glLookat後に行う
void ShadowSys::SetTextureMatrix()
{
    dmat4 getLookat;

    glMatrixMode(GL_MODELVIEW);
    glGetDoublev(GL_MODELVIEW_MATRIX, (GLdouble*)&getLookat);
    // 逆行列に変換
    getLookat = glm::inverse(getLookat);

    // 光源のマトリックスを掛け合わせる
    glMatrixMode(GL_TEXTURE);
    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, dynamicTextureID);
    glLoadIdentity();
    // テクスチャ座標の [-1,1] の範囲を [0,1] の範囲に収める
    glTranslated(0.5, 0.5, 0.5);
    glScaled(0.5, 0.5, 0.5);

    // テクスチャ行列と光源の透視変換行列をかける
    glMultMatrixd((GLdouble*)&modelview);
    glMultMatrixd((GLdouble*)&getLookat);

    glMatrixMode(GL_MODELVIEW);

}


// シャドウマップ表示処理
void ShadowSys::ShadowDisp()
{
     // シェーダーOFF
    shaderObj->ShaderOff();
    //shaderObj->ShaderOn();

    glColor4f(1, 1, 1, 1);

    // ディプスバッファOFF
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glLoadIdentity();


    glDisable(GL_LIGHTING);

    //glDisable(GL_TEXTURE);
    glEnable(GL_TEXTURE);

    // テクスチャ環境パラメータ設定
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // ポリゴンに貼り付けるテクスチャの選択
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, dynamicTextureID);
    // テクスチャ座標値の R(GLSL:P) とテクスチャとの比較 OFF
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);

    // 平行移動
    glTranslatef(3, 2, -8);

    // 回転処理 角度・X・Y・Z
    glRotatef(0, 1.0f, 0.0f, 0.0f);
    glRotatef(0, 0.0f, 1.0f, 0.0f);
    glRotatef(0, 0.0f, 0.0f, 1.0f);

    glScalef(1, 1, 1);

    glBegin(GL_QUADS);    // 4つ１組の頂点で四角形を描画
                                            // 第一頂点
        glTexCoord2f(1.0f, 0.0f);           // テクスチャ座標 S T 座標
        glVertex3f  (2.0f, -1.0f, 0.0f);    // 頂点座標
                                            // 第二頂点
        glTexCoord2f(1.0f, 1.0f);           // テクスチャ座標 S T 座標
        glVertex3f  (2.0f, 1.0f, 0.0f);     // 頂点座標
                                            // 第三頂点
        glTexCoord2f( 0.0f, 1.0f);          // テクスチャ座標 S T 座標
        glVertex3f  (-1.0f, 1.0f, 0.0f);    // 頂点座標
                                            // 第四頂点
        glTexCoord2f( 0.0f,  0.0f);         // テクスチャ座標 S T 座標
        glVertex3f  (-1.0f, -1.0f, 0.0f);   // 頂点座標
    glEnd();


    glPopMatrix();

    // テクスチャ座標値の R(GLSL:P) とテクスチャとの比較 ON
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

    glEnable(GL_DEPTH_TEST);

}