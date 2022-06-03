//=============================================================================
//
// レンダリング処理 [renderer.h]
// Author : 羽鳥太一
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CRenderer
{
public:
	CRenderer();	// デフォルトコンストラクタ
	~CRenderer();	// デフォルトデストラクタ
	HRESULT Init(HWND hWnd, bool bWindow);	// スクリーンの初期化
	void Uninit(void);	// スクリーンの終了
	void Update(void);	// スクリーンの更新
	void Draw(void);	// スクリーンの描画
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }	// デバイスの取得
	POINT GetMousePos(void) { return m_Mouse; }
private:
	HWND m_Hdnd;
	POINT m_Mouse;
	void DrawTxt(void);	// FPSカウンタの描画
	LPDIRECT3D9				m_pD3D = NULL;		// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9		m_pD3DDevice = NULL;// Deviceオブジェクト(描画に必要)
	LPD3DXFONT				m_pFont = NULL;		// フォントへのポインタ
};
#endif