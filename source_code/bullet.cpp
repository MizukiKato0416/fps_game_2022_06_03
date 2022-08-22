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
#include "ballistic.h"
#include "renderer.h"
#include "player.h"
#include "gun_player.h"
#include "mesh_field.h"
#include "xanimmodel.h"
#include "hierarchydata.h"
#include "enemy.h"
#include "networkmanager.h"

//================================================
//�}�N����`
//================================================
#define BULLET_MOVE_SPEED			(450.0f)						//�O���̑���
#define BULLET_SIZE_X				(200.0f)						//�O���̃T�C�Y
#define BULLET_SIZE_Y				(2.5f)							//�O���̃T�C�Y
#define BULLET_MAX_END_POS			(10000.0f)						//�O���̏I�_�ő�l
#define BULLET_DAMAGE				(20)							//�_���[�W
#define BULLET_PLAYER_COL_SIZE_Y	(90.0f)							//�����蔻��T�C�Y�����l
#define BULLET_PLAYER_COL_RADIUS	(28.0f)							//�����蔻�蔼�a
#define BULLET_RAND					(rand() % 280 + -140)			//�g�U��
#define BULLET_RAND_JUMP			(rand() % 360 + -180)			//�W�����v�����Ƃ��̊g�U��

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
	//�I�u�W�F�N�g�̎�ނ�ݒ�
	SetObjType(CObject::OBJTYPE::BULLET);

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
	bool bHitModel = false;
	m_nDamage = BULLET_DAMAGE;

	//ADS���Ă��邩�ǂ����ۑ��p�ϐ�
	bool bAds = false;
	//�W�����v���Ă��邩�ǂ����ۑ��p�ϐ�
	bool bJump = false;

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
			CPlayer *pPlayerObj = nullptr;
			pPlayerObj = (CPlayer*)object[nCnt];

			//ADS���Ă��邩�ǂ����擾
			bAds = pPlayerObj->GetAds();
			//�W�����v�擾
			bJump = pPlayerObj->GetJump();
		}
	}



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

			//��������������
			if (!bAds)
			{
				////�W�����v���Ă�����
				//if (bJump)
				//{
				//	//�g�U������ʐݒ�
				//	m_rand.x = float(BULLET_RAND_JUMP);
				//	m_rand.y = float(BULLET_RAND_JUMP);
				//	m_rand.z = float(BULLET_RAND_JUMP);
				//}
				//else
				//{//�W�����v���Ă��Ȃ�������
				//	//�g�U������ʐݒ�
				//	m_rand.x = float(BULLET_RAND);
				//	m_rand.y = float(BULLET_RAND);
				//	m_rand.z = float(BULLET_RAND);
				//}

				//�g�U������ʐݒ�
				m_rand.x = float(BULLET_RAND);
				m_rand.y = float(BULLET_RAND);
				m_rand.z = float(BULLET_RAND);

				//�������̏ꍇ�̓����_���Ɋg�U������
				posCameraR.x += cosf(rotCamera.y) * m_rand.x;
				posCameraR.z += sinf(rotCamera.y) * m_rand.z;
				posCameraR.y += m_rand.y;
			}

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
	object.clear();

	//�擪�̃|�C���^����
	object = CObject::GetObject(static_cast<int>(CObject::PRIORITY::MODEL));
	nProprty_Size = object.size();

	for (int nCnt = 0; nCnt < nProprty_Size; nCnt++)
	{
		//�I�u�W�F�N�g�̎�ނ����f����������
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
							//���f���ɓ������Ă����Ԃɂ���
							bHitModel = true;
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
	D3DXMATRIX mtx = pPlayerObj->GetGunPlayer()->GetMuzzleMtx();
	D3DXVECTOR3 gunPos = { mtx._41, mtx._42, mtx._43 };

	//���f���Ɠ������Ă���Ƃ�
	if (bHitModel)
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

	float fMeshDiffer = 0.0f;
	D3DXVECTOR3 meshHitPos = { 0.0f, 0.0f, 0.0f };

	//���b�V���t�B�[���h�̓����蔻��
	bool bCollMesh = CMeshField::Collision(meshHitPos, fMeshDiffer, posCameraV, m_endPos);
	//���b�V���t�B�[���h�ɓ���������
	if (bCollMesh)
	{
		//�����������b�V���t�B�[���h�܂ł̋��������f���̋������߂���
		if (fMeshDiffer < m_fDiffer)
		{
			//�I�_��ݒ�
			m_endPos = meshHitPos;
			bHitModel = false;
			//������ۑ�
			m_fDiffer = fMeshDiffer;
		}
		else
		{
			bCollMesh = false;
		}
	}

	//������
	object.clear();
	bool bHitEnemy = false;
	float fSaveDiffer = m_fDiffer;

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

			//�}�g���b�N�X�擾
			D3DXMATRIX enemyMtx = pEnemy->GetMtx();

			//���������ʒu�ۑ��p�ϐ�
			D3DXVECTOR3 hitPos = { 0.0f, 0.0f, 0.0f };
			D3DXVECTOR3 endPos = { 0.0f, 0.0f, 0.0f };

			// ���C�ƃJ�v�Z���̓����蔻��
			if (calcRayCapsule(posCameraV.x, posCameraV.y, posCameraV.z,
				m_rayVec.x, m_rayVec.y, m_rayVec.z,
				enemyMtx._41, enemyMtx._42, enemyMtx._43,
				enemyMtx._41, enemyMtx._42 + BULLET_PLAYER_COL_SIZE_Y, enemyMtx._43,
				BULLET_PLAYER_COL_RADIUS,
				hitPos.x, hitPos.y, hitPos.z,
				endPos.x, endPos.y, endPos.z))
			{
				D3DXVECTOR3 hitVec = hitPos - posCameraV;
				// �x�N�g���𐳋K��
				D3DXVec3Normalize(&hitVec, &hitVec);


				if ((m_rayVec.x > 0.0f && hitVec.x < 0.0f || m_rayVec.x < 0.0f && hitVec.x > 0.0f) &&
					(m_rayVec.y > 0.0f && hitVec.y < 0.0f || m_rayVec.y < 0.0f && hitVec.y > 0.0f) &&
					(m_rayVec.z > 0.0f && hitVec.z < 0.0f || m_rayVec.z < 0.0f && hitVec.z > 0.0f))
				{
					//�������Ă��Ȃ�
				}
				else
				{//�������Ă�
					// ���������ꏊ�܂ł̋������Z�o
					D3DXVECTOR3 differVec = hitPos - posCameraV;
					fDiffer = D3DXVec3Length(&differVec);

					if (fSaveDiffer > fDiffer)
					{
						// ������ۑ�
						fSaveDiffer = fDiffer;
						// ��������
						bHitEnemy = true;
					}
				}
			}
		}
	}

	//���������I�u�W�F�N�g�ɂ���ăG�t�F�N�g���o��
	if (!bCollMesh && bHitModel && !bHitEnemy)
	{//���f���ɓ���������
		//���������ʒu�ɃG�t�F�N�g���o��
		CPresetEffect::SetEffect3D(2, m_endPos, {}, {});
		CPresetEffect::SetEffect3D(3, m_endPos, {}, {});
		//�e���@�@�Ō�̈����ɉ�]����Ă�������(Y�������̂ݓK��)
		CPresetEffect::SetEffect3D(4, m_endPos, {}, D3DXVECTOR3(0.0f, D3DX_PI / 2.0f, D3DX_PI));
		CPresetEffect::SetEffect3D(4, m_endPos, {}, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	else if (bCollMesh && !bHitModel && !bHitEnemy)
	{//���b�V���t�B�[���h�ɓ���������
		//���������ʒu�ɃG�t�F�N�g���o��
		CPresetEffect::SetEffect3D(2, m_endPos, {}, {});
		CPresetEffect::SetEffect3D(3, m_endPos, {}, {});
	}

	//�e�̋O���G�t�F�N�g�𐶐�
	CBallistic::Create(gunPos, { BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f }, rotCamera, m_endPos, BULLET_MOVE_SPEED, "bullet_00.png", "bullet_01.png");

	//�ʐM�f�[�^�ɏ���˂�����
	CCommunicationData::COMMUNICATION_DATA *pData = CManager::GetInstance()->GetNetWorkmanager()->GetPlayerData()->GetCmmuData();

	if(!bCollMesh && bHitModel)
	{//���f���ɓ���������
		//���f���ɓ������Ă����Ԃɂ���
		pData->Bullet.type = CCommunicationData::HIT_TYPE::MODEL;
	}
	else if (bCollMesh && !bHitModel)
	{//���b�V���t�B�[���h�ɓ���������
		//���b�V���t�B�[���h�ɓ������Ă����Ԃɂ���
		pData->Bullet.type = CCommunicationData::HIT_TYPE::MESHFIELD;
	}
	else if (!bCollMesh && !bHitModel)
	{//���ɂ��������Ă��Ȃ�������
		//�����������Ă��Ȃ���Ԃɂ���
		pData->Bullet.type = CCommunicationData::HIT_TYPE::NONE;
	}

	// ����ݒ�
	pData->Bullet.fDiffer = m_fDiffer;
	pData->Bullet.hitPos = m_endPos;
	pData->Bullet.nDamage = m_nDamage;
	pData->Bullet.bUse = true;
	pData->Ballistic.BigenPos = gunPos;
	pData->Ballistic.Size = { BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f };
	pData->Ballistic.Rot = rotCamera;
	pData->Ballistic.EndPos = m_endPos;
	pData->Ballistic.fSpeed = BULLET_MOVE_SPEED;
	pData->Player.CamV = posCameraV;
	pData->Player.CamR = posCameraR;

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
	Uninit();
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

