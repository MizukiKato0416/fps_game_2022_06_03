//================================================
//マウス入力処理
//Author:加藤瑞葵
//================================================
#include "input_mouse.h"
#include "input.h"

//================================================
//静的メンバ変数宣言
//================================================

//================================================
//デフォルトコンストラクタ
//================================================
CInputMouse::CInputMouse()
{
	
}

//================================================
//デストラクタ
//================================================
 CInputMouse::~ CInputMouse()
{

}

//================================================
//初期化処理
//================================================
HRESULT  CInputMouse::Init(const HINSTANCE &hInstance, const HWND &hWnd)
{
	CInput::Init(hInstance);

	// 入力デバイスの生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// マウスへのアクセス権を獲得(入力制御開始)
	m_pDevice->Acquire();

	return S_OK;
}

//================================================
//終了処理
//================================================
void  CInputMouse::Uninit(void)
{
	CInput::Uninit();
}

//================================================
//更新処理
//================================================
void  CInputMouse::Update(void)
{
	DIMOUSESTATE2 mouseState;
	int nCntKey;

	//入力デバイスからデータ取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState)))
	{
		for (nCntKey = 0; nCntKey < MOUSE_TYPE_MAX; nCntKey++)
		{
			m_mouseStateTrigger.rgbButtons[nCntKey] = (m_mouseState.rgbButtons[nCntKey] ^ mouseState.rgbButtons[nCntKey]) & mouseState.rgbButtons[nCntKey];
			m_mouseStateRelease.rgbButtons[nCntKey] = (m_mouseState.rgbButtons[nCntKey] ^ mouseState.rgbButtons[nCntKey]) & ~mouseState.rgbButtons[nCntKey];
			m_mouseState = mouseState;	//マウスの入力情報保存
		}
	}
	else
	{
		m_pDevice->Acquire();
	}
}

//=======================================================================
//マウスの入力情報取得
//=======================================================================
bool CInputMouse::GetPress(const int &nMouse)
{
	return(m_mouseState.rgbButtons[nMouse] & 0x80) ? true : false;
}

bool CInputMouse::GetTrigger(const int &nMouse)
{
	return(m_mouseStateTrigger.rgbButtons[nMouse] & 0x80) ? true : false;
}

bool CInputMouse::GetRelease(const int &nMouse)
{
	return(m_mouseStateRelease.rgbButtons[nMouse] & 0x80) ? true : false;
}

//=======================================================================
//マウスの入力情報取得処理
//=======================================================================
DIMOUSESTATE2 CInputMouse::GetMouseState(void)
{
	return m_mouseState;
}

//=======================================================================
//マウスの移動量取得処理
//=======================================================================
D3DXVECTOR2 CInputMouse::GetMouseVelocity(void)
{
	return D3DXVECTOR2((float)m_mouseState.lX, (float)m_mouseState.lY);
}