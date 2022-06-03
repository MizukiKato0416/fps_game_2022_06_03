//=============================================================================
//
// �L�[�{�[�h���͏��� [keyinput.cpp]
// Author : �H������
//
//=============================================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "keyinput.h"

//=============================================================================
// �f�t�H���g�R���X�g���N�^
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
// �f�t�H���g�f�X�g���N�^
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{

}

//=============================================================================
// ������
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	//�ȗ�
	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	m_pDevice->Acquire();

	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
}

//=============================================================================
// �X�V
//=============================================================================
void CInputKeyboard::Update(void)
{
	BYTE aState[NUM_KEY_MAX];

	//���̓f�o�C�X����f�[�^�擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aState), &aState[0])))
	{
		for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aStateTrigger[nCntKey] = (m_aState[nCntKey] ^ aState[nCntKey]) & aState[nCntKey];
			m_aStateRelease[nCntKey] = (m_aState[nCntKey] ^ aState[nCntKey]) & ~aState[nCntKey];
			m_aState[nCntKey] = aState[nCntKey];	// �L�[�{�[�h�̓��͏��ۑ�
		}
	}
	else
	{
		m_pDevice->Acquire();
	}
}

//=============================================================================
// �ǂ̃L�[�������ꂽ��
//=============================================================================
void CInputKeyboard::GetAllUpdateKey(KEYBIND key)
{
	bool bTrigger = false;	// �g���K�[�łǂ���������Ă�
	bool bPress = false;	// �v���X�łǂ���������Ă�
	int nKeyCode = 0;	// �z��
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
	bool bTrigger = false;	// �g���K�[�łǂ���������Ă�
	bool bPress = false;	// �v���X�łǂ���������Ă�
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
// �ǂ̃L�[��������
//=============================================================================
bool CInputKeyboard::GetAllReleaseKey(void)
{
	bool bRelease = false;	// �g���K�[�łǂ���������Ă�
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
// �L�[�o�C���h
//=============================================================================
void CInputKeyboard::BindKey(KEYBIND key, int keycode)
{
	Bind[key] = keycode;
}

//=============================================================================
// �L�[�{�[�[�h�̏����擾
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