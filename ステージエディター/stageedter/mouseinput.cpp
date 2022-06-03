//=============================================================================
//
// マウス入力処理 [mouseinput.cpp]
// Author : 羽鳥太一
//
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include "mouseinput.h"

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CInputMouse::CInputMouse()
{

}

//=============================================================================
// デフォルトデストラクタ
//=============================================================================
CInputMouse::~CInputMouse()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	//省略
	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	m_pDevice->Acquire();

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CInputMouse::Uninit(void)
{
	CInput::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CInputMouse::Update(void)
{
	DIMOUSESTATE2 State;

	// 最新のマウスの状態を取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(State), &State)))
	{
		for (int nCntBotton = 0; nCntBotton < BOTTON_MAX; nCntBotton++)
		{
			m_aStateTrigger.rgbButtons[nCntBotton] = (m_aState.rgbButtons[nCntBotton] ^ State.rgbButtons[nCntBotton]) & State.rgbButtons[nCntBotton];
			m_aStateRelease.rgbButtons[nCntBotton] = (m_aState.rgbButtons[nCntBotton] ^ State.rgbButtons[nCntBotton]) & ~State.rgbButtons[nCntBotton];
		}
		m_aState = State;
	}
	else
	{
		m_pDevice->Acquire();
	}
}

//=============================================================================
// マウス情報の取得
//=============================================================================
bool CInputMouse::GetPress(int Botton_Type)
{
	return (m_aState.rgbButtons[Botton_Type] & 0x80) ? true : false;
}

bool CInputMouse::GetTrigger(int Botton_Type)
{
	return (m_aStateTrigger.rgbButtons[Botton_Type] & 0x80) ? true : false;
}

bool CInputMouse::GetRelease(int Botton_Type)
{
	return (m_aStateRelease.rgbButtons[Botton_Type] & 0x80) ? true : false;
}

DIMOUSESTATE2 CInputMouse::GetMousePos(void)
{
	return m_aState;
}
