//=============================================================================
//
// �}�E�X���� [mouseinput.h]
// Author : �H������
//
//=============================================================================
#ifndef _MOUSEINPUT_H_
#define _MOUSEINPUT_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "input.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CInputMouse : public CInput
{
public:
	typedef enum
	{
		LEFT_BOTTON = 0,	// ���N���b�N
		RIGHT_BOTTON,	// �E�N���b�N
		WHEEL_BOTTON,	// �z�C�[���N���b�N
		BOTTON_MAX,
	} Type;
	CInputMouse();	// �f�t�H���g�R���X�g���N�^
	~CInputMouse();	// �f�t�H���g�f�X�g���N�^
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	// ������
	void Uninit(void);	// �I��
	void Update(void);	// �X�V
	bool GetPress(int Botton_Type);	// ����
	bool GetTrigger(int Botton_Type);	// ��񉟂�
	bool GetRelease(int Botton_Type);	// ������
	DIMOUSESTATE2 GetMousePos(void);	// �}�E�X���W
private:
	DIMOUSESTATE2 m_Mouse;		//�}�E�X
	DIMOUSESTATE2 m_aState;				//�L�[�{�[�h�̓��͏��i�v���X���j
	DIMOUSESTATE2 m_aStateTrigger;		//�L�[�{�[�h�̓��͏�� (�g���K�[)
	DIMOUSESTATE2 m_aStateRelease;		//�L�[�{�[�h�̓��͏�� (�����[�X)
};

#endif