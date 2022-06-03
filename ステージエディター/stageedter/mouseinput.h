//=============================================================================
//
// マウス処理 [mouseinput.h]
// Author : 羽鳥太一
//
//=============================================================================
#ifndef _MOUSEINPUT_H_
#define _MOUSEINPUT_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "input.h"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CInputMouse : public CInput
{
public:
	typedef enum
	{
		LEFT_BOTTON = 0,	// 左クリック
		RIGHT_BOTTON,	// 右クリック
		WHEEL_BOTTON,	// ホイールクリック
		BOTTON_MAX,
	} Type;
	CInputMouse();	// デフォルトコンストラクタ
	~CInputMouse();	// デフォルトデストラクタ
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	bool GetPress(int Botton_Type);	// 押す
	bool GetTrigger(int Botton_Type);	// 一回押す
	bool GetRelease(int Botton_Type);	// 離した
	DIMOUSESTATE2 GetMousePos(void);	// マウス座標
private:
	DIMOUSESTATE2 m_Mouse;		//マウス
	DIMOUSESTATE2 m_aState;				//キーボードの入力情報（プレス情報）
	DIMOUSESTATE2 m_aStateTrigger;		//キーボードの入力情報 (トリガー)
	DIMOUSESTATE2 m_aStateRelease;		//キーボードの入力情報 (リリース)
};

#endif