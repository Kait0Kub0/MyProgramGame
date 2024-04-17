
#include "MusicSys.h"


// �R���X�g���N�^
MusicSys::MusicSys()
{
    fileName = "";      // ���f�[�^�t�@�C��
    alias    = "";      // �Ăяo����
    loopFlag = false;   // ���[�v�Đ��t���O

    count    = 0;
    countMax = 0;
}


// �f�X�g���N�^
MusicSys::~MusicSys()
{
    for (int i = 0; i < countMax; i++)
    {
        //std::to_string(countMax) ������ϊ�
        // .c_str() char * �֕ϊ�
        CString aliasName = alias + std::to_string(countMax).c_str();

        CloseMci(aliasName);
    }

}


// �����ݒ�
void MusicSys::SetMusic(CString fileName, CString alias, int countMax)
{

    // �J�E���^�[
    count = 0;

    // ���Z�b�g
    this->countMax = countMax;

    // �t�@�C����
    this->fileName = fileName;

    // �Ăяo����
    this->alias = alias;

    // �w�肳�ꂽ���쐬
    for (int i = 0; i < countMax; i++)
    {
        CString callAlias = alias + std::to_string(i).c_str();
        OpenMusicMci(fileName, callAlias);
    }

}

// �����ݒ�F�{�����[�������F����
void MusicSys::SetMusicWav(CString fileName, CString alias, int countMax)
{
    // �J�E���^�[
    count = 0;

    // ���Z�b�g
    this->countMax = countMax;

    // �t�@�C����
    this->fileName = fileName;

    // �Ăяo����
    this->alias = alias;

    // �w�肳�ꂽ���쐬
    for (int i = 0; i < countMax; i++)
    {
        CString callAlias = alias + std::to_string(i).c_str();
        OpenWavMci(fileName, callAlias);
    }

}


// ���̍Đ�
void MusicSys::Play(int no)
{
    int musicNo;

    // �ԍ��w��`�F�b�N
    if (no != -1)
    {
        // ���[�U�[�w��̔ԍ��Đ�
        musicNo = no;
    }
    else
    {
        // �����Ǘ��ԍ��ōĐ�
        musicNo = count;
        count = (count + 1) % countMax;
    }

    CString callAlias = alias + std::to_string(musicNo).c_str();
    PlayMci(callAlias);

}


// ���̒�~
void MusicSys::Stop(int no)
{
        int musicNo;

        // �ԍ��w��`�F�b�N
        if (no != -1)
        {
            // ���[�U�[�w��̔ԍ��Đ�
            musicNo = no;
        }
        else
        {
            // �l�w��Ȃ��� Play �́Acount ��O�ɐi�߂�̂� count-1
            // �ЂƂO��No��~�F�}�C�i�X�΍�
            musicNo = ((count-1) % countMax + countMax) % countMax;
        }

        CString callAlias = alias + std::to_string(musicNo).c_str();
        StopMci(callAlias);
}


// �S���̒�~
void MusicSys::StopAll()
{
    for (int i = 0; i < countMax; i++)
    {
        CString callAlias = alias + std::to_string(i).c_str();
        StopMci(callAlias);
    }
}


// �{�����[��
void MusicSys::Volume(int volume, int no)
{
    int musicNo;

    // �ԍ��w��`�F�b�N
    if (no != -1)
    {
        // ���[�U�[�w��̔ԍ��Đ�
        musicNo = no;
    }
    else
    {
        // �l�w��Ȃ��� Play �́Acount ��O�ɐi�߂�̂� count-1
        // �ЂƂO��No��~�F�}�C�i�X�΍�
        musicNo = ((count - 1) % countMax + countMax) % countMax;
    }

    CString callAlias = alias + std::to_string(musicNo).c_str();
    VolumeMci(callAlias, volume);
}


// �S�{�����[��
void MusicSys::VolumeAll(int volume)
{
    for (int i = 0; i < countMax; i++)
    {
        CString callAlias = alias + std::to_string(i).c_str();
        VolumeMci(callAlias, volume);
    }
}


// ���f�[�^�̔j��
void MusicSys::Close()
{
    for (int i = 0; i < countMax; i++)
    {
        CString callAlias = alias + std::to_string(i).c_str();
        CloseMci(callAlias);
    }
}


//���[�v�Đ�
void MusicSys::Loop()
{
    for (int i = 0; i < countMax; i++)
    {
        CString callAlias = alias + std::to_string(i).c_str();
        LoopMci(callAlias);
    }
}


