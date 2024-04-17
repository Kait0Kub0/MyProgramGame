#pragma once

#define  _CRT_SECURE_NO_WARNINGS

#pragma     comment( lib , "winmm.lib" )        //ライブラリ追加の命令

//----------------------------------------------------------------------------//
// C++
#include <iostream>
#include <string>
#include <vector>

#include    < windows.h >
#include    < mmsystem.h >
#include    < stdio.h >
#include    < stdlib.h >
#include    < string.h >
#include    < conio.h >

//#include    < atlstr.h >



#include "OpenAL/include/al.h"
#include "OpenAL/include/alc.h"
#include "OpenAL/include/alut.h"

#pragma comment(lib, "alut.lib")
#pragma comment(lib, "OpenAL32.lib")

// Ogg 
#include "Ogg/include/ogg/ogg.h"
#include "Ogg/include/vorbis/vorbisenc.h"
#include "Ogg/include/vorbis/vorbisfile.h"
#pragma comment(lib,"libogg.lib")
#pragma comment(lib,"libvorbisfile_static.lib")
#pragma comment(lib,"libvorbis_static.lib") 
//#pragma comment(lib,"vorbis.lib")
#define BUFFER_SIZE     32768       // 32 KB buffers


using namespace std;



class MusicSys
{
private:
    string fileName;       // 音データファイル
    ALuint  bufferNo;       // バッファの管理番号
    ALuint* sourceNo;       // 再生音の管理番号


    bool loopFlag;          // ループ再生フラグ

    int count;              // 次の番号
    int countMax;           // 管理ソースの数

public:

    // コンストラクタ
    MusicSys();

    // デストラクタ
    ~MusicSys();

    // 音源設定
    void SetMusic(string fileName, int countMax = 1);

    // 音源設定：ogg ファイル
    void SetMusicOgg(string fileName, int countMax = 1);

    // 音源設定：wav ファイル
    void SetMusicWav(string fileName, int countMax = 1);
    
    
    // 音の再生
    void Play(int no = -1);

    // 音の停止
    void Stop(int no = -1);

    // 全音の停止
    void StopAll();

    // ボリューム
    void Volume(float volume, int no = -1);

    // 全ボリューム
    void VolumeAll(float volume);

    // 音データの破棄
    void Close();

    //ループ再生
    void Loop(bool flag, int no = -1);


    // Ogg 読み込み
    bool LoadOgg(const char* name, vector<char>& buffer, ALenum& format, ALsizei& freq);

};
