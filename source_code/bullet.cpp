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

//================================================
//�ÓI�����o�ϐ��錾
//================================================

//================================================
//�f�t�H���g�R���X�g���N�^
//================================================
CBullet::CBullet(CObject::PRIORITY Priority) :CObject(Priority)
{
	
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
	// ���_�錾
	const D3DVERTEXELEMENT9 vertexDecl[] =
	{
		{ 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
		{ 0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0 },
		D3DDECL_END()
	};

	//�J�����̈ʒu�ۑ��p
	D3DXVECTOR3 posCameraV = { 0.0f, 0.0f, 0.0f };
	D3DXVECTOR3 posCameraR = { 0.0f, 0.0f, 0.0f };
	//���C���΂�����
	D3DXVECTOR3 rayVec = { 0.0f, 0.0f, 0.0f };
	//�J�����̌����ۑ��p
	D3DXVECTOR3 rotCamera = { 0.0f, 0.0f, 0.0f };
	//���C�̓����蔻�茋�ʕۑ��p
	BOOL bHit = false;
	//���������ꏊ�܂ł̋����ۑ��p
	float fDiffer = 0.0f;

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

			//���C���΂��������Z�o
			rayVec = posCameraR - posCameraV;
			//�x�N�g���𐳋K��
			D3DXVec3Normalize(&rayVec, &rayVec);
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

			D3DXMATRIX modelInvMtx;
			D3DXMatrixIdentity(&modelInvMtx);
			D3DXMatrixInverse(&modelInvMtx, NULL, &pModel->GetModel()->GetMtx());

			D3DXVECTOR3 posV = posCameraV;

			D3DXVECTOR3 endPos = posV + rayVec;
			D3DXVec3TransformCoord(&posV, &posV, &modelInvMtx);
			D3DXVec3TransformCoord(&endPos, &endPos, &modelInvMtx);

			D3DXVECTOR3 vec = endPos - posV;

			//���C�ƃ��b�V���̓����蔻��
			if (D3DXIntersect(pModel->GetModel()->GetMesh(), &posV, &vec, &bHit, NULL, NULL, NULL, &fDiffer, NULL, NULL) == D3D_OK)
			{
				//���������Ƃ�
				if (bHit)
				{
					D3DXVec3Normalize(&vec, &vec);
					//���C�̃x�N�g�����Z�o���������̕��L�΂�
					vec *= fDiffer;

					//�J�����̈ʒu����L�΂����x�N�g���𑫂��ē��������ʒu���Z�o
					D3DXVECTOR3 HitPos = posV + vec;
					D3DXVec3TransformCoord(&HitPos, &HitPos, &pModel->GetModel()->GetMtx());

					//���������ʒu�ɃG�t�F�N�g���o��
					CPresetEffect::SetEffect3D(2, HitPos, {});
					CPresetEffect::SetEffect3D(3, HitPos, {});
				}
			}
		}
	}

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