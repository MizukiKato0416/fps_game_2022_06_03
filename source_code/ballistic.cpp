//================================================
//�e����
//Author: ��������
//================================================
#include "ballistic.h"
#include "manager.h"
#include "Object3D.h"

//================================================
//�}�N����`
//================================================

//================================================
//�ÓI�����o�ϐ��錾
//================================================

//================================================
//�f�t�H���g�R���X�g���N�^
//================================================
CBallistic::CBallistic(CObject::PRIORITY Priority) :CObject(Priority)
{
	m_bigenPos = { 0.0f, 0.0f, 0.0f };
	m_endPos = { 0.0f, 0.0f, 0.0f };
	m_size = { 0.0f, 0.0f, 0.0f };
	m_rot = {0.0f, 0.0f, 0.0f};
	m_fSpeed = 0.0f;
	m_sTexPas1.clear();
	m_sTexPas2.clear();
	memset(m_apOrbit, NULL, sizeof(m_apOrbit[BALLISTIC_MAX_ORBIT]));
}

//================================================
//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
//================================================


//================================================
//�f�X�g���N�^
//================================================
CBallistic::~CBallistic()
{

}

//================================================
//����������
//================================================
HRESULT CBallistic::Init(void)
{
	//�I�u�W�F�N�g�̎�ނ�ݒ�
	SetObjType(CObject::OBJTYPE::BALLISTIC);

	//�e�̋O���G�t�F�N�g�𐶐�
	m_apOrbit[0] = CObject3D::Create(m_bigenPos, { m_size.x, m_size.y, 0.0f }, { 0.0f, m_rot.y + D3DX_PI / 2.0f, m_rot.x + D3DX_PI / 2.0f });
	m_apOrbit[0]->BindTexture(CManager::GetTexture()->GetTexture(m_sTexPas1));
	m_apOrbit[0]->SetOriginType(CObject3D::ORIGIN_TYPE::LEFT);
	m_apOrbit[0]->SetPos(m_bigenPos, { m_size.x, m_size.y, 0.0f });

	m_apOrbit[1] = CObject3D::Create(m_bigenPos, { m_size.y, m_size.x, 0.0f }, { m_rot.x, m_rot.y, 0.0f });
	m_apOrbit[1]->BindTexture(CManager::GetTexture()->GetTexture(m_sTexPas2));
	m_apOrbit[1]->SetOriginType(CObject3D::ORIGIN_TYPE::LOWER);
	m_apOrbit[1]->SetPos(m_bigenPos, { m_size.y, m_size.x, 0.0f });

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
void CBallistic::Uninit(void)
{	
	//�I�u�W�F�N�g�̔j��
	Release();
}

//================================================
//�X�V����
//================================================
void CBallistic::Update(void)
{
	for (int nCntOrbit = 0; nCntOrbit < BALLISTIC_MAX_ORBIT; nCntOrbit++)
	{
		if (m_apOrbit[nCntOrbit] != nullptr)
		{
			//�ʒu�ƃT�C�Y���擾
			D3DXVECTOR3 pos = m_apOrbit[nCntOrbit]->GetPos();

			//�ړ������ʒu����I�_�܂ł̃x�N�g�����Z�o
			D3DXVECTOR3 differVec = m_endPos - pos;
			float fDiffer = D3DXVec3Length(&differVec);

			if (fDiffer < m_size.x)
			{
				//����
				m_apOrbit[nCntOrbit]->Uninit();
				m_apOrbit[nCntOrbit] = nullptr;
				break;
			}

			D3DXVECTOR3 size = m_apOrbit[nCntOrbit]->GetSize();

			//��΂�������ݒ�
			D3DXVECTOR3 vec = m_endPos - m_bigenPos;
			//���K��
			D3DXVec3Normalize(&vec, &vec);
			//�ړ��ʂ�ݒ�
			D3DXVECTOR3 move = vec * m_fSpeed;
			pos += move;

			//�ړ������ʒu����I�_�܂ł̃x�N�g�����Z�o
			differVec = m_endPos - pos;
			fDiffer = D3DXVec3Length(&differVec);

			if (fDiffer < size.x)
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
void CBallistic::Draw(void)
{
	
}

//================================================
//��������
//================================================
CBallistic *CBallistic::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 size, const D3DXVECTOR3 rot, const D3DXVECTOR3 endPos,
	                     const float fSpeed, const string sTexPas1, const string sTexPas2)
{
	//�C���X�^���X�̐���
	CBallistic *pBallistic = nullptr;
	if (pBallistic == nullptr)
	{
		pBallistic = new CBallistic;
		if (pBallistic != nullptr)
		{
			pBallistic->m_bigenPos = pos;
			pBallistic->m_size = size;
			pBallistic->m_rot = rot;
			pBallistic->m_endPos = endPos;
			pBallistic->m_fSpeed = fSpeed;
			pBallistic->m_sTexPas1 = sTexPas1;
			pBallistic->m_sTexPas2 = sTexPas2;
			pBallistic->Init();
		}
	}
	return pBallistic;
}