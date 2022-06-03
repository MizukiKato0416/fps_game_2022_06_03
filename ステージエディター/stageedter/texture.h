//=============================================================================
//
// テクスチャ処理 [texture.h]
// Author : 羽鳥太一
//
//=============================================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTexture
{
public:
	CTexture();	// デフォルトコンストラクタ
	~CTexture();// デフォルトデストラクタ
	void Init(void);	// テクスチャの生成
	LPDIRECT3DTEXTURE9 GetTexture(int type) { return m_pTexture[type]; }	// テクスチャの割り当て(テクスチャの種類) { return 選んだテクスチャを返す }
	int GetTexNum(void) { return m_nTextureMax; }
	char *GetPasName(int nCntPas) { return m_aFilePas[nCntPas]; }
	void Uniinit(void);	// 終了
private:
	LPDIRECT3DTEXTURE9	*m_pTexture;	//テクスチャ
	int m_nTextureMax;	// 読み込むテクスチャの最大数
	char **m_aFilePas;	// ファイルのパス
};

#endif