//----------------------------------------------------------------------
//  Copyright (C) 2014 by Human
//  OpenGL プログラム
//----------------------------------------------------------------------
#include "../system.h"


//----------------------------------------------------------------------
//  テクスチャ作成
//----------------------------------------------------------------------
TEX_DAT * PNG_makeImage(const char *filename )
{
	int     rc = FALSE;

	pngRawInfo  pngInfo;      // PNG    読み込みヘッダ
	TEX_DAT *texDat = NULL;

	rc = pngLoadRaw(filename, &pngInfo);
	if (rc == TRUE)
	{
		// 画像データ管理用構造体の作成
		texDat = (TEX_DAT *)malloc(sizeof(TEX_DAT));
		if (texDat == NULL)  // 領域確保失敗
		{
			return  NULL;
		}
		memset((char*)texDat, 0x00, sizeof(TEX_DAT)); // 領域初期化


		// データ領域のコピー
		int imgSize = pngInfo.Width * pngInfo.Height * 4;
		// アルファ付きデータに変換
		texDat->data = new unsigned char[imgSize];

		switch (pngInfo.Components)
		{
		case 1: // PALETTE RGB
			// パレットデータからの展開処理
			break;
		case 2: // PALETTE RGBA
			break;
		case 3: // RGB
		{
			int rgbSize = pngInfo.Width * pngInfo.Height * 3;
			rgb2rgba(texDat->data, pngInfo.Data, rgbSize);
		}
			break;
		case 4: // RGBA
			memcpy(texDat->data, pngInfo.Data, imgSize);
			break;
		}


        // 上下入れ替え
        topBottom(texDat->data, pngInfo.Width, pngInfo.Height);

		texDat->sizeX = pngInfo.Width;
		texDat->sizeY = pngInfo.Height;

	}
	
	return  texDat;
}


