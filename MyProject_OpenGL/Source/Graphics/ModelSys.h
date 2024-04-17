#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define ModelPath "dat/"

// 前方宣言
class ModelData;

// モデルリストクラス
class ModelList
{
private:
public:
    int         dataNo;     // 管理番号
    string      name;       // 管理名
    ModelData*  modelData;  // モデルデータ

    // コンストラクタ
    ModelList()
    {
        dataNo = -1;
        name = "";
        modelData = nullptr;
    }
    ModelList(int dn, string na, ModelData* md)
    {
        dataNo = dn;
        name = na;
        modelData = md;
    }
};


// モデルデータ管理クラス
class ModelSys
{
private:
    // データの数
    size_t dataCount = 0;
    vector<ModelList*> modelList;

    // リスト削除
    void DeleteList(int listNo);

public:
    // コンストラクタ
    ModelSys();

    // モデルデータの追加
    int SetModel(ModelData* model, string name = "");

    // モデルデータの読み込み
    int LoadModel(string modelName, string name = "");

    // モデルデータの取得
    ModelData* GetModelData(string name);
    ModelData* GetModelData(int dataNo);

    // モデルデータリストの番号取得
    int GetListNo(string name);
    int GetListNo(int dataNo);

    // モデルデータの削除
    void Delete(string name);
    void Delete(int dataNo);
    void Delete(ModelData* model);

    // リスト検索
    int Search(string name);
    int Search(int dataNo);

};


// プロトタイプ宣言（カタログ）
extern ModelSys* modelSysObj;
