//=============================================================================
//
// �}�l�[�W���[�Ǘ����� [namager.h]
// Author : �H������
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "light.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CRenderer;
class CTexture;
class CInputMouse;
class CInputKeyboard;
class CInputMouse;
class CCamera;
class CLight;
class CEdter;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CManager
{
public:
	enum class MODE
	{
		MODE_EDTER = 0,	// �^�C�g���V�[��
		MODE_MAX,	// �S�̐�
	};
	CManager();	// �f�t�H���g�R���X�g���N�^
	~CManager();// �f�t�H���g�f�X�g���N�^
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);	// �X�N���[���̏�����
	void Uninit(void);	// �X�N���[���̏I��
	void Update(void);	// �X�N���[���̍X�V
	void Draw(void);	// �X�N���[���̕`��
	static MODE GetMode(void) { return m_Mode; }	// ���[�h����
	static MODE GetModeOld(void) { return m_ModeOld; }	// �O��̃��[�h����
	static void SetMode(MODE mode);	// ���[�h�`�F���W
	static CRenderer *GetRenderer(void) { return m_pRenderer; }	// �����_�����O�N���X�̎擾
	static CTexture *GetTexture(void) { return m_pTexture; }	// �e�N�X�`���N���X�̎擾
	static CInputMouse *GetMouse(void) { return m_pMouse; }	// �L�[�{�[�h�N���X�̎擾
	static CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }	// �L�[�{�[�h�N���X�̎擾
	static CCamera *GetCamera(void) { return m_pCamera; }	// �J�����N���X�̎擾
	static CLight *GetLight(int nLightNum) { return m_pLight[nLightNum]; }	// ���C�g�N���X�̎擾
	static void SetScore(int nScore) { m_nScore = nScore; }	// �X�R�A�̃Z�b�g
	static int GetScore(void) { return m_nScore; }	// �X�R�A�̎擾
private:
	static CRenderer		*m_pRenderer;		//�����_�����O�N���X
	static CTexture			*m_pTexture;		// �e�N�X�`���N���X
	static CInputMouse		*m_pMouse;
	static CInputKeyboard	*m_pInputKeyboard;	// �L�[�{�[�h�N���X
	static CCamera			*m_pCamera;	// �J�����N���X
	static CLight			*m_pLight[MAX_LIGHT];	// ���C�g�N���X
	static CEdter			*m_pEdter;
	static MODE				m_Mode;		// �V�[�����[�h
	static MODE				m_ModeOld;	// �V�[�����[�h
	static int m_nScore;	// �X�R�A�̕ۑ���
};

#endif