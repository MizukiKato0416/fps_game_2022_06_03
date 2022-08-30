//=============================================================================
//
// 2Dポリゴン処理 [scene2D.h]
// Author : 羽鳥太一
//
//=============================================================================
#ifndef _SCENELETTER_H_
#define _SCENELETTER_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include <tchar.h>
#include "main.h"
#include "object.h"
#include "object2D.h"
#include "texture.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FVF_CUSTOM (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1) // 座標変換済み頂点
#define VERTEX_NUM (4)	// 頂点の数

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CLetter : public CObject
{
public:
	CLetter(CObject::PRIORITY Layer = CObject::PRIORITY::UI);	// デフォルトコンストラクタ
	~CLetter();	// デフォルトデストラクタ
	virtual HRESULT Init(void);	// ポリゴンの初期化
	virtual void Uninit(void);	// ポリゴンの終了
	virtual void Update(void);	// ポリゴンの更新
	virtual void Draw(void);	// ポリゴンの描画
	static void Load(void);	// フォントのロード
	void SetFontType(string type) { m_FontType = type; }	// セッター
	void SetFontSize(int nsize) { m_nFontSize = nsize; }	// セッター
	void SetFontWeight(int nweight) { m_nFontWeight = nweight; }	// セッター
	void SetText(wchar_t text) { m_Text = text; }	// セッター
	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }	// セッター
	void SetSize(D3DXVECTOR3 size) { m_Size = size; }	// セッター
private:
	LPDIRECT3DTEXTURE9		m_pTexture = NULL;		//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;		//頂点バッファへのポインタ
	D3DXVECTOR3				m_Pos;	// ポリゴンの原点
	D3DXVECTOR3				m_Size;	// サイズ
	string					m_FontType;	// テクスチャタイプ
	wchar_t					m_Text;	// テキスト
	int						m_nFontSize;	// 高さ(高さだけで幅も決まる)
	int						m_nFontWeight;	// 太さ
};

#endif