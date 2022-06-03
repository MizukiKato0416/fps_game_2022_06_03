//=============================================================================
//
// マネージャー管理処理 [namager.h]
// Author : 羽鳥太一
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "light.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CRenderer;
class CTexture;
class CInputMouse;
class CInputKeyboard;
class CInputMouse;
class CCamera;
class CLight;
class CEdter;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CManager
{
public:
	enum class MODE
	{
		MODE_EDTER = 0,	// タイトルシーン
		MODE_MAX,	// 全体数
	};
	CManager();	// デフォルトコンストラクタ
	~CManager();// デフォルトデストラクタ
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);	// スクリーンの初期化
	void Uninit(void);	// スクリーンの終了
	void Update(void);	// スクリーンの更新
	void Draw(void);	// スクリーンの描画
	static MODE GetMode(void) { return m_Mode; }	// モード入手
	static MODE GetModeOld(void) { return m_ModeOld; }	// 前回のモード入手
	static void SetMode(MODE mode);	// モードチェンジ
	static CRenderer *GetRenderer(void) { return m_pRenderer; }	// レンダリングクラスの取得
	static CTexture *GetTexture(void) { return m_pTexture; }	// テクスチャクラスの取得
	static CInputMouse *GetMouse(void) { return m_pMouse; }	// キーボードクラスの取得
	static CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }	// キーボードクラスの取得
	static CCamera *GetCamera(void) { return m_pCamera; }	// カメラクラスの取得
	static CLight *GetLight(int nLightNum) { return m_pLight[nLightNum]; }	// ライトクラスの取得
	static void SetScore(int nScore) { m_nScore = nScore; }	// スコアのセット
	static int GetScore(void) { return m_nScore; }	// スコアの取得
private:
	static CRenderer		*m_pRenderer;		//レンダリングクラス
	static CTexture			*m_pTexture;		// テクスチャクラス
	static CInputMouse		*m_pMouse;
	static CInputKeyboard	*m_pInputKeyboard;	// キーボードクラス
	static CCamera			*m_pCamera;	// カメラクラス
	static CLight			*m_pLight[MAX_LIGHT];	// ライトクラス
	static CEdter			*m_pEdter;
	static MODE				m_Mode;		// シーンモード
	static MODE				m_ModeOld;	// シーンモード
	static int m_nScore;	// スコアの保存先
};

#endif