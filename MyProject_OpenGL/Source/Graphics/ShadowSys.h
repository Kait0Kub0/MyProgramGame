#pragma once

#include "../system.h"

#define FBUFFER_WIDTH 2048
#define FBUFFER_HEIGHT 2048

#define GL_COMPARE_R_TO_TEXTURE 0x884E
#define GL_DEPTH_TEXTURE_MODE   0x884B

class ShadowSys
{
private:
public:
    // シャドウマップ（シャドウ用テクスチャ）
    GLuint  dynamicTextureID;

    // フレムバッファーID
    GLuint  frameBuffersID;

    // ビューポート
    ivec4   viewport;
    // プロジェクションマトリックス
    dmat4   projection;
    // モデルビューマトリックス
    dmat4   modelview;

    // ライトポジション
    vec3    lightPos;

    // シェーダー番号
    GLint   shaderNo;

    // シャドウマップ描画フラグ
    bool    mapDispFlag = false;

    // コンストラクタ
    ShadowSys();

    // セッター
    void SetLightPos(vec3 pos);

    // シャドウマップシェーダのセット
    void SetShadowShader(GLint shaderNo);

    // 初期化
    void Init();

    // シャドウスタート
    void ShadowStart();

    // シャドウエンド
    void ShadowEnd();

    // テクスチャマトリックス
    void SetTextureMatrix();


    // シャドウマップ表示
    void ShadowDisp();


};


// プロトタイプ宣言
extern ShadowSys* shadowSysObj;
