#include "system.h"
#include "FadeInOut.h"

// グローバル変数
FadeInOut* fadeInOutObj = new FadeInOut();


// コンストラクタ
FadeInOut::FadeInOut()
{
    // テクスチャ番号
    this->texNo = -1;

    // カラーデータ
    this->color = vec4(1, 1, 1, 1);

    // テクスチャ座標 offset
    this->offsetNo0 = 0;
    this->offsetNo1 = 0;

    // スピード
    this->speed = 0;

    // モード設定
    this->mode = FadeMode::Off;

    // スレッド用初期化
    this->ThreadInit(this);
}


// セッター
void FadeInOut::SetColor(vec4 color)
{
    this->color = color;
}
void FadeInOut::SetShader(int sNo)
{
    this->shaderNo = sNo;
}
void FadeInOut::SetTextureNo(int tNo)
{
    this->texNo = tNo;
}
void FadeInOut::SetSpeed(float speed)
{
    this->speed = speed;
}

// モード取得
FadeMode FadeInOut::GetMode()
{
    return mode;
}


// 初期化
void FadeInOut::Init(int sNo, int tNo)
{
    this->shaderNo = sNo;
    this->texNo = tNo;
}


// 初期化
void FadeInOut::Init(FadeMode mode)
{
    // 表示フラグ
    this->dispFlag = true;

    // モード設定
    this->mode = mode;


    // モード分岐
    switch (mode)
    {
    case FadeMode::FadeIn:
        InitIn();
        break;
    case FadeMode::FadeInTex:
        InitInTex();
        break;
    case FadeMode::FadeOut:
        InitOut();
        break;
    case FadeMode::FadeOutTex:
        InitOutTex();
        break;
    }
}


// アルファ値フェードイン
void FadeInOut::InitIn()
{
    // アルファ値 1 → 0
    color.a = 1;

    // UV座標
    offsetNo0 = 0.0f;
    offsetNo1 = 1.0f;
}


// テクスチャ座標フェードイン
void FadeInOut::InitInTex()
{
    // アルファ値 1
    color.a = 1;

    offsetNo0 =  -9.0f;
    offsetNo1 =  10.0f;
}


// アルファ値フェードアウト
void FadeInOut::InitOut()
{
    // アルファ値 0 → 1
    color.a = 0;

    // UV座標によるフェードアウト
    offsetNo0 = 0.0f;
    offsetNo1 = 1.0f;
}


// テクスチャ座標フェードアウト
void FadeInOut::InitOutTex()
{
    // アルファ値 1
    color.a = 1;

    offsetNo0 = 0.5f;
    offsetNo1 = 0.5f;
}



// フェードIN・OUT
bool FadeInOut::FadeMove()
{
    bool ret = false;

    // モード分岐
    switch (mode)
    {
    case FadeMode::Off:
        break;
    case FadeMode::FadeIn:
        ret = FadeIn();
        break;
    case FadeMode::FadeInTex:
        ret = FadeInTex();
        break;
    case FadeMode::FadeOut:
        ret = FadeOut();
        break;
    case FadeMode::FadeOutTex:
        ret = FadeOutTex();
        break;
    }

    // 結果の返却
    return ret;
}


// スレッド処理用 FadeMove
unsigned FadeInOut::ThreadFadeMove(void* p)
{
    // 実体のアドレス取得
    FadeInOut* fio = (FadeInOut*)p;

    while (true)
    {
        // 強制終了チェック
        if (fio->exitFlag == true)
        {
            break;
        }

        // フレーム待ち
        Sleep(fio->fcount);

        // フェード処理
        fio->FadeMove();

         
        // フェード処理の終了チェック
        if (fio->mode == FadeMode::Off)
        {
            break;
        }
    }

    // 終了処理
    fio->exitFlag = true;
    fio->endFlag = true;

    // 終了パラメータ
    _endthreadex(false);

    return 0;
}


bool FadeInOut::FadeIn()
{
    color.a -= speed;
    // 終了チェック
    if (color.a < 0.0f)
    {
        mode = FadeMode::Off;
        return false;
    }

    return false;
}


bool FadeInOut::FadeInTex()
{
    // テクスチャ座標のフェードイン
    offsetNo0 += speed;
    offsetNo1 -= speed;

    // 終了処理
    if (offsetNo0 > 0.0f)
    {
        mode = FadeMode::Off;
        dispFlag = false;
        return false;
    }

    return true;
}


bool FadeInOut::FadeOut()
{
    // アルファ値のフェードアウト
    color.a += speed;
    // 終了チェック
    if (color.a > 1.0f)
    {
        mode = FadeMode::Off;
        return false;
    }

    return true;
}


bool FadeInOut::FadeOutTex()
{
    // exp計算
    //float sp = exp(speed);
    float sp = speed;

    // テクスチャ座標のフェードアウト
    offsetNo0 -= sp;
    offsetNo1 += sp;

    // 終了処理
    if (offsetNo0 < -100.0f)
    {
        mode = FadeMode::Off;
        return false;
    }

    return true;
}


// 表示処理
void FadeInOut::Disp()
{
    // 表示チェック
    if (dispFlag == false)
    {
        return;
    }

    // シェーダーセット
    shaderObj->ShaderOn(shaderNo);
    shaderObj->SetTexture(0, texNo);

    // テクスチャON・OFF
    if (texNo != -1)
    {
        glEnable(GL_TEXTURE);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texNo);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);     // S 座標
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);     // T 座標

    }
    else
    {
        glDisable(GL_TEXTURE);
    }

    // マトリックス保存
    glPushMatrix(); // モデルビューマトリックスの保存

    // 画面サイズの取得
    float sw = clientW / 2.0f;
    float sh = clientH / 2.0f;

    // プロジェクションマトリックスに変更
    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); // プロジェクションマトリックス保存
    glLoadIdentity();
    // 平行投影
    // 左,右,下,上, ニアクリップ, ファークリップ
    glOrtho(-sw,sw, -sh,sh, -1.0f, 100.0f );

    // モデルビューマトリックスに変更
    glMatrixMode(GL_MODELVIEW);
    // マトリックス初期化
    glLoadIdentity();
    glTranslatef(0, 0, 0);

    // UV座標設定

    // カラー設定
    glColor4fv((float*)&color);

    // ポリゴン表示
    glBegin(GL_QUADS);
        glTexCoord2f(offsetNo1, offsetNo0);
        glVertex3f( sw, -sh, 0.0f);

        glTexCoord2f(offsetNo1, offsetNo1);
        glVertex3f( sw,  sh, 0.0f);

        glTexCoord2f(offsetNo0, offsetNo1);
        glVertex3f(-sw,  sh, 0.0f);

        glTexCoord2f(offsetNo0, offsetNo0);
        glVertex3f(-sw, -sh, 0.0f);
    glEnd();





    // プロジェクションマトリックスに変更
    glMatrixMode(GL_PROJECTION);
    // マトリックス取得
    glPopMatrix();

    // モデルビューマトリックスに変更
    glMatrixMode(GL_MODELVIEW);
    // マトリックス取得
    glPopMatrix();



}
