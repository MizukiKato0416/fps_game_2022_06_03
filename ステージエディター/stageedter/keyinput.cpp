//=============================================================================
//
// キーボード入力処理 [keyinput.cpp]
// Author : 羽鳥太一
//
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include "keyinput.h"

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CInputKeyboard::CInputKeyboard()
{
	memset(&m_aState[0], 0, sizeof(m_aState));
	memset(&m_aStateTrigger[0], 0, sizeof(m_aStateTrigger));
	memset(&m_aStateRelease[0], 0, sizeof(m_aStateRelease));
	for (int nCntKey = 0; nCntKey < KEYBINDMAX; nCntKey++)
	{
		Bind[nCntKey] = -1;
	}
}

//=============================================================================
// デフォルトデストラクタ
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	//省略
	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
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
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CInputKeyboard::Update(void)
{
	BYTE aState[NUM_KEY_MAX];

	//入力デバイスからデータ取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aState), &aState[0])))
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aStateTrigger[nCntKey] = (m_aState[nCntKey] ^ aState[nCntKey]) & aState[nCntKey];
			m_aStateRelease[nCntKey] = (m_aState[nCntKey] ^ aState[nCntKey]) & ~aState[nCntKey];
			m_aState[nCntKey] = aState[nCntKey];	// キーボードの入力情報保存
		}
	}
	else
	{
		m_pDevice->Acquire();
	}
}

//=============================================================================
// どのキーが押されたか
//=============================================================================
void CInputKeyboard::GetAllUpdateKey(KEYBIND key)
{
	bool bTrigger = false;	// トリガーでどこか押されてる
	bool bPress = false;	// プレスでどこか押されてる
	int nKeyCode = 0;	// 配列
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		bPress = (m_aState[nCntKey] & 0x80) ? true : false;
		bTrigger = (m_aStateTrigger[nCntKey] & 0x80) ? true : false;
		if (bPress == true)
		{
			nKeyCode = nCntKey;
			break;
		}
		else if (bTrigger == true)
		{
			nKeyCode = nCntKey;
			break;
		}
	}
	BindKey(key, nKeyCode);
}

bool CInputKeyboard::GetAllPushKey(void)
{
	bool bTrigger = false;	// トリガーでどこか押されてる
	bool bPress = false;	// プレスでどこか押されてる
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		bPress = (m_aState[nCntKey] & 0x80) ? true : false;
		bTrigger = (m_aStateTrigger[nCntKey] & 0x80) ? true : false;
		if (bPress == true)
		{
			return bPress;
			break;
		}
		else if (bTrigger == true)
		{
			return bTrigger;
			break;
		}
	}
	return false;
}

//=============================================================================
// どのキーが離した
//=============================================================================
bool CInputKeyboard::GetAllReleaseKey(void)
{
	bool bRelease = false;	// トリガーでどこか押されてる
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		bRelease = (m_aStateRelease[nCntKey] & 0x80) ? true : false;
		if (bRelease == true)
		{
			return bRelease;
			break;
		}
	}
	return false;
}

//=============================================================================
// キーバインド
//=============================================================================
void CInputKeyboard::BindKey(KEYBIND key, int keycode)
{
	Bind[key] = keycode;
}

//=============================================================================
// キーボーードの情報を取得
//=============================================================================
bool CInputKeyboard::GetPress(KEYBIND nKey)
{
	return (m_aState[Bind[nKey]] & 0x80) ? true : false;
}

bool CInputKeyboard::GetTrigger(KEYBIND nKey)
{
	return (m_aStateTrigger[Bind[nKey]] & 0x80) ? true : false;
}

bool CInputKeyboard::GetRelease(KEYBIND nKey)
{
	return (m_aStateRelease[Bind[nKey]] & 0x80) ? true : false;
}