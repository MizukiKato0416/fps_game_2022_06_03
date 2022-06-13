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
#include "tcp_client.h"
#include "xanimmodel.h"
#include "model.h"
#include "model_single.h"
#include <thread>

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CCommunicationData CEnemy::m_commu_data[MAX_PLAYER];

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CEnemy::CEnemy(CObject::PRIORITY Priority) : CObject(Priority)
{
	for (int count_enemy = 0; count_enemy < MAX_PLAYER; count_enemy++)
	{
		m_pos[count_enemy] = { 0.0f, 0.0f, 0.0f };
		m_rot[count_enemy] = { 0.0f, 0.0f, 0.0f };
	}
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
	for (int count_enemy = 0; count_enemy < MAX_PLAYER; count_enemy++)
	{
		m_model[count_enemy] = CXanimModel::Create("data/motion.x");
		m_model[count_enemy]->ChangeAnimation(1, 60.0f / 4800.0f);
		m_pGunModel[count_enemy] = CModelSingle::Create({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, "asult_gun.x", nullptr, false);
	}

	std::thread th(Recv);

	th.detach();

	return S_OK;
}

//=============================================================================
// ���f���̏I������
//=============================================================================
void CEnemy::Uninit(void)
{
	Release();
	for (int count_enemy = 0; count_enemy < MAX_PLAYER; count_enemy++)
	{
		m_model[count_enemy]->Uninit();
	}
}

//=============================================================================
// ���f���̍X�V����
//=============================================================================
void CEnemy::Update(void)
{
	for (int count_enemy = 0; count_enemy < MAX_PLAYER; count_enemy++)
	{
		m_model[count_enemy]->Update();
	}
	
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

	for (int count_enemy = 0; count_enemy < MAX_PLAYER; count_enemy++)
	{
		//--------------------------------------
		//�v���C���[(���_)�̃}�g���b�N�X�̐ݒ�
		//--------------------------------------
		D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

		D3DXMatrixIdentity(&m_mtx_wld[count_enemy]);	//�}�g���b�N�X������

										//�����̐ݒ�
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			m_rot[count_enemy].y,
			m_rot[count_enemy].x,
			m_rot[count_enemy].z);

		D3DXMatrixMultiply(&m_mtx_wld[count_enemy],
			&m_mtx_wld[count_enemy],
			&mtxRot);
		//�ʒu
		D3DXMatrixTranslation(&mtxTrans,
			m_pos[count_enemy].x,
			m_pos[count_enemy].y,
			m_pos[count_enemy].z);

		D3DXMatrixMultiply(&m_mtx_wld[count_enemy],
			&m_mtx_wld[count_enemy],
			&mtxTrans);
		//�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD,
			&m_mtx_wld[count_enemy]);
		m_model[count_enemy]->Draw();

		//�}�g���b�N�X���擾
		D3DXMATRIX *hand = nullptr;
		hand = m_model[count_enemy]->GetMatrix("handL");

		//�e�Ɛe�q�֌W������
		m_pGunModel[count_enemy]->GetModel()->SetMtxParent(hand);
		m_pGunModel[count_enemy]->GetModel()->SetObjParent(true);
	}
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

	if (pEnemy != NULL)
	{
		pEnemy->Init();
	}
	return pEnemy;
}

//=============================================================================
// ���V�[�u�X���b�h
//=============================================================================
void CEnemy::Recv(void)
{
	int size = 1;

	while (size > 0)
	{
		CTcpClient *pTcp = CManager::GetInstance()->GetCommunication();
		char recv[MAX_COMMU_DATA];

		for (int count_enemy = 0; count_enemy < MAX_PLAYER; count_enemy++)
		{
			CCommunicationData::COMMUNICATION_DATA *pData = m_commu_data[count_enemy].GetCmmuData();

			size = pTcp->Recv(&recv[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			memcpy(pData, &recv[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
		}
	}
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
	for (int count_enemy = 0; count_enemy < MAX_PLAYER; count_enemy++)
	{
		CCommunicationData::COMMUNICATION_DATA *pData = m_commu_data[count_enemy].GetCmmuData();
		int now_motion = -1;

		m_pos[count_enemy] = pData->Player.Pos;
		m_rot[count_enemy] = pData->Player.Rot;
		
		now_motion = m_model[count_enemy]->GetAnimation();
		if (now_motion != pData->Player.nMotion && pData->bConnect == true)
		{
			m_model[count_enemy]->ChangeAnimation(pData->Player.nMotion, pData->Player.fMotionSpeed);
		}
	}
}