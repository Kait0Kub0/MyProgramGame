#include "system.h"
#include "Base.h"

#define ImagePath "dat/"

class ImageSys {
private:
    int imageCount = 0;
    std::vector<GLint>  imageListNo;
    std::vector<string> imageListName;

    int currentNo = -1;
    int backNo = -1;

public:
    // デフォルトコンストラクタを付ける
    ImageSys() { }

    // 画像データの追加
    int SetImage(string imageFileName);
    void SetImageList(string* imageFileName, int count);

    // 画像番号取得
    int GetImageNo(string imageFileName);

    // 画像番号削除
    int DelImage(int no);
    int DelImage(string imagFileName);

};


// 実体へのアクセス
extern ImageSys* imageSysObj;
