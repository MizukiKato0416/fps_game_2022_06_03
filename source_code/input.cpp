//================================================
//入力処理
//Author:加藤瑞葵
//================================================
#include "input.h"

//================================================
//静的メンバ変数宣言
//================================================
LPDIRECTINPUTDEVICE8 CInput::m_pDeviceJoyStick = nullptr;
LPDIRECTINPUT8 CInput::m_pInput = nullptr;

//================================================
//デフォルトコンストラクタ
//================================================
CInput::CInput()
{
	m_pDevice = nullptr;
}

//================================================
//デストラクタ
//================================================
CInput::~CInput()
{

}

//================================================
//初期化処理
//================================================
HRESULT CInput::Init(const HINSTANCE &hInstance/*, const HWND &hWnd*/)
{
	//DirectInputオブジェクトの作成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//================================================
//終了処理
//================================================
void CInput::Uninit(void)
{
	//入力デバイスの開放
	if (m_pDevice != nullptr)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = nullptr;
	}

	//DirectInputオブジェクトの開放
	if (m_pInput != nullptr)
	{
		m_pInput->Release();
		m_pInput = nullptr;
	}
}