//=============================================================================
//
// ���f���e�X�g���� [modeltester.cpp]
// Author : �H������
//
//=============================================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "gunmodel.h"
#include "manager.h"
#include "renderer.h"
#include "model_single.h"
#include "model.h"
#include "model.h"

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CGunModel::CGunModel(CObject::PRIORITY Priority) : CObject(Priority)
{

}

//=============================================================================
// �f�t�H���g�f�X�g���N�^
//=============================================================================
CGunModel::~CGunModel()
{

}

//=============================================================================
// ���f���̏���������
//=============================================================================
HRESULT CGunModel::Init(void)
{
	m_model = CModelSingle::Create(m_pos, {0.0f, 2.0f, 0.0f}, m_type, NULL, false);

	return S_OK;
}

//=============================================================================
// ���f���̏I������
//=============================================================================
void CGunModel::Uninit(void)
{
	Release();
}

//=============================================================================
// ���f���̍X�V����
//=============================================================================
void CGunModel::Update(void)
{

}

//=============================================================================
// ���f���̕`�揈��
//=============================================================================
void CGunModel::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice; // �f�o�C�X�̃|�C���^
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X���擾����

	//--------------------------------------
	//�v���C���[(���_)�̃}�g���b�N�X�̐ݒ�
	//--------------------------------------
	D3DXMATRIX mtx_rot, mtx_trans, mtx_parent;	//�v�Z�p�}�g���b�N�X

	D3DXMatrixIdentity(&m_mtx_muzzle_world);	//�}�g���b�N�X������

	D3DXVECTOR3 rot = m_model->GetModel()->GetRot();

	//�����̐ݒ�
	D3DXMatrixRotationYawPitchRoll(	&mtx_rot,
									rot.y,
									rot.x,
									rot.z);

	D3DXMatrixMultiply(	&m_mtx_muzzle_world,
						&m_mtx_muzzle_world,
						&mtx_rot);
	//�ʒu
	D3DXMatrixTranslation(	&mtx_trans,
							m_muzzle_pos.x,
							m_muzzle_pos.y,
							m_muzzle_pos.z);

	D3DXMatrixMultiply(	&m_mtx_muzzle_world,
						&m_mtx_muzzle_world,
						&mtx_trans);
	//�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(	D3DTS_WORLD,
							&m_mtx_muzzle_world);

	mtx_parent = *m_mtxParent;

	D3DXMatrixMultiply(	&m_mtx_muzzle_world,
						&m_mtx_muzzle_world,
						&mtx_parent);
}

//=============================================================================
// ���f���̐���
//=============================================================================
CGunModel *CGunModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 muzzle_pos, string type)
{
	CGunModel *gun = NULL;
	if (gun == NULL)
	{
		gun = new CGunModel;
	}

	gun->m_pos = pos;
	gun->m_rot = rot;
	gun->m_muzzle_pos = muzzle_pos;
	gun->m_type = type;

	if (gun != NULL)
	{
		gun->Init();
	}
	return gun;
}
