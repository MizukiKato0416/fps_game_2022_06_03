//================================================
//パッド入力処理
//Author:加藤瑞葵
//================================================
#include "input_pad_x.h"
#include "input.h"

//================================================
//静的メンバ変数宣言
//================================================

//================================================
//デフォルトコンストラクタ
//================================================
CInputPadX::CInputPadX()
{
	for (int nCntPad = 0; nCntPad < XUSER_MAX_COUNT; nCntPad++)
	{
		ZeroMemory(&m_astate[nCntPad], sizeof(XINPUT_STATE));
		ZeroMemory(&m_astateLast[nCntPad], sizeof(XINPUT_STATE));
		ZeroMemory(&m_avibration[nCntPad], sizeof(XINPUT_VIBRATION));
		m_aCntVibration[nCntPad] = 0;
		m_aVibration[nCntPad] = false;
	}
}

//================================================
//デストラクタ
//================================================
 CInputPadX::~ CInputPadX()
{

}

//================================================
//初期化処理
//================================================
HRESULT  CInputPadX::Init(const HINSTANCE &hInstance, const HWND &hWnd)
{
	CInput::Init(hInstance);

	return S_OK;
}

//================================================
//終了処理
//================================================
void CInputPadX::Uninit(void)
{
	XInputEnable(false); // BOOL enable
	CInput::Uninit();
}

