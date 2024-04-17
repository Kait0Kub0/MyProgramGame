//----------------------------------------------------------------------
//  Copyright (C) 2021 by Human
//----------------------------------------------------------------------

#include "../system.h"

#define TITLE_BAR_NAME	"3D Game(OpenGL)"

// 画像ファイル名
char  TitleFilename0[] = "dat/Title.png";	// 画像ファイル名
char  EndingFilename0[] = "dat/Ending.png";	// 画像ファイル名


// グローバル変数
GLFWwindow* GlfwWindow;

GLuint      GLTexNo[10];					// テクスチャ管理番号

TEX_DAT    *TexImage[10];					// テクスチャデータアドレス


GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };        // 環境光・周囲光
GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };        // 拡散光
vec4 LightPosition = vec4( 1.0f, 1.0f, 0.0f, 0.0f );       // 平行光源ベクトル
//GLfloat LightPosition[] = { 5.0f, 0.0f, 0.0f, 1.0f };       // 点光源座標


//  色属性（材質）
GLfloat surface_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };    // 
GLfloat surface_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };    // 


// ウィンドウハンドル
HWND hwind;

// Windows ループ処理
int     windowsLoop = TRUE;


// ウィンドウサイズ（クライアント領域）
int clientW, clientH;


//----------------------------------------------------------------------
// エラー表示
//----------------------------------------------------------------------
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}


//----------------------------------------------------------------------
// キー入力処理
//----------------------------------------------------------------------
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		windowsLoop = SYSEXIT;
        glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

//----------------------------------------------------------------------
// キー入力処理
//----------------------------------------------------------------------
int getKeyIn(int key)
{
	return glfwGetKey(GlfwWindow, key);
}

//----------------------------------------------------------------------
// 終了ボタン処理
//----------------------------------------------------------------------
static void window_close_callback(GLFWwindow* window)
{
    if (MessageBox(NULL, L"終了しますか？", L"終了確認", MB_YESNO) == IDYES)
    {
        glfwSetWindowShouldClose(window, GL_FALSE);
        windowsLoop = SYSEXIT;
    }
}


