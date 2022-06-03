//=============================================================================
//
// ���C�g���� [light.h]
// Author : �H������
//
//=============================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_LIGHT (3)			//���C�g�̐�

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CLight
{
public:
	CLight();	// �f�t�H���g�R���X�g���N�^
	~CLight();	// �f�t�H���g�f�X�g���N�^
	HRESULT Init(D3DLIGHTTYPE Type, D3DCOLORVALUE Diffuse, D3DVECTOR Pos, D3DVECTOR Dir);	// �J�����̏�����
	void Uninit(void);	// �J�����̏I��
	void Update(void);	// �J�����̍X�V
	static CLight *Create(D3DLIGHTTYPE Type, D3DCOLORVALUE Diffuse, D3DVECTOR Pos, D3DVECTOR Dir);
	CLight *GetLight(void) { return this; }	// �J�����̓���
private:
	D3DLIGHT9 m_Light;	// ���C�g
	static int m_nMaxNumber;	// ���C�g�̐�
	int m_nMyNumber;	// �����̔ԍ�
};
#endif