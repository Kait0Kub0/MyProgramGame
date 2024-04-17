#pragma once

#include "system.h"

// キャラクター種類
enum class CharMode
{
    Fighter01,
    Fighter02,
    MAX
};


class GameParam
{
private:
public:

    // プレイヤーキャラクタ
    CharMode charModeP1;

    // 勝敗ポイント
    int p1Point;

    GameParam();
};


// 実体
extern GameParam* gameParam;
