//=============================================================================
//
// �u���b�N���� [block.cpp]
// Author : �H������
//
//=============================================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "block.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CBlock::CBlock(LAYER_TYPE Layer) : CScene(Layer)
{
	m_bUse = false;
}

//=============================================================================
// �f�t�H���g�f�X�g���N�^
//=============================================================================
CBlock::~CBlock()
{

}

//=============================================================================
// �|���S���̏�����
//=============================================================================
HRESULT CBlock::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int Type)
{
	m_Pos = pos;
	m_Rot = rot;
	m_Scale = scale;
	m_pModel = CModel::Create(Type);
	m_pModel->SetScale(scale);

	return S_OK;
}

HRESULT CBlock::Init(void)
{
	return S_OK;
}

//=============================================================================
// �|���S���̏I��
//=============================================================================
void CBlock::Uninit(void)
{
	m_pModel->Uninit();
	Release();
}

//=============================================================================
// �|���S���̍X�V
//=============================================================================
void CBlock::Update(void)
{
	m_pModel->Update();
}

//=============================================================================
// �|���S���̕`��
//=============================================================================
void CBlock::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = NULL;	//�f�o�C�X�ւ̃|�C���^
	pDevice = CManager::GetRenderer()->GetDevice();	// �f�o�C�X���擾����

	//-----------------------------------
	//�v���C���[�̐ݒ�
	//-----------------------------------
	D3DXMATRIX mtxRot, mtxTrans, m_mtxView;	//�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_MtxWold);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 
									m_Rot.y,
									m_Rot.x, 
									m_Rot.z);
	D3DXMatrixMultiply( &m_MtxWold, 
						&m_MtxWold, 
						&mtxRot);
	//�ʒu�𔽉f
	D3DXMatrixTranslation(	&mtxTrans, 
							m_Pos.x, 
							m_Pos.y, 
							m_Pos.z);
	D3DXMatrixMultiply( &m_MtxWold, 
						&m_MtxWold, 
						&mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWold);

	m_pModel->Draw(m_bUse);
}

//=============================================================================
// ����
//=============================================================================
CBlock *CBlock::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int type)
{
	CBlock *pBlock = NULL;
	if (pBlock == NULL)
	{
		pBlock = new CBlock;
	}
	
	if (pBlock)
	{
		pBlock->Init(pos, rot, scale, type);
	}
	return pBlock;
}