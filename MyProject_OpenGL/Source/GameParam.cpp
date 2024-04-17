#include "GameParam.h"

// 実体
GameParam* gameParam = new GameParam();


// コンストラクタ
GameParam::GameParam()
{
    // プレイヤーキャラクタ
    charModeP1 = CharMode::Fighter01;

    // ポイント初期化
    p1Point = 0;
}
