//===========================================
//パッド入力処理
//Author:加藤瑞葵
//===========================================
#ifndef _INPUT_PAD_X_H_
#define _INPUT_PAD_X_H_
#include "input.h"

//================================================
//マクロ定義
//================================================

//========================================================
//構造体の定義
//========================================================

//================================================
//クラスの定義
//================================================
//クラスの定義
class CInputPadX : public CInput
{
public:
	typedef enum
	{
		TRIGGERTYPE_RIGHT = 0,
		TRIGGERTYPE_LEFT,
		TRIGGERTYPE_MAX
	} TRIGGERTYPE;
	typedef enum
	{
		STICKTYPE_UP = 0,
		STICKTYPE_RIGHT,
		STICKTYPE_LEFT,
		STICKTYPE_DOWN,
		STICKTYPE_MAX
	} STICKTYPE;

	CInputPadX();			//コンストラクタ
	~CInputPadX();			//デストラクタ

	//メンバ関数
	HRESULT Init(const HINSTANCE &hInstance, const HWND &hWnd);
	void Uninit(void);
	void Update(void);
	bool GetButtonPress(const int &nButton, const int &nCntPad);
	bool GetButtonTrigger(const int &nButton, const int &nCntPad);
	bool GetTriggerPress(const int &nTrigger, const int &nCntPad);
	bool GetTriggerTrigger(const int &nTrigger, const int &nCntPad);
	bool GetLeftStick(const int &nLstick, const int &nCntPad);
	bool GetLeftStickTrigger(const int &nLstick, const int &nCntPad);
	bool GetLeftStickRelease(const int &nLstick, const int &nCntPad);
	bool GetRightStick(const int &nRstick, const int &nCntPad);
	void SetVibration(const int &nLeftSpeed, const int &nRightSpeed, const int &nCntEnd, const int &nCntPad);

private:
	XINPUT_STATE m_astate[XUSER_MAX_COUNT];
	XINPUT_STATE m_astateLast[XUSER_MAX_COUNT];
	XINPUT_VIBRATION m_avibration[XUSER_MAX_COUNT];
	bool m_aConnect[XUSER_MAX_COUNT];
	int m_aCntVibration[XUSER_MAX_COUNT];
	bool m_aVibration[XUSER_MAX_COUNT];
};

#endif // !_INPUT_PAD_X_H_