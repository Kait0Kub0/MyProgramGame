#pragma once

#include "../../system.h"

// マルチテクスチャ使用数
#define MULTI_TEXTURE_MAX 4


// シェーダー接続因子(linkFactor)
#define FogMode0        "FogMode0"          // フォグ リニア補完
#define FogMode1        "FogMode1"          // フォグ Exp
#define FogMode2        "FogMode2"          // フォグ Exp2
#define LightParallel   "LightParallel"     // 平行光源
#define LightPoint      "LightPoint"        // 点光源

#define FogVspFile   "GLSL/FogVsp.txt"
#define FogFsp0File  "GLSL/FogFsp0.txt"
#define FogFsp1File  "GLSL/FogFsp1.txt"


class ShaderData
{
public:
    string name;    // 管理ネーム
    int listNo;     // 管理番号
    GLint sNo;      // シェーダー番号

    // マルチテクスチャ
    // uniform sampler2D uMultiTexture アクセス番号
    GLint   multiTexture[MULTI_TEXTURE_MAX];

    // 頂点カラー
    // uniform vec4 uColor4 アクセス番号
    GLint   color4;


    // フォグカラー
    //uniform vec4        uFogColor; // フォグカラー
    GLint    fogColor;

    // フォグ密度
    //uniform float       uFogDensity; // Fog密度
    GLint    fogDensity;

    //フォグ計算
    //GL_EXP	f = exp(-(density * z))
    //GL_EXP2	f = exp(-(density * z) * (density * z))
    //GL_LINEAR	f = (end - z) / (end - start)

    GLint    fogMode;   // 0:GL_EXP 1:GL_EXP2  2:GL_LINEAR


    // シャドウ管理番号
    // shadowTexture0 にアクセスするためのアクセス番号
    // uniform sampler2DShadow uShadowTexture0
    GLint shadowNo;


    // コンストラクタ
    ShaderData()
    {
        name = "";
        listNo = -1;
        sNo = -1;

        for (int i = 0; i < MULTI_TEXTURE_MAX; i++)
        {
            multiTexture[i] = -1;
        }

        color4 = -1;

        fogColor = -1;
        fogDensity = -1;
        fogMode = -1;
        shadowNo = -1;
    }

};


class Shader
{
private:
    int listCount = 0;  // リストカウンター
    std::vector<ShaderData> linkedList;

    // セレクトシェーダー番号
    ShaderData* sData;

public:


    // list管理番号からシェーダー番号取得
    GLint GetShaderNo(int listNo);
    GLint GetShaderNo(string nama);

    
    int GetListNo(string nama);
    string GetName(int listNo);

    // シェーダー登録
    int Load(string vsName, string fsName, string nama = "");
    int Load(string vsName, string gsName, string fsName, string nama = "");

    //シェーダー登録
    GLint Compile(string filename, GLint shaderName);

    // ファイル読み込み
    string ReadShaderFile(string filename);
    // コメント削除
    inline void RemoveComments(string& str);


    // シェーダー有効（list管理番号）
    void ShaderOn(int listNo);
    // シェーダー無効（現在のシェーダー）
    void ShaderOff();

    // シェーダーパラメータセット
    void SetShaderParam( int listNo );

    // テクスチャセット
    void SetTexture(int tno, GLint mNo);
    // カラー設定
    void SetColor(float r, float g, float b, float a);
    void SetColor(vec4 color);

    // フォグカラー
    void SetFogColor(vec4 color);
    void SetFogColor(float r, float g, float b, float a);
    void SetFogDensity(float density);

    // シャドウ設定
    void SetShadowTexture(GLint texNo = GL_TEXTURE7);

    // シェーダーパラメーター設定
    void SetInt  (const char* name, int   par);
    void SetFloat(const char* name, float par);
    void SetVec2 (const char* name, vec2  par);
    void SetVec3 (const char* name, vec3  par);
    void SetVec4 (const char* name, vec4  par);


    // ライト設定(ベクトル逆転処理)
    void SetLightPos(vec4 pos, GLint lightNo = GL_LIGHT0);


    // データの削除処理
    bool Delete(int listNo);


    // 検索処理
    // 管理番号検索
    int Search(int listNo);
    // 名前検索
    int Search(string name);

};


extern Shader* shaderObj;
