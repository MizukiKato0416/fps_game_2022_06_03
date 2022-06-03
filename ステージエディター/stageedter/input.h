//=============================================================================
//
// ���͏��� [input.h]
// Author : �H������
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CInput
{
public:
	CInput();	// �f�t�H���g�R���X�g���N�^
	virtual ~CInput();	// �f�t�H���g�f�X�g���N�^
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// ������
	virtual void Uninit(void);		// �I��
	virtual void Update(void) = 0;	// �X�V
protected:
	static LPDIRECTINPUT8	m_pInput;	// directinput�f�o�C�X
	LPDIRECTINPUTDEVICE8	m_pDevice;	// �e�f�o�C�X
};

#endif