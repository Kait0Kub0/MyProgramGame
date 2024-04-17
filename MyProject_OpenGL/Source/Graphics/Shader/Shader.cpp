#include "Shader.h"


// シェーダー管理システム実体
Shader* shaderObj = new Shader;



// list管理番号からシェーダー番号取得
GLint Shader::GetShaderNo(int listNo)
{
    int searchNo = Search(listNo);
    if (searchNo == -1)
    {
        return -1;
    }

    return linkedList[searchNo].sNo;
}
GLint Shader::GetShaderNo(string nama)
{
    int searchNo = Search(nama);
    if (searchNo == -1)
    {
        return -1;
    }

    return linkedList[searchNo].sNo;
}


// 管理番号取得
int Shader::GetListNo(string nama)
{
    int lno = Search(nama);
    return lno;
}
// 管理名取得
string Shader::GetName(int listNo)
{
    int searchNo = Search(listNo);
    if (searchNo == -1)
    {
        return "";
    }

    return linkedList[searchNo].name;
}



// シェーダー登録
int Shader::Load(string vsName, string fsName, string name)
{
    // 名前指定あり
    if (name != "")
    {
        // 名前検索
        int searchNo = Search(name);
        if (searchNo != -1)
        {
            // 管理番号返却
            return searchNo;
        }
    }


    // シェーダー用変数
    int vertecShaderNo;
    int flagmentShaderNo;
    GLint linkShaderNo;


    // シェーダーコンパイル
    vertecShaderNo = Compile(vsName, GL_VERTEX_SHADER);
    flagmentShaderNo = Compile(fsName, GL_FRAGMENT_SHADER);


    // リンクシェーダーのセット
    linkShaderNo = glCreateProgram();
    glAttachShader(linkShaderNo, vertecShaderNo);
    glAttachShader(linkShaderNo, flagmentShaderNo);
    // リンク処理
    glLinkProgram(linkShaderNo);

    // エラー情報の取得
    char msg[1000];
    int eSize = 0;
    glGetShaderInfoLog(linkShaderNo, 1000, &eSize, msg);
    OutputDebugStringA("====== linkShader ======\n");

    // エラーチェック
    if (eSize != 0)
    {
        OutputDebugStringA(msg);
        // シェーダーコンパイルエラー
        return -1;
    }


    // リストの管理番号カウントアップ
    listCount++;
    // オーバーフローチェック
    if (listCount < 0)
    {
        listCount = 0;
    }

    // 名前指定なし
    if (name == "")
    {
        name = "_ShaderNo" + listCount;
    }

    // シェーダのリスト登録
    size_t listSize = linkedList.size() + 1;
    linkedList.resize(listSize);                        // リストサイズ変更
    linkedList[listSize - 1].sNo = linkShaderNo;     // シェーダー番号登録
    linkedList[listSize - 1].listNo = listCount;        // 管理番号セット
    linkedList[listSize - 1].name = name;             // 管理名セット

    // シェーダーデフォルト変数チェック
    SetShaderParam(listCount);

    return listCount;                                   // シェーダーリスト番号返却
}


