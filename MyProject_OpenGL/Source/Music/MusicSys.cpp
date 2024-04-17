
#include "MusicSys.h"


// コンストラクタ
MusicSys::MusicSys()
{
    // alut初期化
    alutInit(NULL, 0);

    fileName = "";      // 音データファイル
    bufferNo = 0;       // バッファの管理番号
    sourceNo = nullptr; // 再生音の管理番号

    loopFlag = false;   // ループ再生フラグ

    count    = 0;
    countMax = 0;
}


// デストラクタ
MusicSys::~MusicSys()
{
    // 領域と管理番号の開放
    Close();
}


// 音源設定
void MusicSys::SetMusic(string fileName, int countMax)
{
    // カウンター
    count = 0;

    // 数セット
    this->countMax = countMax;

    // ファイル名
    this->fileName = fileName;

    // ogg に変更！！！！
    // 音データ読み込み(wavファイルのみ)
    bufferNo = alutCreateBufferFromFile(fileName.c_str());

    // 再生音の管理番号を指定された数作成
    sourceNo = new ALuint[countMax];
    alGenSources(countMax, sourceNo);

    // 音データと管理番号の組み合わせ
    for (int i = 0; i < countMax; i++)
    {
        alSourcei(sourceNo[i], AL_BUFFER, bufferNo);
    }

}

// 音源設定(ogg用)
void MusicSys::SetMusicOgg(string fileName, int countMax)
{

    // カウンター
    count = 0;

    // 数セット
    this->countMax = countMax;

    // ファイル名
    this->fileName = fileName;

    ALenum formatOgg;                       // The sound data format
    ALsizei freq;                           // The frequency of the sound data
    vector<char> bufferData;                // The sound buffer data from file

    // Load the Ogg file into memory
    LoadOgg(fileName.c_str(), bufferData, formatOgg, freq);

    // バッファ番号取得
    alGenBuffers(1, &bufferNo);
    // Upload sound data to buffer
    alBufferData(bufferNo, formatOgg, &bufferData[0], static_cast<ALsizei>(bufferData.size()), freq);


    // 再生音の管理番号を指定された数作成
    sourceNo = new ALuint[countMax];
    alGenSources(countMax, sourceNo);

    // 音データと管理番号の組み合わせ
    for (int i = 0; i < countMax; i++)
    {
        alSourcei(sourceNo[i], AL_BUFFER, bufferNo);
    }


}


// 音源設定(wav用)
void MusicSys::SetMusicWav(string fileName, int countMax)
{
    // カウンター
    count = 0;

    // 数セット
    this->countMax = countMax;

    // ファイル名
    this->fileName = fileName;

    // 音データ読み込み(wavファイルのみ)
    bufferNo = alutCreateBufferFromFile(fileName.c_str());

    // 再生音の管理番号を指定された数作成
    sourceNo = new ALuint[countMax];
    alGenSources(countMax, sourceNo);

    // 音データと管理番号の組み合わせ
    for (int i = 0; i < countMax; i++)
    {
        alSourcei(sourceNo[i], AL_BUFFER, bufferNo);
    }

}


// 音の再生
void MusicSys::Play(int no)
{
    int musicNo;

    // 番号指定チェック
    if (no < 0 || no >= countMax)
    {
        // 内部管理番号で再生
        musicNo = count;
        count = (count + 1) % countMax;
    }
    else
    {
        // ユーザー指定の番号再生
        musicNo = no;
    }

    // 再生
    //alSourcePlay( *(sourceNo+musicNo) );
    alSourcePlay(sourceNo[musicNo]);

}


// 音の停止
void MusicSys::Stop(int no)
{
    int musicNo;

    // 番号指定チェック
    if (no < 0 || no >= countMax)
    {
        // 値指定なしの Play は、count を前に進めるので count-1
        // ひとつ前のNo停止：マイナス対策
        musicNo = ((count - 1) % countMax + countMax) % countMax;
    }
    else
    {
        // ユーザー指定の番号再生
        musicNo = no;
    }

    // 停止
    //alSourceStop( *(sourceNo+musicNo) );
    alSourceStop(sourceNo[musicNo]);

}


// 全音の停止
void MusicSys::StopAll()
{
    for (int i = 0; i < countMax; i++)
    {
        //alSourceStop( *(sourceNo+i) );
        alSourceStop(sourceNo[i]);
    }
}


