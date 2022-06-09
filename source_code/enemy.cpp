//=============================================================================
//
// ���f���e�X�g���� [modeltester.cpp]
// Author : �H������
//
//=============================================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include <thread>

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CEnemy::CEnemy(CObject::PRIORITY Priority) : CObject(Priority)
{
	m_pos ={ 0.0f, 0.0f, 0.0f };
	m_rot ={ 0.0f, 0.0f, 0.0f };
}

//=============================================================================
// �f�t�H���g�f�X�g���N�^
//=============================================================================
CEnemy::~CEnemy()
{
	
}

//=============================================================================
// ���f���̏���������
//=============================================================================
HRESULT CEnemy::Init(void)
{

	std::thread th(Recv, m_commu_data.GetCmmuData());

	th.detach();

	return S_OK;
}

//=============================================================================
// ���f���̏I������
//=============================================================================
void CEnemy::Uninit(void)
{
	Release();
}

//=============================================================================
// ���f���̍X�V����
//=============================================================================
void CEnemy::Update(void)
{
	Move();
	Attack();
}

//=============================================================================
// ���f���̕`�揈��
//=============================================================================
void CEnemy::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice; // �f�o�C�X�̃|�C���^
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// �f�o�C�X���擾����

	//--------------------------------------
	//�v���C���[(���_)�̃}�g���b�N�X�̐ݒ�
	//--------------------------------------
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	D3DXMatrixIdentity(&m_mtx_wld);	//�}�g���b�N�X������

	//�����̐ݒ�
	D3DXMatrixRotationYawPitchRoll(	&mtxRot,
									m_rot.y,
									m_rot.x,
									m_rot.z);

	D3DXMatrixMultiply(	&m_mtx_wld,
						&m_mtx_wld,
						&mtxRot);
	//�ʒu
	D3DXMatrixTranslation(	&mtxTrans,
							m_pos.x,
							m_pos.y,
							m_pos.z);

	D3DXMatrixMultiply(	&m_mtx_wld,
						&m_mtx_wld,
						&mtxTrans);
	//�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(	D3DTS_WORLD,
							&m_mtx_wld);

}

//=============================================================================
// ���f���̐���
//=============================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CEnemy *pEnemy = NULL;
	if (pEnemy == NULL)
	{
		pEnemy = new CEnemy;
	}

	pEnemy->m_pos = pos;
	pEnemy->m_rot = rot;

	if (pEnemy != NULL)
	{
		pEnemy->Init();
	}
	return pEnemy;
}

//=============================================================================
// ���V�[�u�X���b�h
//=============================================================================
void CEnemy::Recv(CCommunicationData::COMMUNICATION_DATA *my_data)
{
	/*while (pData->bConnect == false)
	{
		CTcpClient *pTcp = CManager::GetTcpClient();
		char aRecv[MAX_COMMUDATA];

		pTcp->Recv(&aRecv[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
		if (nRecvSize < 0)
		{
			if (pTcp != NULL)
			{
				pTcp->Uninit();
				pTcp = NULL;
				break;
			}
		}
		memcpy(pData, &aRecv[0], (int)sizeof(CCommunicationData::COMMUNICATION_DATA));
	}*/
}

//=============================================================================
// �U��
//=============================================================================
void CEnemy::Attack(void)
{
	
}

//=============================================================================
// �ړ�
//=============================================================================
void CEnemy::Move(void)
{
	CCommunicationData::COMMUNICATION_DATA *pData = m_commu_data.GetCmmuData();

	m_pos = pData->Player.Pos;
	m_rot = pData->Player.Rot;
}