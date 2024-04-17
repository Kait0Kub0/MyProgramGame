#pragma once

#include "Base.h"

class FadeInOut : public Base
{
private:
    // テクスチャ番号
    int texNo;

    // カラー
    vec4 color;


public:

    // コンストラクタ
    FadeInOut();

    // セッター
    void SetColor(vec4 c4);
    void SetShader(GLint sn);

    // 初期化
    void Init(int mNo, int tNo = -1);
    void Init();

    // 表示処理
    void Disp();

};