// ボリューム
void MusicSys::Volume(float volume, int no)
{
    int musicNo;

    // 番号指定チェック
    if (no < 0 || no >= countMax)
    {
        // 値指定なしの Play は、count を前に進めるので count-1
        // ひとつ前のNo停止：マイナス対策
        musicNo = ((count - 1) % countMax + countMax) % countMax;
    }
    else
    {
        // ユーザー指定の番号再生
        musicNo = no;
    }

    // ボリューム設定
    //alSourcef( *(sourceNo+musicNo), AL_GAIN, volume);
    alSourcef(sourceNo[musicNo], AL_GAIN, volume);

}


// 全ボリューム
void MusicSys::VolumeAll(float volume)
{
    for (int i = 0; i < countMax; i++)
    {
        //alSourcef( *(sourceNo+i), AL_GAIN, volume);
        alSourcef(sourceNo[i], AL_GAIN, volume);
    }
}


// 音データの破棄
void MusicSys::Close()
{
    // ソース削除
    if (sourceNo != nullptr)
    {
        // 再生の停止
        StopAll();

        // 番号の開放
        alDeleteSources(countMax, sourceNo);

        // 領域の開放
        delete[] sourceNo;
        sourceNo = nullptr;
    }

    // バッファの開放
    if (bufferNo != 0)
    {
        alDeleteBuffers(1, &bufferNo);
        bufferNo = 0;
    }

}


//ループ再生
void MusicSys::Loop( bool flag, int no )
{
    // エラーチェック
    if (no < 0 || no >= countMax)
    {
        // 範囲外のときは、全設定
        for (int i = 0; i < countMax; i++)
        {
            // ループ再生
            alSourcei(sourceNo[i], AL_LOOPING, flag);
        }
    }
    else
    {
        // ループ再生
        alSourcei(sourceNo[no], AL_LOOPING, flag);
    }

}


//OGG データ読み込み
// Ogg 読み込み
//      Ogg データは一括のデータ読み込みができない。
//      分割されたデータが複数読み込まれるので vector に追加して一纏めにする。
bool MusicSys::LoadOgg(const char* filename, vector<char>& buffer, ALenum& format, ALsizei& freq)
{
    int endian = 0;                         // 0 for Little-Endian, 1 for Big-Endian
    int bitStream;
    long bytes;
    FILE* fp = NULL;

    // ファイルオープン
    fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        string msg = "MusicSys:Error Open :";
        msg += filename;
        OutputDebugStringA(msg.c_str());
        return false;
    }

    vorbis_info* pInfo;
    OggVorbis_File oggFile;

    // Ogg ファイルオープン
    if (ov_open(fp, &oggFile, NULL, 0) != 0)
    {
        string msg = "MusicSys:Error Ogg Open :";
        msg += filename;
        OutputDebugStringA(msg.c_str());

        // ファイルクローズ
        fclose(fp);

        return false;
    }


    // インフォメーション情報セット
    pInfo = ov_info(&oggFile, -1);

    // ステレオ・モノラルチェック
    if (pInfo->channels == 1)
    {
        format = AL_FORMAT_MONO16;
    }
    else
    {
        format = AL_FORMAT_STEREO16;
    }


    // レート情報セット
    freq = pInfo->rate;


    // データ領域確保
    char* array = new char[BUFFER_SIZE];    // Local fixed size array

    // 分割されたデータの読み込み
    do
    {
        // データの読み込み処理
        bytes = ov_read(&oggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);

        // エラーチェック
        if (bytes < 0)
        {
            // データの読み込みに失敗( bytes = -1 )

            // Oggデーコーダー を閉じる
            ov_clear(&oggFile);

            // エラーメッセージ
            string msg = "MusicSys:Error decoding :";
            msg += filename;
            OutputDebugStringA( msg.c_str() );

            // 領域開放
            delete[] array;
            array = nullptr;

            // ファイルクローズ
            if (fp != NULL)
            {
                fclose(fp);
            }

            return false;
        }

        // vector に追加
        char* startAddress = array;             // 先頭アドレス
        char* endAddress   = array + bytes;     // 終了アドレス
        // startAddress から endAddress までのデータを buffer へ追加
        buffer.insert(buffer.end(), startAddress, endAddress);

    } while (bytes > 0);

    // Oggデーコーダー を閉じる
    ov_clear(&oggFile);


    // ファイルクローズ
    if (fp != NULL)
    {
        fclose(fp);
    }


    // 領域開放
    delete[] array;
    array = nullptr;

    return true;
}
