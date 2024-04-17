#include "../system.h"
#include "../Base.h"

// 実体を作成
ModelSys* modelSysObj = new ModelSys();


// コンストラクタ
ModelSys::ModelSys()
{

}

// モデルデータの追加
int ModelSys::SetModel(ModelData* model, string name)
{
    int ret = -1;

    // エラーチェック
    if (model == nullptr)
    {
        OutputDebugString(L"ModelSys::SetModelエラー\n");
        return -1;
    }

    // 名前設定
    string mName = "_ModelNo";
    if (name == "")
    {
        // 名前をつけない場合
        mName = mName + to_string(dataCount);
    }
    else
    {
        // ユーザー指定の名前
        mName = name;
    }


    // 同名登録チェック
    int listNo = Search(name);
    if (listNo == -1)
    {
        // リストへ登録
        ret = (int)dataCount;   // データ番号

        // データ準備
        ModelList* ml = new ModelList();
        ml->name = mName;
        ml->dataNo = (int)dataCount;
        ml->modelData = model;

        // 次の番号
        dataCount++;

        // サイズ変更
        int modelSize = (int)(modelList.size() + 1);
        modelList.resize(modelSize);

        // リスト登録
        int listNo = modelSize - 1;
        modelList[listNo] = ml;

    }
    else
    {
        OutputDebugString(L"ModelSys::SetModel同名登録エラー\n");
    }

    return ret;
}


// モデルデータの読み込み
int ModelSys::LoadModel(string modelName, string name)
{
    // パス名＋名前の作成
    string pathName = ModelPath + modelName;

    // モデルデータ読み込み
    ModelData* modelData = ModelDataLoad(pathName.c_str());
    // エラーチェック
    if (modelData == nullptr)
    {
        OutputDebugString(L"ModelSys::SetModelエラー\n");
        return -1;
    }

    // リスト登録
    int dataNo = SetModel(modelData, name);

    return dataNo;
}


// モデルデータの取得
ModelData* ModelSys::GetModelData(string name)
{
    // データ検索
    int listNo = Search(name);
    if (listNo == -1)
    {
        return nullptr;
    }

    return modelList[listNo]->modelData;
}
ModelData* ModelSys::GetModelData(int dataNo)
{
    // 登録サイズチェック
    int listNo = Search(dataNo);
    if (listNo == -1)
    {
        return nullptr;
    }

    return modelList[listNo]->modelData;
}

// モデルデータリストの番号取得
int ModelSys::GetListNo(string name)
{
    // データ検索
    int listNo = Search(name);
    return listNo;
}
int ModelSys::GetListNo(int dataNo)
{
    // データ検索
    int listNo = Search(dataNo);
    return listNo;
}


// モデルデータの削除
void ModelSys::DeleteList(int listNo)
{
    // 削除処理
    ModelData* modelData = modelList[listNo]->modelData;
    // モデルデータ内の領域開放
    ObjVertexFree2(modelData->vertexObj);
    modelData->vertexObj = nullptr;
    modelList.erase(modelList.begin() + listNo);

}
void ModelSys::Delete(int dataNo)
{
    int listNo = Search(dataNo);
    if (listNo != -1)
    {
        DeleteList(listNo);
    }
}
void ModelSys::Delete(string name)
{
    int listNo = Search(name);
    if (listNo != -1)
    {
        DeleteList(listNo);
    }
}
void ModelSys::Delete(ModelData* model)
{
    for (int i = 0; i < modelList.size(); i++)
    {
        if (modelList[i]->modelData == model)
        {
            Delete(i);
        }
    }
}

// リスト検索
int ModelSys::Search(string name)
{
    // 全件検索
    for (int i = 0; i < modelList.size(); i++)
    {
        if (modelList[i]->name == name)
        {
            return i;
        }
    }

    return -1;
}
int ModelSys::Search(int dataNo)
{
    // 全件検索
    for (int i = 0; i < modelList.size(); i++)
    {
        if (modelList[i]->dataNo == dataNo)
        {
            return i;
        }
    }

    return -1;
}
