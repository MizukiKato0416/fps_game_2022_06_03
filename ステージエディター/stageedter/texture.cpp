//=============================================================================
//
// テクスチャ処理 [texture.cpp]
// Author : 羽鳥太一
//
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include "texture.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CTexture::CTexture()
{

}

//=============================================================================
// デフォルトデストラクタ
//=============================================================================
CTexture::~CTexture()
{

}

//=============================================================================
// テクスチャの生成
//=============================================================================
void CTexture::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = CManager::GetRenderer()->GetDevice();	//デバイスを取得する
	FILE *pFile;
	char aFile[64];

	pFile = fopen("data/Txtdata/texturedata.txt", "r");

	if (pFile != NULL)
	{
		while (true)
		{
			fscanf(pFile, "%s", &aFile[0]);
			if (strcmp(&aFile[0], "TEX_NUM") == 0) // TEX_NUMの文字列を見つけたら
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%d", &m_nTextureMax);
				m_aFilePas = new char*[m_nTextureMax];
				m_pTexture = new LPDIRECT3DTEXTURE9[m_nTextureMax];
				for (int nCnt = 0; nCnt < m_nTextureMax; nCnt++)
				{
					m_aFilePas[nCnt] = new char[PASCHAR_MAX];
				}
			}
			else if (strcmp(&aFile[0], "TEXTURE_PAS") == 0) // TEXTURE_PASの文字列を見つけたら
			{
				for (int nCnt = 0; nCnt < m_nTextureMax; nCnt++)
				{
					fscanf(pFile, "%s", m_aFilePas[nCnt]);
				}
			}
			else if (strcmp(&aFile[0], "END") == 0) //ENDの文字列を見つけたら
			{
				break;
			}
		}
	}
	else
	{
		printf("テクスチャデータが読み取れませんでした。");
	}

	fclose(pFile);

	for (int nCnt = 0; nCnt < m_nTextureMax; nCnt++)
	{
		D3DXCreateTextureFromFile(	pDevice,
									m_aFilePas[nCnt],	// テクスチャパス
									&m_pTexture[nCnt]);
	}
}	

//=============================================================================
// 終了
//=============================================================================
void CTexture::Uniinit(void)
{
	for (int nCntTexture = 0; nCntTexture < m_nTextureMax; nCntTexture++)
	{
		//テクスチャの破棄
		if (m_pTexture[nCntTexture] != NULL)
		{
			m_pTexture[nCntTexture]->Release();
			m_pTexture[nCntTexture] = NULL;
		}
	}
	delete[] m_aFilePas;
}