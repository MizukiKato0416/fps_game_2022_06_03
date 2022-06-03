//===========================================
//パッド入力(ダイレクト)処理
//Author:加藤瑞葵
//===========================================
#include "input_pad_d.h"

//================================================
//静的メンバ変数宣言
//================================================

//=============================================================================
//デフォルトコンストラクタ
//=============================================================================
CInputPadD::CInputPadD()
{

}

//=============================================================================
//デストラクタ
//=============================================================================
CInputPadD::~CInputPadD()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CInputPadD::Init(const HINSTANCE &hInstance, const HWND &hWnd)
{
	CInput::Init(hInstance);

	m_pInput->EnumDevices(	DI8DEVCLASS_GAMECTRL, 
							EnumJoysticksCallback,
							NULL,
							DIEDFL_ATTACHEDONLY);

	if (m_pDeviceJoyStick != nullptr)
	{
		//協調モードを設定
		if (FAILED(m_pDeviceJoyStick->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
		{
			return E_FAIL;
		}
	}
	else
	{
		return E_FAIL;
	}

	m_pDeviceJoyStick->GetCapabilities(&m_diDevCaps);

	m_pDeviceJoyStick->EnumObjects(EnumAxesCallback, (VOID*)hWnd, DIDFT_AXIS);

	m_pDeviceJoyStick->Poll();

	// アクセス権を獲得(入力制御開始)
	m_pDeviceJoyStick->Acquire();

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CInputPadD::Uninit(void)
{
	CInput::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CInputPadD::Update(void)
{
	if (m_pDeviceJoyStick != nullptr)
	{
		//状態を変える前に保存
		m_aStateOld = m_aState;

		DIJOYSTATE2 state;
		//m_aState.rglSlider[0] = m_pDeviceJoyStick->GetObjectInfo();
		//入力デバイスからデータ取得
		if (SUCCEEDED(m_pDeviceJoyStick->GetDeviceState(sizeof(state), &state)))
		{
			for (int nCntKey = 0; nCntKey < BUTTON_MAX; nCntKey++)
			{
				m_aStateTrigger.rgbButtons[nCntKey] = (m_aState.rgbButtons[nCntKey] ^ state.rgbButtons[nCntKey]) & state.rgbButtons[nCntKey];
				m_aStateRelease.rgbButtons[nCntKey] = (m_aState.rgbButtons[nCntKey] ^ state.rgbButtons[nCntKey]) & ~state.rgbButtons[nCntKey];
			}
			m_aState = state;
		}
		else
		{
			m_pDeviceJoyStick->Acquire();
		}
	}
}

//=============================================================================
// ボタン情報の取得
//=============================================================================
bool CInputPadD::GetPress(const int &Botton_Type)
{
	return (m_aState.rgbButtons[Botton_Type] & 0x80) ? true : false;
}

bool CInputPadD::GetTrigger(const int &Botton_Type)
{
	return (m_aStateTrigger.rgbButtons[Botton_Type] & 0x80) ? true : false;
}

bool CInputPadD::GetRelease(const int &Botton_Type)
{
	return (m_aStateRelease.rgbButtons[Botton_Type] & 0x80) ? true : false;
}

DIJOYSTATE2 CInputPadD::GetGamepad(void)
{
	return m_aState;
}

DIJOYSTATE2 CInputPadD::GetGamepadOld(void)
{
	return m_aStateOld;
}

//=============================================================================
//コールバック関数
//=============================================================================
BOOL CALLBACK CInputPadD::EnumJoysticksCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID /*pvRef*/)
{
	// ジョイスティックデバイスの作成
	HRESULT ret = m_pInput->CreateDevice(lpddi->guidInstance, &m_pDeviceJoyStick, NULL);
	if (FAILED(ret)) 
	{
		return DIENUM_STOP;
	}

	// 入力データ形式のセット
	ret = m_pDeviceJoyStick->SetDataFormat(&c_dfDIJoystick2);
	if (FAILED(ret)) 
	{
		m_pDeviceJoyStick->Release();
		return DIENUM_STOP;
	}

	return DIENUM_STOP;
}

BOOL CALLBACK CInputPadD::EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID * /*pContext*/)
{
	DIPROPRANGE diprg;

	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYID;
	diprg.diph.dwObj = pdidoi->dwType;
	diprg.lMin = -STICK_POW;
	diprg.lMax = STICK_POW;

	if (FAILED(m_pDeviceJoyStick->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;
}