//================================================
//更新処理
//================================================
void CInputPadX::Update(void)
{

	XINPUT_STATE state[XUSER_MAX_COUNT];
	for (int nCntPad = 0; nCntPad < XUSER_MAX_COUNT; nCntPad++)
	{
		// Simply get the state of the controler from XInput.
		DWORD dwResult = XInputGetState(nCntPad, &state[nCntPad]);

		if (dwResult == ERROR_SUCCESS)
		{
			// Controler is connected
			m_aConnect[nCntPad] = true;
		}
		else
		{
			// Controler is not connected
			m_aConnect[nCntPad] = false;
		}
	}

	for (int nCntPad = 0; nCntPad < XUSER_MAX_COUNT; nCntPad++)
	{
		if (XInputGetState(nCntPad, &state[nCntPad]) == ERROR_SUCCESS)
		{
			//============================================
			//左スティック入力
			//============================================
			// Zero value if thumbsticks are within the dead zone
			if ((m_astate[nCntPad].Gamepad.sThumbLX <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
				m_astate[nCntPad].Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
				(m_astate[nCntPad].Gamepad.sThumbLY <  XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
					m_astate[nCntPad].Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
			{
				m_astate[nCntPad].Gamepad.sThumbLX = 0;
				m_astate[nCntPad].Gamepad.sThumbLY = 0;
			}

			//============================================
			//右スティック入力
			//============================================
			// Zero value if thumbsticks are within the dead zone
			if ((m_astate[nCntPad].Gamepad.sThumbRX <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
				m_astate[nCntPad].Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
				(m_astate[nCntPad].Gamepad.sThumbRY <  XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
					m_astate[nCntPad].Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
			{
				m_astate[nCntPad].Gamepad.sThumbRX = 0;
				m_astate[nCntPad].Gamepad.sThumbRY = 0;
			}

			//バイブレーション
			if (m_aCntVibration[nCntPad] > 0)
			{
				m_aCntVibration[nCntPad]--;
			}
			if (m_aCntVibration[nCntPad] <= 0 && m_aVibration[nCntPad])
			{
				SetVibration(0, 0, 0, nCntPad);
				m_aVibration[nCntPad] = false;
			}

			m_astateLast[nCntPad] = m_astate[nCntPad];
			m_astate[nCntPad] = state[nCntPad];
		}
	}
}

//=======================================================================
//パッドの入力情報取得
//=======================================================================
bool CInputPadX::GetButtonPress(const int &nButton, const int &nCntPad)
{
//	return m_astate[nCntPad].Gamepad.wButtons & nButton ? true : false;
	return (m_astate[nCntPad].Gamepad.wButtons & nButton) != 0;
}

bool CInputPadX::GetButtonTrigger(const int &nButton, const int &nCntPad)
{
//	return (((m_astateLast[nCntPad].Gamepad.wButtons & nButton) == false) && (m_astate[nCntPad].Gamepad.wButtons & nButton)) ? true : false;
	return ((m_astateLast[nCntPad].Gamepad.wButtons & nButton) == 0) && ((m_astate[nCntPad].Gamepad.wButtons & nButton) != 0);
}

bool CInputPadX::GetTriggerPress(const int &nTrigger, const int &nCntPad)
{
	if (m_aConnect[nCntPad] == false)
	{
		return false;
	}

	switch (nTrigger)
	{
	case TRIGGERTYPE_RIGHT:
		return m_astate[nCntPad].Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
	case TRIGGERTYPE_LEFT:
		return m_astate[nCntPad].Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
	default:
		return false;
	}
}

bool CInputPadX::GetTriggerTrigger(const int &nTrigger, const int &nCntPad)
{
	if (m_aConnect[nCntPad] == false)
	{
		return false;
	}

	switch (nTrigger)
	{
	case TRIGGERTYPE_RIGHT:
		return (((m_astateLast[nCntPad].Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) == false) && (m_astate[nCntPad].Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)) ? true : false;
	case TRIGGERTYPE_LEFT:
		return (((m_astateLast[nCntPad].Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD) == false) && (m_astate[nCntPad].Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)) ? true : false;
	default:
		return false;
	}
}

bool CInputPadX::GetLeftStick(const int &nLstick, const int &nCntPad)
{
	if (m_aConnect[nCntPad] == false)
	{
		return false;
	}

	switch (nLstick)
	{
	case STICKTYPE_UP:
		return m_astate[nCntPad].Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ? true : false;
		break;
	case STICKTYPE_RIGHT:
		return m_astate[nCntPad].Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ? true : false;
		break;
	case STICKTYPE_LEFT:
		return m_astate[nCntPad].Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ? true : false;
		break;
	case STICKTYPE_DOWN:
		return m_astate[nCntPad].Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE ? true : false;
		break;
	default:
		return false;
		break;
	}
}

bool CInputPadX::GetLeftStickTrigger(const int &nLstick, const int &nCntPad)//不完全
{
	if (m_aConnect[nCntPad] == false)
	{
		return false;
	}

	switch (nLstick)
	{
	case STICKTYPE_UP:
		return (((m_astateLast[nCntPad].Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) == false) && (m_astate[nCntPad].Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) ? true : false;
		break;
	case STICKTYPE_RIGHT:
		return (((m_astateLast[nCntPad].Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) == false) && (m_astate[nCntPad].Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) ? true : false;
		break;
	case STICKTYPE_LEFT:
		return (((m_astateLast[nCntPad].Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) == false) && (m_astate[nCntPad].Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) ? true : false;
		break;
	case STICKTYPE_DOWN:
		return (((m_astateLast[nCntPad].Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) == false) && (m_astate[nCntPad].Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) ? true : false;
		break;
	default:
		return false;
		break;
	}
}

bool CInputPadX::GetRightStick(const int &nRstick, const int &nCntPad)
{
	if (m_aConnect[nCntPad] == false)
	{
		return false;
	}

	switch (nRstick)
	{
	case STICKTYPE_UP:
		return m_astate[nCntPad].Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ? true : false;
		break;
	case STICKTYPE_RIGHT:
		return m_astate[nCntPad].Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ? true : false;
		break;
	case STICKTYPE_LEFT:
		return m_astate[nCntPad].Gamepad.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ? true : false;
		break;
	case STICKTYPE_DOWN:
		return m_astate[nCntPad].Gamepad.sThumbRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE ? true : false;
		break;
	default:
		return false;
		break;
	}
}

bool CInputPadX::GetLeftStickRelease(const int &nLstick, const int &nCntPad)
{
	if (m_aConnect[nCntPad] == false)
	{
		return false;
	}

	switch (nLstick)
	{
	case STICKTYPE_UP:
		return (((m_astate[nCntPad].Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) == false) && (m_astateLast[nCntPad].Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) ? true : false;
		break;
	case STICKTYPE_RIGHT:
		return (((m_astate[nCntPad].Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) == false) && (m_astateLast[nCntPad].Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) ? true : false;
		break;
	case STICKTYPE_LEFT:
		return (((m_astate[nCntPad].Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) == false) && (m_astateLast[nCntPad].Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) ? true : false;
		break;
	case STICKTYPE_DOWN:
		return (((m_astate[nCntPad].Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) == false) && (m_astateLast[nCntPad].Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)) ? true : false;
		break;
	default:
		return false;
		break;
	}
}

//=======================================================================
//バイブレーション設定
//=======================================================================
void CInputPadX::SetVibration(const int &nLeftSpeed, const int &nRightSpeed, const int &nCntEnd, const int &nCntPad)
{
	m_avibration[nCntPad].wLeftMotorSpeed = static_cast<WORD>(nLeftSpeed);
	m_avibration[nCntPad].wRightMotorSpeed = static_cast<WORD>(nRightSpeed);
	XInputSetState(nCntPad, &m_avibration[nCntPad]);
	m_aCntVibration[nCntPad] = nCntEnd;
	m_aVibration[nCntPad] = true;
}