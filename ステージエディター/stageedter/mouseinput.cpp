//=============================================================================
//
// �}�E�X���͏��� [mouseinput.cpp]
// Author : �H������
//
//=============================================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "mouseinput.h"

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CInputMouse::CInputMouse()
{

}

//=============================================================================
// �f�t�H���g�f�X�g���N�^
//=============================================================================
CInputMouse::~CInputMouse()
{

}

//=============================================================================
// ������
//=============================================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	//�ȗ�
	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse2)))
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
void CInputMouse::Uninit(void)
{
	CInput::Uninit();
}

//=============================================================================
// �X�V
//=============================================================================
void CInputMouse::Update(void)
{
	DIMOUSESTATE2 State;

	// �ŐV�̃}�E�X�̏�Ԃ��擾
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
// �}�E�X���̎擾
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
