#pragma once

#include "Music.h"

class MusicSys
{
private:
    CString fileName;   // 音データファイル
    CString alias;      // 呼び出し名
    bool loopFlag;      // ループ再生フラグ

    int count;
    int countMax;

public:

    // コンストラクタ
    MusicSys();

    // デストラクタ
    ~MusicSys();

    // 音源設定
    void SetMusic(CString fileName, CString alias, int countMax = 1);

    // 音源設定：ボリューム無し：高速
    void SetMusicWav(CString fileName, CString alias, int countMax = 1);
    
    
    // 音の再生
    void Play(int no = -1);

    // 音の停止
    void Stop(int no = -1);

    // 全音の停止
    void StopAll();

    // ボリューム
    void Volume(int volume, int no = -1);

    // 全ボリューム
    void VolumeAll(int volume);

    // 音データの破棄
    void Close();

    //ループ再生
    void Loop();

};
