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
#include "gunmodel.h"
#include "model_collision.h"
#include "player.h"
#include "PresetSetEffect.h"
#include "ballistic.h"
#include <thread>

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CEnemy::CEnemy(CObject::PRIORITY Priority) : CObject(Priority)
{
	m_pos = { 0.0f, 0.0f, 0.0f };
	m_rot = { 0.0f, 0.0f, 0.0f };
	m_nLife = 0;
	m_pCollModel = nullptr;
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
	SetObjType(CObject::OBJTYPE::ENEMY);
	m_nLife = PLAYER_LIFE;

	m_model = CXanimModel::Create("data/motion.x");
	m_model->ChangeAnimation("nutral", 60.0f / 4800.0f);
	//�e���f���̐���
	m_pGunModel = CGunModel::Create({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.6f, 12.0f }, "asult_gun_inv.x");
	//�����蔻��{�b�N�X�̐���
	m_pCollModel = CModelCollision::Create({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, "player_coll.x", nullptr, true);

	std::thread th(Recv, &m_commu_data);

	th.detach();

	return S_OK;
}

//=============================================================================
// ���f���̏I������
//=============================================================================
void CEnemy::Uninit(void)
{
	Release();
	m_model->Uninit();
}

//=============================================================================
// ���f���̍X�V����
//=============================================================================
void CEnemy::Update(void)
{
	m_model->Update();
	
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
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y,
		m_rot.x,
		m_rot.z);

	D3DXMatrixMultiply(&m_mtx_wld,
		&m_mtx_wld,
		&mtxRot);
	//�ʒu
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x,
		m_pos.y,
		m_pos.z);

	D3DXMatrixMultiply(&m_mtx_wld,
		&m_mtx_wld,
		&mtxTrans);
	//�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD,
		&m_mtx_wld);

	m_model->Draw();

	//�}�g���b�N�X���擾
	D3DXMATRIX *hand = nullptr;
	hand = m_model->GetMatrix("handR");
	m_pGunModel->SetMtxParent(m_pGunModel->GetModel()->GetModel()->GetMtxPoint());
	//�e�Ɛe�q�֌W������
	m_pGunModel->GetModel()->GetModel()->SetMtxParent(hand);
	m_pGunModel->GetModel()->GetModel()->SetObjParent(true);
	m_pGunModel->GetModel()->GetModel()->SetRot({ 0.0f, D3DX_PI / 2.0f, 0.0f });
	m_pGunModel->GetModel()->GetModel()->SetPos({ 0.0f, 0.0f, 0.0f });
	m_pGunModel->GetModel()->SetCulliMode(false);

	//�e�q�֌W������
	m_pCollModel->GetModel()->SetMtxParent(&m_mtx_wld);
	m_pCollModel->GetModel()->SetObjParent(true);
}

//=============================================================================
// ���f���̐���
//=============================================================================
CEnemy *CEnemy::Create(void)
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
void CEnemy::Recv(CCommunicationData *data)
{
	int size = 1;

	while (size >= 0)
	{
		CTcpClient *pTcp = CManager::GetInstance()->GetCommunication();
		char recv[MAX_COMMU_DATA];

		CCommunicationData::COMMUNICATION_DATA *pData = data->GetCmmuData();

		size = pTcp->Recv(&recv[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
		if (size <= 0)
		{
			break;
		}
		else
		{
			memcpy(pData, &recv[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			data->SetCmmuData(*pData);
		}
	}
	CCommunicationData::COMMUNICATION_DATA *pData = data->GetCmmuData();
	pData->bConnect = false;
}

//=============================================================================
// �U��
//=============================================================================
void CEnemy::Attack(void)
{
	CCommunicationData::COMMUNICATION_DATA *pData = m_commu_data.GetCmmuData();

	//�G�������Ă�����
	if (pData->Bullet.bUse == true)
	{
		//�e�̋O���G�t�F�N�g�𐶐�
		CBallistic::Create(pData->Ballistic.BigenPos, pData->Ballistic.Size, pData->Ballistic.Rot, pData->Ballistic.EndPos,
			               pData->Ballistic.fSpeed, pData->Ballistic.sTexPas1, pData->Ballistic.sTexPas2);

		//�}�Y���t���b�V���G�t�F�N�g�̐���
		CPresetEffect::SetEffect3D(0, pData->Ballistic.BigenPos, {}, {});
		CPresetEffect::SetEffect3D(1, pData->Ballistic.BigenPos, {}, {});

		pData->Bullet.bUse = false;
	}
}

//=============================================================================
// �ړ�
//=============================================================================
void CEnemy::Move(void)
{
	CCommunicationData::COMMUNICATION_DATA *pData = m_commu_data.GetCmmuData(); 
	string now_motion;
	string commu_motion = pData->Player.aMotion[0];

	if (pData->bConnect == true)
	{
		m_pos = pData->Player.Pos;
		m_rot = pData->Player.Rot;

		now_motion = m_model->GetAnimation();
		if (now_motion != commu_motion && pData->bConnect == true)
		{
			m_model->ChangeAnimation(commu_motion, pData->Player.fMotionSpeed);
		}
	}
	else
	{
		m_pos.y = 100;
	}
}