//----------------------------------------------------------------------
// 画面サイズ変更処理
//----------------------------------------------------------------------
static void reshape_callback(GLFWwindow* window, int width, int height)
{
	float   fw, fh;

	// キャスト変換
	fw = (float)width;
	fh = (float)height;

    // クライアント領域サイズ設定
    clientW = width;
    clientH = height;

	//ビューポート設定
    glViewport(0, 0, width, height);


	//マトリックス変更（プロジェクションマトリックス）
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


	// パース設定
	gluPerspective(
		45.0,				//画角
		fw / fh,			//アスペクト比
		0.1,				//Near クリップ
		2000.0				//Far  クリップ
		);

	//マトリックス変更（モデルビューマトリックス）
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


//----------------------------------------------------------------------
// 画像読み込み処理
//----------------------------------------------------------------------
int TextureLoad(const char *fileName, bool linear, bool mipmap)
{
    GLuint texNo;
    TEX_DAT *texImg;

	// テクスチャ貼り付け ON
	glEnable(GL_TEXTURE_2D);

    // テクスチャ管理番号取得
    // 引数 ： 番号取得数， テクスチャ番号格納変数のアドレス
    glGenTextures(1, &texNo);

	// 画像読込み
	// ミップマップOnOFF
	if (mipmap == false)
	{
		// 直線補完
		if (linear == false)
		{
			// 補完処理 OFF
			texImg = TextureGL(texNo, fileName, GL_REPEAT, GL_NEAREST);
		}
		else
		{
			// 補完処理 ON
			texImg = TextureGL(texNo, fileName, GL_REPEAT, GL_LINEAR);
		}

	}
	else
	{
		// 異方性フィルタリング（ミップマップが必要）
		// GL_NEAREST_MIPMAP_NEAREST
		// GL_NEAREST_MIPMAP_LINEAR
		// GL_LINEAR_MIPMAP_NEAREST
		// GL_LINEAR_MIPMAP_LINEAR
		// 直線補完
		if (linear == false)
		{
			// 補完処理 OFF
			texImg = TextureGL(texNo, fileName, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST);
		}
		else
		{
			// 補完処理 ON
			texImg = TextureGL(texNo, fileName, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR);
		}
	}

    // エラーチェック
    if (texImg == nullptr)
    {
        // データの読み込み失敗
        texNo = -1;
    }
    
    // 画像データ領域開放
    TexDatFree(&texImg);

    return texNo;
}


//----------------------------------------------------------------------
// OpenGL 初期化 処理
//----------------------------------------------------------------------
int sysInit()
{

	// glfw 初期化
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
	{
		return -1;
	}

	// glfw 初期設定
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);

	// glfw ウィンドウ作成
	GlfwWindow = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE_BAR_NAME, NULL, NULL);
	if (!GlfwWindow)
	{
		// 終了処理
		glfwTerminate();
		return -1;
	}

	// ウィンドウハンドルの取得
	hwind = glfwGetWin32Window(GlfwWindow);

	// コンテキスト設定
	glfwMakeContextCurrent(GlfwWindow);

	// gl3w OpenGL 拡張用関数
	if (gl3wInit()) {
		printf("failed to initialize OpenGL\n");
		return -1;
	}
	if (!gl3wIsSupported(3, 2)) {
		printf("OpenGL 3.2 not supported\n");
		return -1;
	}


	// キー入力コールバック設定
	glfwSetKeyCallback(GlfwWindow, key_callback);

    // 終了ボタンコールバック設定
    glfwSetWindowCloseCallback(GlfwWindow, window_close_callback);

	// 画面サイズ変更処理
	glfwSetWindowSizeCallback(GlfwWindow, reshape_callback);
	reshape_callback(GlfwWindow, SCREEN_WIDTH, SCREEN_HEIGHT);

    // VSync 設定
    glfwSwapInterval(FALSE);

    // ＦＰＳ処理の初期化
    TIMECAPS    tc;

    timeGetDevCaps(&tc, sizeof(TIMECAPS));    // 実行可能な分解能取得
    timeBeginPeriod(tc.wPeriodMin);           // timeGetTime() の精度を上げる


	// 画像読み込み
    GLTexNo[0] = TextureLoad(TitleFilename0);
    GLTexNo[1] = TextureLoad(EndingFilename0);


	glEnable(GL_CULL_FACE);           // 面方向クリッピング ON
	glEnable(GL_DEPTH_TEST);          // ディプスバッファ   ON


	// スムースシェーディング使用
	// GL_SMOOTH ： グラデーションポリゴン ※デフォルト
	// GL_FLAT   ： フラットポリゴン
	glShadeModel(GL_SMOOTH);


	// ライトの設定
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);     // 環境光
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);     // 拡散光
	glLightfv(GL_LIGHT0, GL_POSITION, (float *)&LightPosition);   // 光源の位置
	glEnable(GL_LIGHT0);

	// ライト使用許可
	glEnable(GL_LIGHTING);
	glDisable(GL_LIGHTING);


	// アルファ値の比較書き込み
	glEnable(GL_ALPHA_TEST);
	// アルファ値が 0.3f 以外のデータを書き込む
	glAlphaFunc(GL_GEQUAL, 0.3f);


	// ブレンディング
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);


	// 背景クリア色設定
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
	glClearDepth(1.0f);


	// スプライト初期化
	SpriteInit(sprite, SPRITE_MAX);

	// シェーダーの読み込み
	GLint shaderNo
		= shaderObj->Load("GLSL/SpriteVsp.txt", "GLSL/SpriteFsp.txt");

	// スプライトにシェーダーセット
	SpriteShaderAll(shaderNo);


    // 文字の初期化
    ttfTextSysInit(&hwind);

	return 0;
}


//----------------------------------------------------------------------
// OpenGL 描画処理
//----------------------------------------------------------------------
void sysDraw()
{

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glColor3f(1.0f, 1.0f, 1.0f);

		// 行列の初期化
		glLoadIdentity();

        // 視点の設定
        gluLookAt(
            0.0f, 0.0f, 10.0f,  // 視点 
            0.0f, 0.0f,  0.0f,  // 注視点
            0.0f, 1.0f,  0.0f   // アップベクトル
            );


		SpriteDraw();


		// バッファスワップ
		glfwSwapBuffers(GlfwWindow);
		// イベントのポーリング
		glfwPollEvents();

}



//----------------------------------------------------------------------
// OpenGL 描画処理(Loop)
//----------------------------------------------------------------------
void sysDrawLoop()
{
    // 回転量
    while (windowsLoop)
    {
        // メインループ
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // 行列の初期化
        glLoadIdentity();


        // 視点の設定
        gluLookAt(
            0.0f, 0.0f, 10.0f,  // 視点 
            0.0f, 0.0f, 0.0f,   // 注視点
            0.0f, 1.0f, 0.0f    // アップベクトル
            );


        SpriteDraw();


        // バッファスワップ
        glfwSwapBuffers(GlfwWindow);
        // イベントのポーリング
        glfwPollEvents();

    }

}



//----------------------------------------------------------------------
// OpenGL 終了処理
//----------------------------------------------------------------------
void sysEnd()
{
	glfwDestroyWindow(GlfwWindow);
	glfwTerminate();

}