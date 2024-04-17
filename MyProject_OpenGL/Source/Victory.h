#pragma once
#include "system.h"

class Victory
{
private:

    // テクスチャ
    int texNo[3];

    // 表示ポジション
    vec3 pos;

    // 表示フラグ
    int dispNo = 0;


public:

    void SetDispNo(int no, vec3 pos);

    void Init(int tWin, int tLose, int tDoraw);

    void Init();

    void Disp();

};