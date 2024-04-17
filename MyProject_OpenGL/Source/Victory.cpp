
#include "Victory.h"


void Victory::SetDispNo(int no, vec3 pos)
{
    this->dispNo = no;
    this->pos = pos;
}


void Victory::Init(int tWin, int tLose, int tDraw)
{
    texNo[0] = tWin;
    texNo[1] = tLose;
    texNo[2] = tDraw;
}


void Victory::Init()
{
    // 表示フラグ
    dispNo = 0;
    // 表示位置
    pos = vec3(0, 0, 0);
}


void Victory::Disp()
{
    if (dispNo == 0)
    {
        return;
    }


    glPushMatrix();

    // スプライト表示
    SpritePos(texNo[ dispNo - 1 ], pos);

    glPopMatrix();

}
