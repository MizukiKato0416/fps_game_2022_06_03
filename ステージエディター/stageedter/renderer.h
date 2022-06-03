//=============================================================================
//
// �����_�����O���� [renderer.h]
// Author : �H������
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CRenderer
{
public:
	CRenderer();	// �f�t�H���g�R���X�g���N�^
	~CRenderer();	// �f�t�H���g�f�X�g���N�^
	HRESULT Init(HWND hWnd, bool bWindow);	// �X�N���[���̏�����
	void Uninit(void);	// �X�N���[���̏I��
	void Update(void);	// �X�N���[���̍X�V
	void Draw(void);	// �X�N���[���̕`��
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }	// �f�o�C�X�̎擾
	POINT GetMousePos(void) { return m_Mouse; }
private:
	HWND m_Hdnd;
	POINT m_Mouse;
	void DrawTxt(void);	// FPS�J�E���^�̕`��
	LPDIRECT3D9				m_pD3D = NULL;		// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9		m_pD3DDevice = NULL;// Device�I�u�W�F�N�g(�`��ɕK�v)
	LPD3DXFONT				m_pFont = NULL;		// �t�H���g�ւ̃|�C���^
};
#endif