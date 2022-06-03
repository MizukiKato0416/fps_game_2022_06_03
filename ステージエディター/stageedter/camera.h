//=============================================================================
//
// �J�������� [camera.h]
// Author : �H������
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CAMERA_DESTAANCE (200)		//�J�����ƒ����_�̋���

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CCamera
{
public:
	CCamera();	// �f�t�H���g�R���X�g���N�^
	~CCamera();	// �f�t�H���g�f�X�g���N�^
	HRESULT Init(D3DXVECTOR3 PosV, D3DXVECTOR3 PosR, D3DXVECTOR3 Rot);	// �J�����̏�����
	void Uninit(void);	// �J�����̏I��
	void Update(void);	// �J�����̍X�V
	void SetCamera(void);	// �J�����̕`��
	static CCamera *Create(D3DXVECTOR3 PosV, D3DXVECTOR3 PosR, D3DXVECTOR3 Rot);
	CCamera *GetCamera(void) { return this; }	// �J�����̓���
	D3DXMATRIX GetViewMtx(void) { return m_mtxView; }
	D3DXMATRIX GetProjecMtx(void) { return m_mtxProjection; }
	D3DXVECTOR3 *CalcScreenToWorld(D3DXVECTOR3* pout, int Sx, int Sy, float fZ, int Screen_w, int Screen_h, D3DXMATRIX* View, D3DXMATRIX* Prj);	// �X�N���[�����W�����[���h���W�ɕϊ�
	D3DXVECTOR3 *GetMousePos(void) { return &m_MouseWorldPos; }
private:
	void Move(void);			//����

	D3DXVECTOR3 m_PosV;			//���_
	D3DXVECTOR3 m_PosR;			//�����_
	D3DXVECTOR3 m_Rot;			//�J�����̌���
	D3DXVECTOR3 m_VecU;			//������x�N�g��
	D3DXVECTOR3 m_MouseWorldPos;
	D3DXMATRIX m_mtxProjection;	//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX m_mtxView;		//�r���[�}�g���b�N�X
	float Long;					//��
	float m_fDifferVR;
};
#endif