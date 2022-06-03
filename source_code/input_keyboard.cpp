//================================================
//キーボード入力処理
//Author:加藤瑞葵
//================================================
#include "input_keyboard.h"
#include "input.h"

//================================================
//静的メンバ変数宣言
//================================================

//================================================
//デフォルトコンストラクタ
//================================================
CInputKeyboard::CInputKeyboard()
{
	memset(&m_aKeyState[0], 0, sizeof(m_aKeyState));
	memset(&m_aKeyStateRelease[0], 0, sizeof(m_aKeyState));
	memset(&m_aKeyStateTrigger[0], 0, sizeof(m_aKeyState));
}

//================================================
//デストラクタ
//================================================
 CInputKeyboard::~ CInputKeyboard()
{

}

//================================================
//初期化処理
//================================================
HRESULT  CInputKeyboard::Init(const HINSTANCE &hInstance, const HWND &hWnd)
{
	CInput::Init(hInstance);

	// 入力デバイス(キーボード)の生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

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

//================================================
//終了処理
//================================================
void  CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
}

//================================================
//更新処理
//================================================
void  CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];
	int nCntKey;

	//入力デバイスからデータ取得
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & ~aKeyState[nCntKey];
			m_aKeyState[nCntKey] = aKeyState[nCntKey];	//キーボードの入力情報保存
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
pair<int, bool> CInputKeyboard::GetAllKeyUpdate(void)
{
	pair<int, bool> KeyUpdate;
	bool bTrigger = false;	// トリガーでどこか押されてる
	bool bKeyBoard = false;	
	for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		bTrigger = (m_aKeyStateTrigger[nCntKey] & 0x80) ? true : false;
		if (bTrigger == true)
		{
			KeyUpdate.first = nCntKey;
			bKeyBoard = true;
			break;
		}
	}
	KeyUpdate.second = bKeyBoard;

	return KeyUpdate;
}

//=======================================================================
//キーボードの入力情報取得
//=======================================================================
bool CInputKeyboard::GetPress(const int &nKey)
{
	return(m_aKeyState[nKey] & 0x80) ? true : false;
}

bool CInputKeyboard::GetTrigger(const int &nKey)
{
	return(m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

bool CInputKeyboard::GetRelease(const int &nKey)
{
	return(m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}