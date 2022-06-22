//================================================
//�e����
//Author: ��������
//================================================
#include "bullet.h"
#include "manager.h"
#include "fade.h"
#include "model.h"
#include "model_single.h"
#include "model_collision.h"
#include "camera.h"
#include "PresetSetEffect.h"
#include "Object3D.h"
#include "renderer.h"
#include "player.h"
#include "gunmodel.h"
#include "mesh_field.h"
#include "xanimmodel.h"
#include "hierarchydata.h"
#include "enemy.h"

//================================================
//�}�N����`
//================================================
#define BULLET_MOVE_SPEED		(350.0f)		//�O���̑���
#define BULLET_SIZE_X			(300.0f)		//�O���̃T�C�Y
#define BULLET_SIZE_Y			(3.0f)			//�O���̃T�C�Y
#define BULLET_MAX_END_POS		(10000.0f)		//�O���̏I�_�ő�l
#define BULLET_DAMAGE			(20)			//�_���[�W

//================================================
//�ÓI�����o�ϐ��錾
//================================================

//================================================
//�f�t�H���g�R���X�g���N�^
//================================================
CBullet::CBullet(CObject::PRIORITY Priority) :CObject(Priority)
{
	m_bigenPos = { 0.0f, 0.0f, 0.0f };
	m_endPos = {0.0f, 0.0f, 0.0f};
	m_rayVec = { 0.0f, 0.0f, 0.0f };
	memset(m_apOrbit, NULL, sizeof(m_apOrbit[BULLET_MAX_ORBIT]));
	m_fDiffer = 0.0f;
	m_nPlayer = 0;
	m_nDamage = 0;
}

//================================================
//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
//================================================


//================================================
//�f�X�g���N�^
//================================================
CBullet::~CBullet()
{

}

