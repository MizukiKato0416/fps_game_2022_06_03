//================================================
//�e����
//Author: ��������
//================================================
#include "bullet.h"
#include "manager.h"
#include "fade.h"
#include "model.h"
#include "model_single.h"
#include "camera.h"
#include "PresetSetEffect.h"
#include "Object3D.h"
#include "renderer.h"

//================================================
//�}�N����`
//================================================
#define BULLET_MOVE_SPEED		(100.0f)		//�O���̑���
#define BULLET_SIZE_X			(200.0f)			//�O���̃T�C�Y
#define BULLET_SIZE_Y			(5.0f)			//�O���̃T�C�Y

//================================================
//�ÓI�����o�ϐ��錾
//================================================

//================================================
//�f�t�H���g�R���X�g���N�^
//================================================
CBullet::CBullet(CObject::PRIORITY Priority) :CObject(Priority)
{
	m_bigenPos = {0.0f, 0.0f, 0.0f};
	m_rayVec = { 0.0f, 0.0f, 0.0f };
	memset(m_apOrbit, NULL, sizeof(m_apOrbit[BULLET_MAX_ORBIT]));
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
	float fDifferNear = 100000.0f;
	//���������I�u�W�F�N�g�̃|�C���^
	CObject *pHitObject = nullptr;
	//�J�����̃��[�J�����W
	D3DXVECTOR3 hitPosV = { 0.0f, 0.0f, 0.0f };
	//��������ɂ����������ǂ���
	bool bHitAny = false;

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
						if (fDifferNear > fDiffer)
						{
							//������ۑ�
							fDifferNear = fDiffer;
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


	if (bHitAny)
	{
		//modelSingle�ɃL���X�g
		CModelSingle *pHitModel = (CModelSingle*)pHitObject;

		D3DXVec3Normalize(&rayVecHit, &rayVecHit);
		//���C�̃x�N�g�����Z�o���������̕��L�΂�
		rayVecHit *= fDifferNear;

		//�J�����̈ʒu����L�΂����x�N�g���𑫂��ē��������ʒu���Z�o
		D3DXVECTOR3 HitPos = hitPosV + rayVecHit;
		D3DXMATRIX hitModelMtx = pHitModel->GetModel()->GetMtx();
		D3DXVec3TransformCoord(&HitPos, &HitPos, &hitModelMtx);

		//���������ʒu�ɃG�t�F�N�g���o��
		CPresetEffect::SetEffect3D(2, HitPos, {});
		CPresetEffect::SetEffect3D(3, HitPos, {});
	}

	//�e�̋O���G�t�F�N�g�𐶐�
	m_apOrbit[0] = CObject3D::Create(posCameraV, { BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f }, { 0.0f, rotCamera.y + D3DX_PI / 2.0f, rotCamera.x + D3DX_PI / 2.0f });
	m_apOrbit[0]->BindTexture(CManager::GetTexture()->GetTexture("bullet_00.png"));
	m_apOrbit[1] = CObject3D::Create(posCameraV, { BULLET_SIZE_Y, BULLET_SIZE_X, 0.0f }, { rotCamera.x, rotCamera.y, 0.0f });
	m_apOrbit[1]->BindTexture(CManager::GetTexture()->GetTexture("bullet_01.png"));
	//�J�����O���I�t�ɂ���
	m_apOrbit[0]->SetCulling(false);
	m_apOrbit[1]->SetCulling(false);

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
	for (int nCntOrbit = 0; nCntOrbit < BULLET_MAX_ORBIT; nCntOrbit++)
	{
		//�ʒu�ƃT�C�Y���擾
		D3DXVECTOR3 pos = m_apOrbit[nCntOrbit]->GetPos();
		D3DXVECTOR3 size = m_apOrbit[nCntOrbit]->GetSize();
		//�ړ��ʂ�ݒ�
		D3DXVECTOR3 move = m_rayVec * BULLET_MOVE_SPEED;
		pos += move;
		//�ʒu��ݒ�
		m_apOrbit[nCntOrbit]->SetPos(pos, size);
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