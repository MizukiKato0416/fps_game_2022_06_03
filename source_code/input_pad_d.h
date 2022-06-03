//===========================================
//パッド入力(ダイレクト)処理
//Author:加藤瑞葵
//===========================================
#ifndef _INPUT_PAD_D_H_
#define _INPUT_PAD_D_H_
#include "input.h"

//================================================
//マクロ定義
//================================================
#define STICK_POW (1000)

//========================================================
//構造体の定義
//========================================================

//================================================
//クラスの定義
//================================================
//クラスの定義
class CInputPadD : public CInput
{
public:
	//ボタンの種類
	typedef enum
	{
		X = 0,
		Y,
		A,
		B,
		L,
		R,
		L2,
		R2,
		L_STICK,
		R_STICK,
		BACK,
		START,
		BUTTOM,
		HOME,
		BUTTON_MAX,
	} PADBUTTON;

	//メンバ関数
	CInputPadD();		//デフォルトコンストラクタ
	~CInputPadD();		//デストラクタ

	HRESULT Init(const HINSTANCE &hInstance, const HWND &hWnd);		//初期化
	void Uninit(void);									//終了
	void Update(void);									//更新

	bool GetPress(const int &Botton_Type);						//押す
	bool GetTrigger(const int &Botton_Type);					//一回押す
	bool GetRelease(const int &Botton_Type);					//離した
	DIJOYSTATE2 GetGamepad(void);						//ゲームパッド情報取得
	DIJOYSTATE2 GetGamepadOld(void);					//1フレーム前のゲームパッド情報取得
	static BOOL CALLBACK EnumJoysticksCallback(const LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);
	static BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext);

private:
	DIDEVCAPS m_diDevCaps;
	DIJOYSTATE2 m_aState;				//入力情報
	DIJOYSTATE2 m_aStateOld;			//1フレーム前の入力情報
	DIJOYSTATE2 m_aStateTrigger;        //入力情報(トリガー)
	DIJOYSTATE2 m_aStateRelease;        //入力情報(リリース)
};
#endif // !_INPUT_PAD_D_H_