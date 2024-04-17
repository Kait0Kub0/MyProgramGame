#pragma once

/********************************************************************************/
/*        music.h                                                               */
/*          ＭＣＩ及びコンソールＡＰＩ拡張ヘッダ                                */
/********************************************************************************/


#pragma     comment( lib , "winmm.lib" )        //ライブラリ追加の命令

//----------------------------------------------------------------------------//
#include <iostream>
#include <string>

#include    < windows.h >
#include    < mmsystem.h >
#include    < stdio.h >
#include    < stdlib.h >
#include    < string.h >
#include    < conio.h >

#include    < atlstr.h >

// 音楽処理
extern  void    OpenMusicMci( CString soundfile, CString alias );
extern  void    OpenWavMci  ( CString soundfile, CString alias );
extern  void    PlayMci     ( CString alias );
extern  void    StopMci     ( CString alias );
extern  void    LoopMci     ( CString alias );
extern  void    CloseMci    ( CString alias );
extern  void    VolumeMci   ( CString alias, int volume );



