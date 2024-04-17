/********************************************************************************/
/*        music.cpp                                                             */
/*          ＭＣＩ及びコンソールＡＰＩ拡張ヘッダ                                */
/********************************************************************************/
#include "music.h"

//ユニコード


//----------------------------------------------------------------------------//
void    StopMci( CString alias ) {
    CString buff = "stop " + alias;
    mciSendString( buff, NULL , 0 , NULL ) ;
}

//----------------------------------------------------------------------------//
void    OpenMusicMci(CString soundfile , CString alias ) {
    CString buff;

    // データ使用中のデータがあれば開放
    CloseMci( soundfile ) ;

    // 本来はtype指定してオープンするべきだが、ボリューム調整ができなくなるので
    // 音量調整するときは、全てMPEGVideoで開く
    buff = "open " + soundfile + " alias " + alias + " type MPEGVideo";
    mciSendString ( buff, NULL , 0 , NULL ) ;
}


void    OpenWavMci(CString soundfile, CString alias) {

    CString buff;

    // データ使用中のデータがあれば開放
    CloseMci(soundfile);

    // Waveファイルは、音量調整ができないが高速
    buff = "open " + soundfile + " alias " + alias + " type WaveAudio";
    mciSendString(buff, NULL, 0, NULL);

}

//----------------------------------------------------------------------------//
void    PlayMci( CString alias ) {
    CString buff;

    StopMci(alias);

    buff = "play " + alias + " from 0 notify";
    mciSendString ( buff, NULL , 0 , NULL ) ;
}

//----------------------------------------------------------------------------//
void    LoopMci( CString alias ) {
    CString buff;

    buff = "play " + alias + " repeat from 0 notify";
    mciSendString ( buff, NULL , 0 , NULL ) ;
}

//----------------------------------------------------------------------------//
void    CloseMci( CString alias ) {
    CString buff;

    StopMci(alias);

    buff = "close " + alias;
    mciSendString ( buff, NULL , 0 , NULL ) ;
}


//----------------------------------------------------------------------------//
// volume 0～1000
void    VolumeMci( CString alias, int volume )
{
    CString buff;

    buff = "setaudio " + alias + " volume to " + std::to_string(volume).c_str();

// CStringを使った変換
//    CString volumeString;
//    volumeString.Format(_T("%d"), volume);
//    buff = "setaudio " + alias + " volume to " + volumeString;

	mciSendString( buff, NULL, 0, NULL );
}
