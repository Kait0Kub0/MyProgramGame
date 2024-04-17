#include "Hitchk.h"

// 円と点の当たり判定
bool  Hitchk::CircleHitPoint(Base* h1, Base* h2)
{
    bool ret = false;
    ret = CircleHitPoint(h1->pos, h1->radius, h2->pos);
    return ret;

}
bool  Hitchk::CircleHitPoint(vec3 cPos, float radius, vec3 pPos)
{
    // 球の中心から点までの長さを求める
    float length = glm::length(cPos - pPos);

    // 半径と比べる
    if (radius < length)
    {
        // ハズレ判定
        return false;
    }

    // 当たり判定
    return true;
}

// 円と円の当たり判定
bool Hitchk::CircleHitCircle(Base* h1, Base* h2)
{
    //glm::vec3 pos;  // glmの記述  
    // 中心の長さ求める
    float vecLength = glm::length((h1->pos - h2->pos));

    // 半径を足した長さを求める
    float radius = h1->radius + h2->radius;

    // 距離を比べる
    if (vecLength < radius)
    {
        // 当たり
        return true;
    }

    // 外れ
    return false;
}


// 円と円の当たり判定と押し戻し
bool Hitchk::PushCircleHitCircle(Base* h1, Base* h2)
{
    // 円と円の当たり判定
    // 中心の長さ求める
    float vecLength = glm::length((h1->pos - h2->pos));

    // 完全一致している場合
    if (vecLength == 0.0f)
    {
        // 押し戻せません
        return true;
    }

    // 半径を足した長さを求める
    float radius = h1->radius + h2->radius;

    // 距離を比べる
    if (vecLength >= radius)
    {
        // 外れ
        return false;
    }

    // 押し戻し処理

    // 中心点の距離と半径の差分を求める
    float diff = radius - vecLength;

    // 差分を2で割る
    //diff = diff / 2.0f;

    // 半径の比率で移動量を決める
    diff = diff / radius;
    float diff1 = diff * h1->radius;
    float diff2 = diff * h2->radius;

    // h1 の移動方向計算
    vec3 movH1 = h1->pos - h2->pos;
    movH1 = glm::normalize(movH1); // 単位ベクトル

    // h2 の移動方向計算
    vec3 movH2 = h2->pos - h1->pos;
    movH2 = glm::normalize(movH2); // 単位ベクトル

    // h1 座標を修正
    h1->pos += movH1 * diff1;

    // h2 座標を修正
    h2->pos += movH2 * diff2;

    // 当たり
    return true;
}


// 線と面の当たり判定
bool Hitchk::LineHitPoly(vec3* pp, vec3 lineP1, vec3 lineP2)
{
    bool ret;
    vec3 E;     // 単位ベクトル
    vec3 H;     // 交点H
    vec3 N;     // 法線
    vec3 pv[2]; // ラインベクトル
    float T;    // Hまでの距離
    float LS;   // 線の長さ

    // ラインベクトル計算・ベクトルの正規化
    E = lineP2 - lineP1;
    // ラインベクトルの長さ計算
    LS = glm::length(E);
    // 単位ベクトルに変換
    E = glm::normalize(E);

    // 法線計算
    pv[0] = pp[0] - pp[1];          // ポリゴンの辺１
    pv[1] = pp[1] - pp[2];          // ポリゴンの辺２
    N = glm::cross(pv[0], pv[1]);   // 外積計算
    N = glm::normalize(N);          // 長さ１のベクトルに変換

    // 平行時の処理
    //if ((N.x * E.x + N.y * E.y + N.z * E.z) == 0.0f)
    if ( glm::dot(N,E) == 0.0f )
    {
        return false;
    }

    // 開始点から無限平面までの距離 t
    T = -(N.x * (lineP1.x - pp->x)
        + N.y * (lineP1.y - pp->y)
        + N.z * (lineP1.z - pp->z))
        / (N.x * E.x + N.y * E.y + N.z * E.z);

    // 無限平面との交点
    H.x = E.x * T + lineP1.x;
    H.y = E.y * T + lineP1.y;
    H.z = E.z * T + lineP1.z;

    // 直線内の交点チェック
    if (0.0f > T || T > LS)
    {
        return false;
    }

    // ポリゴン内外判定
    ret = TriangleHitPoint(H, pp);
    return ret;
}


// 内外判定
bool Hitchk::TriangleHitPoint(vec3 h, vec3* p)
{
    vec3    ret[3];
    vec3    p1, p2;

    for (int i = 0; i < 3; i++)
    {
        // %3 は、+1 した時に0に戻す処理
        p1 = p[(i + 1) % 3] - p[i];
        p2 = h - p[i];

        // 外積計算
        ret[i] = glm::cross(p2, p1);

        // 直交するベクトルの方向チェック（内積を使用）
        if (glm::dot(ret[0], ret[i]) < 0.0f)
        {
            return false;   // ポリゴン外
        }
    }

    return true;    // ポリゴン内
}