//================================================
//����������
//================================================
HRESULT CBullet::Init(void)
{
	//�J�����̈ʒu�ۑ��p
	D3DXVECTOR3 posCameraV = { 0.0f, 0.0f, 0.0f };
	D3DXVECTOR3 posCameraR = { 0.0f, 0.0f, 0.0f };
	//�J�����̌����ۑ�
	D3DXVECTOR3 rotCamera = { 0.0f, 0.0f, 0.0f };
	//���C���΂�����
	D3DXVECTOR3 rayVecHit = { 0.0f, 0.0f, 0.0f };
	//���C�̓����蔻�茋�ʕۑ��p
	BOOL bHit = false;
	//���������ꏊ�܂ł̋����ۑ��p
	float fDiffer = 0.0f;
	//��ԃv���C���[�ɋ߂����f���̓�����������
	m_fDiffer = 100000.0f;
	//���������I�u�W�F�N�g�̃|�C���^
	CObject *pHitObject = nullptr;
	//�J�����̃��[�J�����W
	D3DXVECTOR3 hitPosV = { 0.0f, 0.0f, 0.0f };
	//���f���ɂ����������ǂ���
	bool bHitAny = false;
	//�G�ɂ����������ǂ���
	bool bHitEnemy = false;

	//camera�̃|�C���^�z��1�Ԗڂ̃A�h���X�擾
	CCamera **pCameraAddress = CManager::GetInstance()->GetCamera();

	for (int nCntCamera = 0; nCntCamera < MAX_MAIN_CAMERA; nCntCamera++, pCameraAddress++)
	{
		//camera�̎擾
		CCamera *pCamera = &**pCameraAddress;
		if (pCamera != nullptr)
		{
			//�J�����̈ʒu�擾
			posCameraV = pCamera->GetPosV();
			posCameraR = pCamera->GetPosR();
			//�J�����̌����擾
			rotCamera = pCamera->GetRotV();

			//���C���΂��������Z�o
			m_rayVec = posCameraR - posCameraV;
			//�x�N�g���𐳋K��
			D3DXVec3Normalize(&m_rayVec, &m_rayVec);
			//�n�_��ݒ�
			m_bigenPos = posCameraV;
			//�I�_��ݒ�
			m_endPos = posCameraV + m_rayVec * BULLET_MAX_END_POS;
		}
	}

	//�I�u�W�F�N�g��������|�C���^
	vector<CObject*> object;

	//�擪�̃|�C���^����
	object = CObject::GetObject(static_cast<int>(CObject::PRIORITY::MODEL));
	int nProprty_Size = object.size();

	for (int nCnt = 0; nCnt < nProprty_Size; nCnt++)
	{
		//�I�u�W�F�N�g�̎�ނ����f����������
		if (object[nCnt]->GetObjType() == CObject::OBJTYPE::MODEL || object[nCnt]->GetObjType() == CObject::OBJTYPE::MODEL_COLLISION)
		{
			if (object[nCnt]->GetObjType() == CObject::OBJTYPE::MODEL)
			{
				//modelSingle�ɃL���X�g
				CModelSingle *pModel = (CModelSingle*)object[nCnt];

				if (pModel->GetColl() == true)
				{
					D3DXMATRIX modelInvMtx;
					D3DXMATRIX modelMtx = pModel->GetModel()->GetMtx();
					D3DXMatrixIdentity(&modelInvMtx);
					D3DXMatrixInverse(&modelInvMtx, NULL, &modelMtx);

					D3DXVECTOR3 posV = posCameraV;

					D3DXVECTOR3 endPos = posV + m_rayVec;
					D3DXVec3TransformCoord(&posV, &posV, &modelInvMtx);
					D3DXVec3TransformCoord(&endPos, &endPos, &modelInvMtx);

					D3DXVECTOR3 vec = endPos - posV;

					//���C�ƃ��b�V���̓����蔻��
					if (D3DXIntersect(pModel->GetModel()->GetMesh(), &posV, &vec, &bHit, NULL, NULL, NULL, &fDiffer, NULL, NULL) == D3D_OK)
					{
						//���������Ƃ�
						if (bHit)
						{
							if (m_fDiffer > fDiffer)
							{
								//������ۑ�
								m_fDiffer = fDiffer;
								//���������I�u�W�F�N�g��ۑ�
								pHitObject = object[nCnt];
								//���C�̕�����ۑ�
								rayVecHit = vec;
								//�J�����̃��[�J�����W��ۑ�
								hitPosV = posV;
								bHitAny = true;
							}
						}
					}
				}
			}
		}
	}

	//�v���C���[�̃|�C���^
	CPlayer *pPlayerObj = nullptr;

	//�I�u�W�F�N�g��������|�C���^
	object.clear();

	//�擪�̃|�C���^����
	object = CObject::GetObject(static_cast<int>(CObject::PRIORITY::PLAYER));
	nProprty_Size = object.size();

	for (int nCnt = 0; nCnt < nProprty_Size; nCnt++)
	{
		//�I�u�W�F�N�g�̎�ނ��v���C���[��������
		if (object[nCnt]->GetObjType() == CObject::OBJTYPE::PLAYER)
		{
			//�v���C���[�ɃL���X�g
			pPlayerObj = nullptr;
			pPlayerObj = (CPlayer*)object[nCnt];
		}
	}
	//�e���̃}�g���b�N�X
	D3DXMATRIX mtx = pPlayerObj->GetGunModel()->GetMuzzleMtx();
	D3DXVECTOR3 gunPos = { mtx._41, mtx._42, mtx._43 };


	object.clear();

	//�擪�̃|�C���^����
	object = CObject::GetObject(static_cast<int>(CObject::PRIORITY::ENEMY));
	nProprty_Size = object.size();

	for (int nCnt = 0; nCnt < nProprty_Size; nCnt++)
	{
		//�I�u�W�F�N�g�̎�ނ��G��������
		if (object[nCnt]->GetObjType() == CObject::OBJTYPE::ENEMY)
		{
			//�G�ɃL���X�g
			CEnemy *pEnemy = (CEnemy*)object[nCnt];

			//���f���擾
			CModelCollision *pModel = pEnemy->GetModelCollision();

			if (pModel->GetColl() == true)
			{
				D3DXMATRIX modelInvMtx;
				D3DXMATRIX modelMtx = pModel->GetModel()->GetMtx();
				D3DXMatrixIdentity(&modelInvMtx);
				D3DXMatrixInverse(&modelInvMtx, NULL, &modelMtx);

				D3DXVECTOR3 posV = posCameraV;

				D3DXVECTOR3 endPos = posV + m_rayVec;
				D3DXVec3TransformCoord(&posV, &posV, &modelInvMtx);
				D3DXVec3TransformCoord(&endPos, &endPos, &modelInvMtx);

				D3DXVECTOR3 vec = endPos - posV;

				//���C�ƃ��b�V���̓����蔻��
				if (D3DXIntersect(pModel->GetModel()->GetMesh(), &posV, &vec, &bHit, NULL, NULL, NULL, &fDiffer, NULL, NULL) == D3D_OK)
				{
					//���������Ƃ�
					if (bHit)
					{
						if (m_fDiffer > fDiffer)
						{
							//������ۑ�
							m_fDiffer = fDiffer;
							//���������I�u�W�F�N�g��ۑ�
							pHitObject = pModel;
							//���C�̕�����ۑ�
							rayVecHit = vec;
							//�J�����̃��[�J�����W��ۑ�
							hitPosV = posV;
							bHitAny = false;
							bHitEnemy = true;

							//�G�̔ԍ��ۑ�
							m_nPlayer = pEnemy->GetPlayerNumber();
						}
					}
				}
			}
		}
	}





	//���f���Ɠ������Ă��ēG�ɓ������Ă��Ȃ��Ƃ�
	if (bHitAny)
	{
		//modelSingle�ɃL���X�g
		CModelSingle *pHitModel = (CModelSingle*)pHitObject;

		D3DXVec3Normalize(&rayVecHit, &rayVecHit);
		//���C�̃x�N�g�����Z�o���������̕��L�΂�
		rayVecHit *= m_fDiffer;

		//�J�����̈ʒu����L�΂����x�N�g���𑫂��ē��������ʒu���Z�o
		D3DXVECTOR3 HitPos = hitPosV + rayVecHit;
		D3DXMATRIX hitModelMtx = pHitModel->GetModel()->GetMtx();
		D3DXVec3TransformCoord(&HitPos, &HitPos, &hitModelMtx);

		//�I�_��ݒ�
		m_endPos = HitPos;
	}

	//�G�ɓ���������
	if (bHitEnemy)
	{
		//modelSingle�ɃL���X�g
		CModelCollision *pHitModel = (CModelCollision*)pHitObject;

		D3DXVec3Normalize(&rayVecHit, &rayVecHit);
		//���C�̃x�N�g�����Z�o���������̕��L�΂�
		rayVecHit *= m_fDiffer;

		//�J�����̈ʒu����L�΂����x�N�g���𑫂��ē��������ʒu���Z�o
		D3DXVECTOR3 HitPos = hitPosV + rayVecHit;
		D3DXMATRIX hitModelMtx = pHitModel->GetModel()->GetMtx();
		D3DXVec3TransformCoord(&HitPos, &HitPos, &hitModelMtx);

		//�_���[�W��ݒ�
		m_nDamage = BULLET_DAMAGE;

		//�I�_��ݒ�
		m_endPos = HitPos;
	}

	float fMeshDiffer = 0.0f;
	D3DXVECTOR3 meshHitPos = { 0.0f, 0.0f, 0.0f };
	bool bCollMesh = CMeshField::Collision(meshHitPos, fMeshDiffer, posCameraV, m_endPos);
	//���b�V���t�B�[���h�ɓ���������
	if (bCollMesh)
	{
		//�����������b�V���t�B�[���h�܂ł̋��������f���̋�����艓���Ƃ�
		if (fMeshDiffer > m_fDiffer)
		{
			//���f���̓��������ʒu�ɃG�t�F�N�g���o��
			CPresetEffect::SetEffect3D(2, m_endPos, {}, {});
			CPresetEffect::SetEffect3D(3, m_endPos, {}, {});
			//�e���@�@�Ō�̈����ɉ�]����Ă�������(Y�������̂ݓK��)
			CPresetEffect::SetEffect3D(4, m_endPos, {}, D3DXVECTOR3(0.0f, D3DX_PI / 2.0f, D3DX_PI));
			CPresetEffect::SetEffect3D(4, m_endPos, {}, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
		else
		{
			//�I�_��ݒ�
			m_endPos = meshHitPos;
			//���b�V���t�B�[���h�̓��������ʒu�ɃG�t�F�N�g���o��
			CPresetEffect::SetEffect3D(2, meshHitPos, {}, {});
			CPresetEffect::SetEffect3D(3, meshHitPos, {}, {});
		}
	}
	else if(!bCollMesh && bHitAny)
	{//���f���ɓ���������
		//���f���̓��������ʒu�ɃG�t�F�N�g���o��
		CPresetEffect::SetEffect3D(2, m_endPos, {}, {});
		CPresetEffect::SetEffect3D(3, m_endPos, {}, {});
		//�e���@�@�Ō�̈����ɉ�]����Ă�������(Y�������̂ݓK��)
		CPresetEffect::SetEffect3D(4, m_endPos, {}, D3DXVECTOR3(0.0f, D3DX_PI / 2.0f, D3DX_PI));
		CPresetEffect::SetEffect3D(4, m_endPos, {}, D3DXVECTOR3(0.0f, 0.0f , 0.0f));
	}
	else if (!bCollMesh && bHitEnemy)
	{//�G�ɓ���������
		//���f���̓��������ʒu�ɃG�t�F�N�g���o��
		CPresetEffect::SetEffect3D(2, m_endPos, {}, {});
		CPresetEffect::SetEffect3D(3, m_endPos, {}, {});
	}


	//�e�̋O���G�t�F�N�g�𐶐�
	m_apOrbit[0] = CObject3D::Create(gunPos, { BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f }, { 0.0f, rotCamera.y + D3DX_PI / 2.0f, rotCamera.x + D3DX_PI / 2.0f });
	m_apOrbit[0]->BindTexture(CManager::GetTexture()->GetTexture("bullet_00.png"));
	m_apOrbit[0]->SetOriginType(CObject3D::ORIGIN_TYPE::LEFT);
	m_apOrbit[0]->SetPos(gunPos, { BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f });

	m_apOrbit[1] = CObject3D::Create(gunPos, { BULLET_SIZE_Y, BULLET_SIZE_X, 0.0f }, { rotCamera.x, rotCamera.y, 0.0f });
	m_apOrbit[1]->BindTexture(CManager::GetTexture()->GetTexture("bullet_01.png"));
	m_apOrbit[1]->SetOriginType(CObject3D::ORIGIN_TYPE::LOWER);
	m_apOrbit[1]->SetPos(gunPos, { BULLET_SIZE_Y, BULLET_SIZE_X, 0.0f });

	//�J�����O���I�t�ɂ���
	m_apOrbit[0]->SetCulling(false);
	m_apOrbit[1]->SetCulling(false);
	//���C�e�B���O�I�t�ɂ���
	m_apOrbit[0]->SetLighting(false);
	m_apOrbit[1]->SetLighting(false);
	//���C�e�B���O�I�t�ɂ���
	m_apOrbit[0]->SetAlphaTest(true);
	m_apOrbit[1]->SetAlphaTest(true);

	return S_OK;
}

//================================================
//�I������
//================================================
void CBullet::Uninit(void)
{	
	//�I�u�W�F�N�g�̔j��
	Release();
}

//================================================
//�X�V����
//================================================
void CBullet::Update(void)
{
	//�v���C���[�̃|�C���^
	CPlayer *pPlayerObj = nullptr;

	//�I�u�W�F�N�g��������|�C���^
	vector<CObject*> object;

	//�擪�̃|�C���^����
	object = CObject::GetObject(static_cast<int>(CObject::PRIORITY::PLAYER));
	int nProprty_Size = object.size();

	for (int nCnt = 0; nCnt < nProprty_Size; nCnt++)
	{
		//�I�u�W�F�N�g�̎�ނ��v���C���[��������
		if (object[nCnt]->GetObjType() == CObject::OBJTYPE::PLAYER)
		{
			//�v���C���[�ɃL���X�g
			pPlayerObj = nullptr;
			pPlayerObj = (CPlayer*)object[nCnt];
		}
	}


	for (int nCntOrbit = 0; nCntOrbit < BULLET_MAX_ORBIT; nCntOrbit++)
	{
		if (m_apOrbit[nCntOrbit] != nullptr)
		{
			//�ʒu�ƃT�C�Y���擾
			D3DXVECTOR3 pos = m_apOrbit[nCntOrbit]->GetPos();

			//�ړ������ʒu����I�_�܂ł̃x�N�g�����Z�o
			D3DXVECTOR3 differVec = m_endPos - pos;
			float fDiffer = D3DXVec3Length(&differVec);

			if (fDiffer < BULLET_SIZE_X)
			{
				//����
				m_apOrbit[nCntOrbit]->Uninit();
				m_apOrbit[nCntOrbit] = nullptr;
				break;
			}

			D3DXVECTOR3 size = m_apOrbit[nCntOrbit]->GetSize();

			//�e���̃}�g���b�N�X
			D3DXMATRIX mtx = pPlayerObj->GetGunModel()->GetMuzzleMtx();
			D3DXVECTOR3 gunPos = { mtx._41, mtx._42, mtx._43 };
			//��΂�������ݒ�
			D3DXVECTOR3 vec = m_endPos - gunPos;
			//���K��
			D3DXVec3Normalize(&vec, &vec);
			//�ړ��ʂ�ݒ�
			D3DXVECTOR3 move = vec * BULLET_MOVE_SPEED;
			pos += move;

			//�ړ������ʒu����I�_�܂ł̃x�N�g�����Z�o
			differVec = m_endPos - pos;
			fDiffer = D3DXVec3Length(&differVec);

			if (fDiffer < BULLET_SIZE_X)
			{
				//����
				m_apOrbit[nCntOrbit]->Uninit();
				m_apOrbit[nCntOrbit] = nullptr;
			}
			else
			{
				//�ʒu��ݒ�
				m_apOrbit[nCntOrbit]->SetPos(pos, size);
			}
		}
	}
}

//================================================
//�`�揈��
//================================================
void CBullet::Draw(void)
{
	
}

//================================================
//��������
//================================================
CBullet *CBullet::Create(void)
{
	//�C���X�^���X�̐���
	CBullet *pBullet = nullptr;
	if (pBullet == nullptr)
	{
		pBullet = new CBullet;
		if (pBullet != nullptr)
		{
			pBullet->Init();
		}
	}
	return pBullet;
}