// シェーダー登録(ジオメトリーシェーダーバージョン)
int Shader::Load(string vsName, string gsName, string fsName, string name)
{
    // 名前指定あり
    if (name != "")
    {
        // 名前検索
        int lno = Search(name);
        if (lno != -1)
        {
            // 管理番号返却
            return lno;
        }
    }


    // シェーダー用変数
    int vertecShaderNo;
    int geometryShaderNo;
    int flagmentShaderNo;
    GLint linkShaderNo;

    // シェーダーコンパイル
    vertecShaderNo = Compile(vsName, GL_VERTEX_SHADER);
    geometryShaderNo = Compile(gsName, GL_GEOMETRY_SHADER);
    flagmentShaderNo = Compile(fsName, GL_FRAGMENT_SHADER);

    // リンクシェーダーのセット
    linkShaderNo = glCreateProgram();
    glAttachShader(linkShaderNo, vertecShaderNo);
    glAttachShader(linkShaderNo, geometryShaderNo);
    glAttachShader(linkShaderNo, flagmentShaderNo);
    // リンク処理
    glLinkProgram(linkShaderNo);

    // エラー情報の取得
    char msg[1000];
    int eSize = 0;
    glGetShaderInfoLog(linkShaderNo, 1000, &eSize, msg);
    OutputDebugStringA("====== linkShader ======\n");

    // エラーチェック
    if (eSize != 0)
    {
        OutputDebugStringA(msg);
        // シェーダーコンパイルエラー
        return -1;
    }


    // リストの管理番号カウントアップ
    listCount++;
    // オーバーフローチェック
    if (listCount < 0)
    {
        listCount = 0;
    }

    // 名前指定なし
    if (name == "")
    {
        name = "_ShaderNo" + listCount;
    }

    // シェーダのリスト登録
    size_t listSize = linkedList.size() + 1;
    linkedList.resize(listSize);                        // リストサイズ変更
    linkedList[listSize - 1].sNo = linkShaderNo;        // シェーダー番号登録
    linkedList[listSize - 1].listNo = listCount;        // 管理番号セット
    linkedList[listSize - 1].name = name;               // 管理名セット

    // シェーダーデフォルト変数チェック
    SetShaderParam(listCount);

    return listCount;                                   // シェーダーリスト番号返却
}


// シェーダーコンパイル
GLint Shader::Compile(string filename, GLint shaderName)
{
    string shaderString;
    int    shaderNo;

    // シェーダーファイル読み込み
    shaderString = ReadShaderFile(filename);

    // =========================================
    // 因子接続（fog light その他） 追加すること
    // =========================================

    // 強制的な GLchar* へ変換
    const GLchar* str = reinterpret_cast<GLchar*>(shaderString.data());


    // バーテックスシェーダー選択
    shaderNo = glCreateShader(shaderName);

    // シェーダーのコンーパイラーにプログラム設定
    glShaderSource(shaderNo, 1, &str, NULL);

    // コンパイル
    glCompileShader(shaderNo);

    // エラー情報の取得
    char msg[1000];
    int eSize = 0;

    glGetShaderInfoLog(shaderNo, 1000, &eSize, msg);
    filename = "---- " + filename + "----\n";
    OutputDebugStringA(filename.c_str());
    OutputDebugStringA(msg);

    return shaderNo;
}

// ファイル読み込み
string Shader::ReadShaderFile(string filename)
{
    string str;
    ifstream ifstr(filename);
    if (ifstr.fail())
    {
        cout << "Shader::ReadShaderFile：ファイルがありません。" << endl;
        return str;
    }
    // ファイル読み込み
    istreambuf_iterator<char> first(ifstr);
    istreambuf_iterator<char> last;
    str = string(first, last);

    // コメント排除
    RemoveComments(str);

    return str;
}



//コメント削除処理
// BOMコード削除
inline void Shader::RemoveComments(string& str)
{
    string::size_type posStart = 0, posEnd, posCR, posLF;

    // BOMコードチェック
    char* cStr = (char*)str.c_str();    // char型(１バイトデータ)
    if (*(cStr + 0) == (char)0xEF
        && *(cStr + 1) == (char)0xBB
        && *(cStr + 2) == (char)0xBF)
    {
        // BOMコード排除
        *(cStr + 0) = 0x20; // 0x20 : Space
        *(cStr + 1) = 0x20;
        *(cStr + 2) = 0x20;
    }


    // コメントの排除
    while (true)
    {
        // 文字列検索( // )
        posStart = str.find("//", posStart);
        // データが見つからない
        if (posStart == string::npos) return;

        // 改行文字の検索（※改行は２種類ある \r と \n）
        posCR = str.find("\r", posStart + 2); // + 2 は、// の文字数
        posLF = str.find("\n", posStart + 2); // + 2 は、// の文字数
        posEnd = posCR < posLF ? posCR - posStart : posLF - posStart;

        // コメントを削除
        str.erase(posStart, posEnd);
    }
};






