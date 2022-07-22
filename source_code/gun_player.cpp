//=============================================================================
//
// �v���C���[�������Ă���e���� [modeltester.cpp]
// Author : ��������
//
//=============================================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "gun_player.h"
#include "manager.h"
#include "renderer.h"
#include "model_single.h"
#include "model.h"
#include "communicationdata.h"
#include "input_mouse.h"
#include "player.h"
#include "manager.h"
#include "PresetSetEffect.h"
#include "networkmanager.h"
#include "xanimmodel.h"


//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CGunPlayer::CGunPlayer(CObject::PRIORITY Priority) : CObject(Priority)
{

}

//=============================================================================
// �f�t�H���g�f�X�g���N�^
//=============================================================================
CGunPlayer::~CGunPlayer()
{

}

//=============================================================================
// ���f���̏���������
//=============================================================================
HRESULT CGunPlayer::Init(void)
{
	m_model = CModelSingle::Create(m_pos, {0.0f, 2.0f, 0.0f}, m_type, NULL, false);

	return S_OK;
}

//=============================================================================
// ���f���̏I������
//=============================================================================
void CGunPlayer::Uninit(void)
{
	Release();
}

//=============================================================================
// ���f���̍X�V����
//=============================================================================
void CGunPlayer::Update(void)
{
	
}


//=============================================================================
// ���f���̕`�揈��
//=============================================================================
void CGunPlayer::Draw(void)
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
	pDevice->SetTransform(D3DTS_WORLD,
						  &m_mtx_muzzle_world);

	m_model->GetModel()->SetMtx();
	*m_mtxParent = m_model->GetModel()->GetMtx();
	mtx_parent = *m_mtxParent;

	D3DXMatrixMultiply(&m_mtx_muzzle_world,
						&m_mtx_muzzle_world,
						&mtx_parent);

	//�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(	D3DTS_WORLD,
							&m_mtx_muzzle_world);
}

