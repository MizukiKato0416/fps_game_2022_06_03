//=============================================================================
//
// キーボード処理 [keyinput.h]
// Author : 羽鳥太一
//
//=============================================================================
#ifndef _KEYINPUT_H_
#define _KEYINPUT_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_KEY_MAX (256)//キーの最大数

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CInputKeyboard : public CInput
{
public:
	typedef enum
	{
		W = 0,	// W
		A,	// A
		S,	// S
		D,	// D
		UP,
		LEFT,
		RIGHT,
		DOWN,
		T,
		B,
		Z,
		C,
		Y,
		N,
		U,
		M,
		Q,
		E,
		SPACE,
		LSHIFT,
		F9,
		KEYBINDMAX,	// バインドキー総数
	} KEYBIND;
	CInputKeyboard();	// デフォルトコンストラクタ
	~CInputKeyboard();	// デフォルトデストラクタ
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// 初期化
	void Uninit(void);	// 終了
	void Update(void);	// 更新
	void GetAllUpdateKey(KEYBIND key);	// どのキーが押されたか
	void BindKey(KEYBIND key, int keycode);	// キーバインド
	bool GetPress(KEYBIND nKey);	// 押す
	bool GetTrigger(KEYBIND nKey);	// 一回押す
	bool GetRelease(KEYBIND nKey);	// 離した
	bool GetAllPushKey(void);	// 全部のキー確認
	bool GetAllReleaseKey(void);	// どのキーが押されたか
private:
	BYTE	m_aState[NUM_KEY_MAX];				//キーボードの入力情報（プレス情報）
	BYTE	m_aStateTrigger[NUM_KEY_MAX];		//キーボードの入力情報 (トリガー)
	BYTE	m_aStateRelease[NUM_KEY_MAX];		//キーボードの入力情報 (リリース)
	int Bind[KEYBINDMAX];// キーバインド情報
};

#endif