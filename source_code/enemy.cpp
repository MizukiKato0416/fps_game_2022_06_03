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
#include "player.h"
#include <thread>

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CEnemy::CEnemy(CObject::PRIORITY Priority) : CObject(Priority)
{
	m_pos = { 0.0f, 0.0f, 0.0f };
	m_recvPos = { 0.0f, 0.0f, 0.0f };
	m_posOld = { 0.0f, 0.0f, 0.0f };
	m_rot = { 0.0f, 0.0f, 0.0f };
	m_recvRot = { 0.0f, 0.0f, 0.0f };
	m_rotOld = { 0.0f, 0.0f, 0.0f };
	m_size = { 0.0f, 0.0f, 0.0f };
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
	m_posOld = m_pos;
	m_recvPos = m_pos;

	m_rotOld = m_rot;
	m_recvRot = m_rot;

	SetObjType(CObject::OBJTYPE::ENEMY);
	m_nLife = PLAYER_LIFE;

	m_model = CXanimModel::Create("data/motion.x");
	m_model->ChangeAnimation("nutral", 60.0f / 4800.0f);
	//�e���f���̐���
	m_pGunModel = CGunModel::Create({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.6f, 12.0f }, "asult_gun_inv.x");
	m_pGunModel->SetMtxParent(m_pGunModel->GetModel()->GetModel()->GetMtxPoint());

	//�����蔻��{�b�N�X�̐���
	m_pCollModel = CModelCollision::Create({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, "player_coll.x", nullptr, true);

	//�T�C�Y���擾
	D3DXVECTOR3 modelSize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	modelSize = m_model->GetSize();

	//�T�C�Y��X��Z���ׂđ傫���ق���X��Z���ꂼ��Ɋ��蓖�Ă�
	if (modelSize.x >= modelSize.z)
	{
		m_size = D3DXVECTOR3(modelSize.x + PLAYER_SIZE, modelSize.y, modelSize.x + PLAYER_SIZE);
	}
	else
	{
		m_size = D3DXVECTOR3(modelSize.z + PLAYER_SIZE, modelSize.y, modelSize.z + PLAYER_SIZE);
	}

	//�T�C�Y�̐ݒ�
	SetSize(m_size);

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

	SetPos(m_pos);

	CPlayer::Collision(this, m_size.x / 2.0f);
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

	m_model->SetMatrix(&m_mtx_wld);
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
		CCommunicationData::COMMUNICATION_DATA *pDataBuf = new CCommunicationData::COMMUNICATION_DATA;

		size = pTcp->Recv(&recv[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
		if (size <= 0)
		{
			break;
		}
		else
		{
			memcpy(pDataBuf, &recv[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			if (pData->Player.nNumber == 0)
			{
				pData = pDataBuf;
			}
			else if (pData->Player.nNumber == pDataBuf->Player.nNumber)
			{
				pData = pDataBuf;
			}
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
		//�e���̃}�g���b�N�X
		D3DXMATRIX mtx = m_pGunModel->GetMuzzleMtx();
		D3DXVECTOR3 gunPos = { mtx._41, mtx._42, mtx._43 };

		//�e�̋O���G�t�F�N�g�𐶐�
		CBallistic::Create(gunPos, pData->Ballistic.Size, pData->Ballistic.Rot, pData->Ballistic.EndPos,
			               pData->Ballistic.fSpeed, "bullet_00.png", "bullet_01.png");

		//�}�Y���t���b�V���G�t�F�N�g�̐���
		CPresetEffect::SetEffect3D(0, gunPos, {}, {});
		CPresetEffect::SetEffect3D(1, gunPos, {}, {});
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
		m_posOld = m_pos;
		m_rotOld = m_rot;
		m_recvPos = pData->Player.Pos;
		m_recvRot = pData->Player.Rot;

		now_motion = m_model->GetAnimation();
		if (now_motion != commu_motion && pData->bConnect == true)
		{
			m_model->ChangeAnimation(commu_motion, pData->Player.fMotionSpeed);
		}
	}
	else
	{
		m_pos = { 0.0f, 100.0f, 0.0f };
	}

	//�󂯎�����ʒu���猳�̈ʒu�܂ł̃��F�N�g�����Z�o
	D3DXVECTOR3 posVec = m_recvPos - m_posOld;
	D3DXVECTOR3 rotVec = m_recvRot - m_rotOld;
	//�x�N�g��������̐��Ŋ���
	posVec /= 10.0f;
	rotVec /= 10.0f;
	//���݈ʒu����x�N�g�����ʒu���ړ�
	m_pos += posVec;
	m_rot += rotVec;
}