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
#include "player.h"
#include "PresetSetEffect.h"
#include "ballistic.h"
#include "player.h"
#include "networkmanager.h"
#include "shadow.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
int CEnemy::m_all_count = 0;	// �G�̃J�E���^�[

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
	m_my_number = m_all_count;
	m_all_count++;
	m_pShadow = nullptr;
}

//=============================================================================
// �f�t�H���g�f�X�g���N�^
//=============================================================================
CEnemy::~CEnemy()
{
	m_all_count = 0;
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
	m_model->ChangeAnimation("neutral", 60.0f / 4800.0f);
	//�e���f���̐���
	m_pGunModel = CGunModel::Create({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.6f, 12.0f }, "asult_gun_inv.x");
	m_pGunModel->SetMtxParent(m_pGunModel->GetModel()->GetModel()->GetMtxPoint());

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

	//�e�̐ݒ�
	m_pShadow = CShadow::Create(D3DXVECTOR3(m_pos.x, 0.0f, m_pos.z), D3DXVECTOR3(m_size.x, 0.0f, m_size.z), this);

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

	//�e�̓����蔻��
	m_pShadow->Collision(m_pos, m_size.x * 20.0f);

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
// �U��
//=============================================================================
void CEnemy::Attack(void)
{
	vector<CCommunicationData> data = CManager::GetInstance()->GetNetWorkmanager()->GetEnemyData();

	//�G�������Ă�����
	if (data[m_my_number].GetCmmuData()->Bullet.bUse == true)
	{
		//�e���̃}�g���b�N�X
		D3DXMATRIX mtx = m_pGunModel->GetMuzzleMtx();
		D3DXVECTOR3 gunPos = { mtx._41, mtx._42, mtx._43 };

		//�e�̋O���G�t�F�N�g�𐶐�
		CBallistic::Create(gunPos, data[m_my_number].GetCmmuData()->Ballistic.Size, data[m_my_number].GetCmmuData()->Ballistic.Rot, data[m_my_number].GetCmmuData()->Ballistic.EndPos,
			data[m_my_number].GetCmmuData()->Ballistic.fSpeed, "bullet_00.png", "bullet_01.png");

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
	vector<CCommunicationData> data = CManager::GetInstance()->GetNetWorkmanager()->GetEnemyData();
	string now_motion;
	string commu_motion = data[m_my_number].GetCmmuData()->Player.aMotion[0];

	//����ł��Ȃ�������
	if (!data[m_my_number].GetCmmuData()->Player.bDeath)
	{
		if (data[m_my_number].GetCmmuData()->bConnect == true)
		{
			m_posOld = m_pos;
			m_rotOld = m_rot;
			m_recvPos = data[m_my_number].GetCmmuData()->Player.Pos;
			m_recvRot = data[m_my_number].GetCmmuData()->Player.Rot;

			now_motion = m_model->GetAnimation();
			m_model->ChangeSpeed(data[m_my_number].GetCmmuData()->Player.fMotionSpeed);
			if (now_motion != commu_motion && data[m_my_number].GetCmmuData()->bConnect == true)
			{
				m_model->ChangeAnimation(commu_motion, data[m_my_number].GetCmmuData()->Player.fMotionSpeed);
			}
		}
		else
		{
			m_pos = { 0.0f, 100.0f, 0.0f };
		}

		//�󂯎�����ʒu���猳�̈ʒu�܂ł̃��F�N�g�����Z�o
		D3DXVECTOR3 posVec = m_recvPos - m_posOld;
		D3DXVECTOR3 rotVec = { 0.0f, 0.0f, 0.0f };

		//���݂̌������Ƃɂ��ꂼ�������ς���ʂ��v�Z
		if (m_rotOld.y < 0.0f && -m_rotOld.y + m_recvRot.y > D3DX_PI)
		{
			rotVec.y = (-D3DX_PI - m_rotOld.y) + -(D3DX_PI - m_recvRot.y);
		}
		else if (m_rotOld.y >= D3DX_PI / 2.0f && m_rotOld.y - m_recvRot.y > D3DX_PI)
		{
			rotVec.y = (D3DX_PI - m_rotOld.y) - (-D3DX_PI - m_recvRot.y);
		}
		else
		{
			rotVec.y = (m_recvRot.y - m_rotOld.y);
		}

		//�x�N�g��������̐��Ŋ���
		posVec /= (float)SEND_COUNTER;
		rotVec /= (float)SEND_COUNTER;
		//���݈ʒu����x�N�g�����ʒu���ړ�
		m_pos += posVec;
		m_rot += rotVec;

		//�΂��傫���Ȃ�����-2�΂���
		if (m_rot.y > D3DX_PI)
		{
			m_rot.y -= D3DX_PI * 2.0f;
		}
		else if (m_rot.y < -D3DX_PI)
		{	//-�΂�菬�����Ȃ�����+2�΂���
			m_rot.y += D3DX_PI * 2.0f;
		}
	}
}