//=============================================================================
// ���f���̐���
//=============================================================================
CGunPlayer *CGunPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 muzzle_pos, string type)
{
	CGunPlayer *gun = NULL;
	if (gun == NULL)
	{
		gun = new CGunPlayer;
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

//=============================================================================
//�ˌ�����
//=============================================================================
void CGunPlayer::Shot(void)
{
	////�}�E�X�擾����
	//CInputMouse *pInputMouse;
	//pInputMouse = CManager::GetInstance()->GetInputMouse();

	//// �ʐM�f�[�^�擾����
	//CCommunicationData::COMMUNICATION_DATA *pData = CManager::GetInstance()->GetNetWorkmanager()->GetPlayerData()->GetCmmuData();

	////�I�u�W�F�N�g��������|�C���^
	//vector<CObject*> object;

	////�擪�̃|�C���^����
	//object = CObject::GetObject(static_cast<int>(CObject::PRIORITY::PLAYER));
	//int nProprty_Size = object.size();

	//for (int nCnt = 0; nCnt < nProprty_Size; nCnt++)
	//{
	//	//�I�u�W�F�N�g�̎�ނ��v���C���[��������
	//	if (object[nCnt]->GetObjType() == CObject::OBJTYPE::PLAYER)
	//	{
	//		//�v���C���[�ɃL���X�g
	//		CPlayer *pPlayerObj = nullptr;
	//		pPlayerObj = (CPlayer*)object[nCnt];

	//		if (pInputMouse->GetPress(CInputMouse::MOUSE_TYPE::MOUSE_TYPE_LEFT) == true)
	//		{
	//			//�ړ��ʎ擾
	//			D3DXVECTOR3 move = pPlayerObj->GetMove();
	//			//���f���擾
	//			CXanimModel *pAnimodel = pPlayerObj->GetAnimModel();

	//			if (move.x != 0.0f || move.z != 0.0f)
	//			{
	//				//���A�j���[�V�����łȂ�������
	//				if (pAnimodel->GetAnimation() != "fireneutral")
	//				{
	//					//�����[�V�����ɂ���
	//					pPlayerObj->SetAnimSpeed((20.0f * 3.0f) / 4800.0f);
	//					pAnimodel->ChangeAnimation("fireneutral", pPlayerObj->GetAnimSpeed());
	//					memset(pData->Player.aMotion[0], NULL, sizeof(pData->Player.aMotion[0]));
	//					memcpy(pData->Player.aMotion[0], pAnimodel->GetAnimation().c_str(), pAnimodel->GetAnimation().size());
	//				}
	//			}
	//			else
	//			{
	//				//���A�j���[�V�����łȂ�������
	//				if (pAnimodel->GetAnimation() != "fireneutral")
	//				{
	//					//�����[�V�����ɂ���
	//					pPlayerObj->SetAnimSpeed((20.0f * 3.0f) / 4800.0f);
	//					pAnimodel->ChangeAnimation("fireneutral", pPlayerObj->GetAnimSpeed());
	//					memset(pData->Player.aMotion[0], NULL, sizeof(pData->Player.aMotion[0]));
	//					memcpy(pData->Player.aMotion[0], pAnimodel->GetAnimation().c_str(), pAnimodel->GetAnimation().size());
	//				}
	//			}

	//			//�J�E���^�[�����Z
	//			m_nCounter--;

	//			//�����Ă����ԂȂ�
	//			if (m_bShot == true)
	//			{
	//				D3DXVECTOR3 pos = { 0.0f, 0.0f,0.0f };
	//				//ADS���Ă��Ȃ�������
	//				if (!m_bAds)
	//				{
	//					m_pGunPlayer->GetModel()->GetModel()->SetMtx();
	//					//�I�t�Z�b�g�ʒu�ݒ�
	//					pos = { m_pGunPlayer->GetMuzzleMtx()._41, m_pGunPlayer->GetMuzzleMtx()._42, m_pGunPlayer->GetMuzzleMtx()._43 };
	//				}
	//				else
	//				{//ADS������
	//					m_pGunPlayerAds->GetModel()->GetModel()->SetMtx();
	//					//�I�t�Z�b�g�ʒu�ݒ�
	//					pos = { m_pGunPlayerAds->GetMuzzleMtx()._41, m_pGunPlayerAds->GetMuzzleMtx()._42, m_pGunPlayerAds->GetMuzzleMtx()._43 };
	//				}


	//				//ADS��ԂȂ�
	//				if (m_bAds)
	//				{
	//					//�}�Y���t���b�V���G�t�F�N�g�̐���
	//					CPresetEffect::SetEffect3D(7, pos, {}, {});
	//					CPresetEffect::SetEffect3D(8, pos, {}, {});
	//				}
	//				else
	//				{//ADS��ԂłȂ��Ȃ�
	//				 //�}�Y���t���b�V���G�t�F�N�g�̐���
	//					CPresetEffect::SetEffect3D(0, pos, {}, {});
	//					CPresetEffect::SetEffect3D(1, pos, {}, {});
	//				}

	//				CBullet *pBullet;	// �e�̃|�C���^
	//									//�e�̐���
	//				pBullet = CBullet::Create();

	//				//camera�̃|�C���^�z��1�Ԗڂ̃A�h���X�擾
	//				CCamera **pCameraAddress = CManager::GetInstance()->GetCamera();

	//				for (int nCntCamera = 0; nCntCamera < MAX_MAIN_CAMERA; nCntCamera++, pCameraAddress++)
	//				{
	//					//camera�̎擾
	//					CCamera *pCamera = &**pCameraAddress;
	//					if (pCamera != nullptr)
	//					{
	//						//������ݒ�
	//						D3DXVECTOR3 rotV = pCamera->GetRotV();
	//						rotV.x -= 0.02f;
	//						rotV.y -= ((rand() % 30 + -15) / 1000.0f);
	//						pCamera->SetRotV(rotV);
	//					}
	//				}

	//				//�����Ă��Ȃ���Ԃɂ���
	//				m_bShot = false;
	//			}

	//			//0��菬�����Ȃ�����
	//			if (m_nCounter < 0)
	//			{
	//				//����̒l�ɂ���
	//				m_nCounter = PLAYER_SHOT_COUNTER;

	//				//�����Ă��Ԃɂ���
	//				m_bShot = true;
	//			}
	//		}
	//		else
	//		{
	//			//�����Ă��Ȃ���Ԃɂ���
	//			m_bShot = false;
	//			//�e���g���ĂȂ�
	//			pData->Bullet.bUse = false;
	//			pData->Bullet.fDiffer = 0.0f;
	//			pData->Bullet.hitPos = { 0.0f, 0.0f, 0.0f };
	//			pData->Bullet.nDamage = 0;
	//			pData->Bullet.type = CCommunicationData::HIT_TYPE::NONE;
	//			pData->Ballistic.BigenPos = { 0.0f, 0.0f, 0.0f };
	//			pData->Ballistic.EndPos = { 0.0f, 0.0f, 0.0f };
	//			pData->Ballistic.fSpeed = 0.0f;
	//			pData->Ballistic.Rot = { 0.0f, 0.0f, 0.0f };
	//			pData->Ballistic.Size = { 0.0f, 0.0f, 0.0f };

	//			//���A�j���[�V������������
	//			if (m_pAnimModel->GetAnimation() == "fireneutral")
	//			{
	//				//�j���[�g�������[�V�����ɂ���
	//				m_fAnimSpeed = (20.0f * 3.0f) / 4800.0f;
	//				m_pAnimModel->ChangeAnimation("neutral", m_fAnimSpeed);
	//				memset(pData->Player.aMotion[0], NULL, sizeof(pData->Player.aMotion[0]));
	//				memcpy(pData->Player.aMotion[0], m_pAnimModel->GetAnimation().c_str(), m_pAnimModel->GetAnimation().size());
	//			}

	//			//����̒l��菬����������
	//			if (m_nCounter > 0 && m_nCounter <= PLAYER_SHOT_COUNTER)
	//			{
	//				//�J�E���^�[�����Z
	//				m_nCounter++;

	//				//����̒l���傫���Ȃ�����
	//				if (m_nCounter > PLAYER_SHOT_COUNTER)
	//				{
	//					//0�ɂ���
	//					m_nCounter = 0;
	//				}
	//			}
	//		}
	//	}
	//}
}
