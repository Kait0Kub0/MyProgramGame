#pragma once

#include "system.h"

class Base
{
private:

public:
    bool    dispFlag;       // 表示フラグ（表示・生存チェック用）


    vec3    pos;            // ポジション
    vec3    rot;            // 回転量
    vec3    mov;            // 移動ベクトル
    vec3    scale;          // 拡大縮小
    float   radius;         // 半径(キャラクタの半径を設定)
    float   alpha;          // 半透明
    float   speed;          // スピード

    vec3    centerPos;      // 中心座標

    int     shaderNo = -1;  // シェーダー番号


    int     life;           // ライフポイント



    // ジャンプフラグ    ジャンプ中:ON(true)
    bool    jumpFlag;
    // ジャンプベクトル
    vec3    jumpMove;


    // 視点座標
    vec3    lookPos;



    // 自分の回転マトリックス
    quat myQuaternion;
#define DELAY_MAX 20
    vector<quat>delayQuaternion;


    // コンストラクタ
    Base()
    {
        dispFlag = false;
        pos = vec3(0, 0, 0);
        rot = vec3(0, 0, 0);
        mov = vec3(0, 0, 0);
        scale = vec3(1, 1, 1);
        radius = 0;
        alpha = 1;
        speed = 0;

        shaderNo = -1;

        life = 0;

        jumpFlag = false;
        jumpMove = vec3(0, 0, 0);

        lookPos = vec3(0, 0, 0);

        myQuaternion = quat(0,0,0,0);

        delayQuaternion.clear();
    }


    // 共有関数を用意する
    virtual void CommonHit()
    {
        cout << "Base関数 CommonHit " << endl;
    }


};