// シェーダーON
void Shader::ShaderOn(int listNo)
{
    // 登録データ検索
    int searchNo = Search(listNo);
    if (searchNo == -1)
    {
        ShaderOff();
        return;
    }

    // シェーダーデータ
    sData = &linkedList[searchNo];

    glUseProgram(sData->sNo);


}


// シェーダーOFF
void Shader::ShaderOff()
{
    glUseProgram(NULL);
    for (int i = 0; i < 7; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);   // マルチテクスチャ番号選択
        glBindTexture(GL_TEXTURE_2D, 0);    // テクスチャ OFF
    }

}


// シェーダーデフォルト変数登録
void Shader::SetShaderParam(int listNo)
{
    // 登録データ検索
    int searchNo = Search(listNo);
    if (searchNo == -1)
    {
        ShaderOff();
        return;
    }

    // シェーダーデータ
    sData = &linkedList[searchNo];

    // シェーダー番号取得
    int sno = sData->sNo;

    // テクスチャセット（アクセス番号取得）
    sData->multiTexture[0] = glGetUniformLocation(sno, "uMultiTexture0");
    sData->multiTexture[1] = glGetUniformLocation(sno, "uMultiTexture1");
    sData->multiTexture[2] = glGetUniformLocation(sno, "uMultiTexture2");
    sData->multiTexture[3] = glGetUniformLocation(sno, "uMultiTexture3");

    // マルチテクスチャ処理
    for (int i = 0; i < 4; i++)
    {
        SetTexture(i, i);
    }


    // カラーセット（アクセス番号取得）
    sData->color4 = glGetUniformLocation(sno, "uColor4");
    if (sData->color4 != -1)
    {
        SetColor(1, 1, 1, 1);
    }


    // フォグ設定
    // フォグカラー
    sData->fogColor = glGetUniformLocation(sno, "uFogColor");
    // フォグ密度
    sData->fogDensity = glGetUniformLocation(sno, "uFogDensity");


    // シャドウセット（アクセス番号取得）
    sData->shadowNo =
        glGetUniformLocation(sno, "uShadowTexture0");

    if (sData->shadowNo != -1)
    {
        // マルチテクスチャ７番セット(0～7)
        SetShadowTexture(7);
    }

}

// テクスチャのセット
void Shader::SetTexture(int tno, GLint mNo)
{
    if (mNo > 7 || mNo < 0)
    {
        return;
    }

    if (sData->multiTexture[tno] == -1)
    {
        // マルチテクスチャOFF
        glActiveTexture(GL_TEXTURE0 + mNo); // マルチテクスチャ番号選択
        glBindTexture(GL_TEXTURE_2D, 0);    // テクスチャ OFF
        return;
    }

    // シェーダーテクスチャセット
    glUniform1i(sData->multiTexture[tno], mNo);

}

void Shader::SetColor(vec4 color)
{
    SetColor(color.r, color.g, color.b, color.a);
}
void Shader::SetColor(float r, float g, float b, float a)
{
    if (sData->color4 == -1)
    {
        return;
    }

    glUniform4f(sData->color4, r, g, b, a);

}


// フォグ関連
void Shader::SetFogColor(vec4 color)
{
    SetFogColor(color.r, color.g, color.b, color.a);
}
void Shader::SetFogColor(float r, float g, float b, float a)
{
    if (sData->fogColor == -1)
    {
        return;
    }

    glUniform4f(sData->fogColor, r, g, b, a);
}

// フォグ密度
void Shader::SetFogDensity(float density)
{
    if (sData->fogDensity == -1)
    {
        return;
    }

    glUniform1f(sData->fogDensity, density);
}



// シャドウマルチテクスチャ番号（デフォルト:7）
void Shader::SetShadowTexture(GLint texNo)
{
    if (sData->shadowNo == -1)
    {
        return;
    }

    glUniform1i(sData->shadowNo, texNo);
}




