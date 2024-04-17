
#include "MusicSys.h"


// コンストラクタ
MusicSys::MusicSys()
{
    fileName = "";      // 音データファイル
    alias    = "";      // 呼び出し名
    loopFlag = false;   // ループ再生フラグ

    count    = 0;
    countMax = 0;
}


// デストラクタ
MusicSys::~MusicSys()
{
    for (int i = 0; i < countMax; i++)
    {
        //std::to_string(countMax) 文字列変換
        // .c_str() char * へ変換
        CString aliasName = alias + std::to_string(countMax).c_str();

        CloseMci(aliasName);
    }

}


// 音源設定
void MusicSys::SetMusic(CString fileName, CString alias, int countMax)
{

    // カウンター
    count = 0;

    // 数セット
    this->countMax = countMax;

    // ファイル名
    this->fileName = fileName;

    // 呼び出し名
    this->alias = alias;

    // 指定された数作成
    for (int i = 0; i < countMax; i++)
    {
        CString callAlias = alias + std::to_string(i).c_str();
        OpenMusicMci(fileName, callAlias);
    }

}

// 音源設定：ボリューム無し：高速
void MusicSys::SetMusicWav(CString fileName, CString alias, int countMax)
{
    // カウンター
    count = 0;

    // 数セット
    this->countMax = countMax;

    // ファイル名
    this->fileName = fileName;

    // 呼び出し名
    this->alias = alias;

    // 指定された数作成
    for (int i = 0; i < countMax; i++)
    {
        CString callAlias = alias + std::to_string(i).c_str();
        OpenWavMci(fileName, callAlias);
    }

}


// 音の再生
void MusicSys::Play(int no)
{
    int musicNo;

    // 番号指定チェック
    if (no != -1)
    {
        // ユーザー指定の番号再生
        musicNo = no;
    }
    else
    {
        // 内部管理番号で再生
        musicNo = count;
        count = (count + 1) % countMax;
    }

    CString callAlias = alias + std::to_string(musicNo).c_str();
    PlayMci(callAlias);

}


// 音の停止
void MusicSys::Stop(int no)
{
        int musicNo;

        // 番号指定チェック
        if (no != -1)
        {
            // ユーザー指定の番号再生
            musicNo = no;
        }
        else
        {
            // 値指定なしの Play は、count を前に進めるので count-1
            // ひとつ前のNo停止：マイナス対策
            musicNo = ((count-1) % countMax + countMax) % countMax;
        }

        CString callAlias = alias + std::to_string(musicNo).c_str();
        StopMci(callAlias);
}


// 全音の停止
void MusicSys::StopAll()
{
    for (int i = 0; i < countMax; i++)
    {
        CString callAlias = alias + std::to_string(i).c_str();
        StopMci(callAlias);
    }
}


// ボリューム
void MusicSys::Volume(int volume, int no)
{
    int musicNo;

    // 番号指定チェック
    if (no != -1)
    {
        // ユーザー指定の番号再生
        musicNo = no;
    }
    else
    {
        // 値指定なしの Play は、count を前に進めるので count-1
        // ひとつ前のNo停止：マイナス対策
        musicNo = ((count - 1) % countMax + countMax) % countMax;
    }

    CString callAlias = alias + std::to_string(musicNo).c_str();
    VolumeMci(callAlias, volume);
}


// 全ボリューム
void MusicSys::VolumeAll(int volume)
{
    for (int i = 0; i < countMax; i++)
    {
        CString callAlias = alias + std::to_string(i).c_str();
        VolumeMci(callAlias, volume);
    }
}


// 音データの破棄
void MusicSys::Close()
{
    for (int i = 0; i < countMax; i++)
    {
        CString callAlias = alias + std::to_string(i).c_str();
        CloseMci(callAlias);
    }
}


//ループ再生
void MusicSys::Loop()
{
    for (int i = 0; i < countMax; i++)
    {
        CString callAlias = alias + std::to_string(i).c_str();
        LoopMci(callAlias);
    }
}


