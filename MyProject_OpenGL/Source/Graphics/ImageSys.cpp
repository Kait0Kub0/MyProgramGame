#include "../system.h"

//#include "ImageSys.h"


// 実体
ImageSys* imageSysObj = new ImageSys();

// 画像データの追加
int ImageSys::SetImage(string imageFileName)
{
    // 画像読み込み
    string fileName = ImagePath + imageFileName;
    int imageNo = TextureLoad(fileName.c_str());
    // エラーチェック
    if (imageNo < 1)
    {
        OutputDebugStringW(L"画像読み込みエラー");
        return -1;
    }

    // データ登録
    imageCount++;
    int count = imageCount - 1;

    // 画像番号登録
    imageListNo.resize(imageCount);
    imageListNo[count] = imageNo;

    // 画像名登録
    imageListName.resize(imageCount);
    imageListName[count] = imageFileName;

    return imageNo;
}


void ImageSys::SetImageList(string* imageFileName, int count)
{
    // 複数データの登録
    for (int i = 0; i < count; i++)
    {
        SetImage(imageFileName[i]);
    }
}


// 画像番号取得
int ImageSys::GetImageNo(string imagFileName)
{
    //auto item = std::find(imageListName.begin(), imageListName.end(), imagFileName);
    for (int i = 0; i < imageListName.size(); i++)
    {
        // 同じ番号のチェック
        if (imageListName[i]._Equal(imagFileName))
        {
            return imageListNo[i];
        }
    }

    return 0;
}


// 画像番号削除
int ImageSys::DelImage(int no)
{
    // エラーチェック
    if (no < 0 || no >imageCount - 1)
    {
        // 無効な番号
        return false;
    }

    // 登録番号取得
    GLuint gno = imageListNo[no];
    // テクスチャの削除
    glDeleteTextures(1, &gno);

    // データの削除
    erase(imageListNo, imageListNo[no]);
    erase(imageListName, imageListName[no]);

    imageCount--;

    return true;
}


int ImageSys::DelImage(string imagFileName)
{
    // エラーチェック
    int no = GetImageNo(imagFileName);
    if (no <= 0)
    {
        // 無効な番号
        return false;
    }

    // 登録番号取得
    GLuint gno = imageListNo[no];
    // テクスチャの削除
    glDeleteTextures(1, &gno);

    // データの削除
    erase(imageListNo, imageListNo[no]);
    erase(imageListName, imageListName[no]);

    imageCount--;

    return true;
}
