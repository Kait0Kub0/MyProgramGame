#pragma once

#include "Music.h"

class MusicSys
{
private:
    CString fileName;   // ���f�[�^�t�@�C��
    CString alias;      // �Ăяo����
    bool loopFlag;      // ���[�v�Đ��t���O

    int count;
    int countMax;

public:

    // �R���X�g���N�^
    MusicSys();

    // �f�X�g���N�^
    ~MusicSys();

    // �����ݒ�
    void SetMusic(CString fileName, CString alias, int countMax = 1);

    // �����ݒ�F�{�����[�������F����
    void SetMusicWav(CString fileName, CString alias, int countMax = 1);
    
    
    // ���̍Đ�
    void Play(int no = -1);

    // ���̒�~
    void Stop(int no = -1);

    // �S���̒�~
    void StopAll();

    // �{�����[��
    void Volume(int volume, int no = -1);

    // �S�{�����[��
    void VolumeAll(int volume);

    // ���f�[�^�̔j��
    void Close();

    //���[�v�Đ�
    void Loop();

};
