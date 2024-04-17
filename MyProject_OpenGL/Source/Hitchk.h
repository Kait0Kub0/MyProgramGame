#pragma once

#include "Base.h"


class Hitchk
{
private:
public:
    
    // 円と点の当たり判定
    static bool CircleHitPoint(Base* h1, Base* h2);
    static bool CircleHitPoint(vec3 cPos, float radius, vec3 pPos);

    // 円と円の当たり判定
    static bool CircleHitCircle(Base* h1, Base* h2);

    // 円と円の当たり判定と押し戻し
    static bool PushCircleHitCircle(Base* h1, Base* h2);

    // 線と面の当たり判定
    static bool LineHitPoly(vec3* pp, vec3 lineP1, vec3 lineP2);

    // 内外判定
    static bool TriangleHitPoint(vec3 h,vec3* p);

};
