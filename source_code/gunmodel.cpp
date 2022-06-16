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
	m_model = CModel::Create({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, m_type);

	return S_OK;
}

//=============================================================================
// ���f���̏I������
//=============================================================================
void CGunModel::Uninit(void)
{
	Release();
	m_model->Uninit();
}

//=============================================================================
// ���f���̍X�V����
//=============================================================================
void CGunModel::Update(void)
{
	m_model->Update();
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

	D3DXMatrixIdentity(&m_mtx_world);	//�}�g���b�N�X������

	//�����̐ݒ�
	D3DXMatrixRotationYawPitchRoll(	&mtx_rot,
									m_rot.y,
									m_rot.x,
									m_rot.z);

	D3DXMatrixMultiply(	&m_mtx_world,
						&m_mtx_world,
						&mtx_rot);
	//�ʒu
	D3DXMatrixTranslation(	&mtx_trans,
							m_pos.x,
							m_pos.y,
							m_pos.z);

	D3DXMatrixMultiply(	&m_mtx_world,
						&m_mtx_world,
						&mtx_trans);
	//�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(	D3DTS_WORLD,
							&m_mtx_world);
	m_model->Draw();

	pDevice->GetTransform(	D3DTS_WORLD,
							&mtx_parent);

	D3DXMatrixIdentity(&m_mtx_muzzle_world);	//�}�g���b�N�X������

	//�����̐ݒ�
	D3DXMatrixRotationYawPitchRoll(	&mtx_rot,
									0.0f,
									0.0f,
									0.0f);

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

	//�Z�o�����p�[�c�̃��[���h�}�g���b�N�X�Ɛe�̃��[���h�}�g���b�N�X���|�����킹��
	D3DXMatrixMultiply(	&m_mtx_muzzle_world,
						&m_mtx_muzzle_world,
						&mtx_parent);

	//�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(	D3DTS_WORLD,
							&m_mtx_muzzle_world);
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
