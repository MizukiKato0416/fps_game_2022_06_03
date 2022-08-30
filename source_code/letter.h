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
	static CLetter*Create(D3DXVECTOR3 porigon_pos, D3DXVECTOR3 polygon_size, int font_size, int font_weight, wchar_t text);	// 生成処理
	static vector<wstring> Conbrt(string buf);
	void CreateTexture(void);	// テクスチャ生成
	void SetFontType(string type) { m_font_type = type; }	// セッター
	void SetFontSize(int nsize) { m_font_size = nsize; }	// セッター
	void SetFontWeight(int nweight) { m_font_weight = nweight; }	// セッター
	void SetText(wchar_t text) { m_text = text; CreateTexture(); }	// セッター
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }	// セッター
	void SetSize(D3DXVECTOR3 size) { m_size = size; }	// セッター
private:
	LPDIRECT3DTEXTURE9		m_texture = NULL;		//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_vtx_buff = NULL;		//頂点バッファへのポインタ
	D3DXVECTOR3				m_pos;	// ポリゴンの原点
	D3DXVECTOR3				m_size;	// サイズ
	string					m_font_type;	// テクスチャタイプ
	wchar_t					m_text;	// テキスト
	int						m_font_size;	// 高さ(高さだけで幅も決まる)
	int						m_font_weight;	// 太さ
};

#endif