// シェーダーへパラメータを送る処理
// int データを送る
void Shader::SetInt(const char* name, int par)
{
    // 選択中のシェーダー
    if (sData == nullptr)
    {
        return;
    }

    // シェーダー番号
    GLint sno = sData->sNo;

    int uniformNo = glGetUniformLocation(sno, name);

    if (uniformNo == -1)
    {
        //OutputDebugStringW(L"SetInt:uniform エラー\n");
        return;
    }

    glUniform1i(uniformNo, par);

}

// float データを送る
void Shader::SetFloat(const char* name, float par)
{
    // 選択中のシェーダー
    if (sData == nullptr)
    {
        return;
    }

    // シェーダー番号
    GLint sno = sData->sNo;

    int uniformNo = glGetUniformLocation(sno, name);

    if (uniformNo == -1)
    {
        //OutputDebugStringW(L"SetFloat:uniform エラー\n");
        return;
    }

    glUniform1f(uniformNo, par);

}

// vec2 データを送る
void Shader::SetVec2(const char* name, vec2 par)
{
    // 選択中のシェーダー
    if (sData == nullptr)
    {
        return;
    }

    // シェーダー番号
    GLint sno = sData->sNo;

    int uniformNo = glGetUniformLocation(sno, name);

    if (uniformNo == -1)
    {
        //OutputDebugStringW(L"SetFloat:uniform エラー\n");
        return;
    }

    glUniform2f(uniformNo, par.x, par.y);

}


// vec3 データを送る
void Shader::SetVec3(const char* name, vec3 par)
{
    // 選択中のシェーダー
    if (sData == nullptr)
    {
        return;
    }

    // シェーダー番号
    GLint sno = sData->sNo;

    int uniformNo = glGetUniformLocation(sno, name);

    if (uniformNo == -1)
    {
        //OutputDebugStringW(L"SetFloat:uniform エラー\n");
        return;
    }

    glUniform3f(uniformNo, par.x, par.y, par.z);

}


// vec4 データを送る
void Shader::SetVec4(const char* name, vec4 par)
{
    // 選択中のシェーダー
    if (sData == nullptr)
    {
        return;
    }

    // シェーダー番号
    GLint sno = sData->sNo;

    int uniformNo = glGetUniformLocation(sno, name);

    if (uniformNo == -1)
    {
        //OutputDebugStringW(L"SetFloat:uniform エラー\n");
        return;
    }

    glUniform4f(uniformNo, par.x, par.y, par.z, par.w);

}



// ライト設定(ベクトル逆転処理)
void Shader::SetLightPos(vec4 pos, GLint lighNo)
{
    // ライトポジション
    vec4 lightPos;


    // 光源の種類チェック
    if (pos.w == 0.0f)
    {
        // 並行光源
        // ベクトルを逆転する
        // ライトベクトルと法線は逆向きなので
        // 光源ベクトルを逆向きにする
        vec3 work = pos * -1.0f;
        // 光源ベクトルを長さ１にする
        work = glm::normalize(work);
        lightPos
            = vec4(work.x, work.y, work.z, 0);
    }
    else
    {
        // 点光源
        lightPos = pos;
    }


    // ライトのセット
    glLightfv(GL_LIGHT0, GL_POSITION, (float*)&lightPos);

}





// データの削除処理
bool Shader::Delete(int listNo)
{
    // 管理番号検索
    int searchNo = Search(listNo);

    // 該当データなし
    if (searchNo == -1)
    {
        return false;
    }

    // シェーダー番号
    GLint sno = linkedList[searchNo].sNo;

    // シェーダープログラム削除
    glDeleteProgram(sno);

    // リスト番号削除
    linkedList.erase(linkedList.begin() + searchNo);

    return true;
}



// 管理番号検索
int Shader::Search(int listNo)
{
    // 検索処理
    for (int i = 0; i < linkedList.size(); i++)
    {
        if (listNo == linkedList[i].listNo)
        {
            return i;
        }
    }

    return -1;
}
// 名前検索
int Shader::Search(string name)
{
    // 検索処理
    for (int i = 0; i < linkedList.size(); i++)
    {
        if (name == linkedList[i].name)
        {
            return i;
        }
    }

    return -1;
}
