//=============================================================================
//
// �X�R�A��UI���� [score_ui.cpp]
// Author :��������
//
//=============================================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "score_ui.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include "counter.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
#define SCORE_UI_FRAME_DEFAULT_SIZE_X	(304.0f)		//�t���[���̃f�t�H���g�T�C�Y
#define SCORE_UI_FRAME_DEFAULT_SIZE_Y	(42.0f)			//�t���[���̃f�t�H���g�T�C�Y
#define SCORE_UI_RANK_DEFAULT_SIZE_X	(35.0f)			//�����N�̃f�t�H���g�T�C�Y
#define SCORE_UI_RANK_DEFAULT_SIZE_Y	(35.0f)			//�����N�̃f�t�H���g�T�C�Y

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CScoreUi::CScoreUi(CObject::PRIORITY Priority) : CObject(Priority)
{
	m_pos = { 0.0f, 0.0f, 0.0f };
	m_scale = { 0.0f, 0.0f, 0.0f };
	m_nRank = 0;
	m_pRankCounter = nullptr;
	m_pKillCounter = nullptr;
}

//=============================================================================
// �f�t�H���g�f�X�g���N�^
//=============================================================================
CScoreUi::~CScoreUi()
{
	
}

//=============================================================================
// ���f���̏���������
//=============================================================================
HRESULT CScoreUi::Init(void)
{
	//�ϐ�������
	m_nRank = 1;

	SetObjType(CObject::OBJTYPE::UI);

	//�t���[���̐���
	CObject2D *pFrame = nullptr;
	pFrame = CObject2D::Create(D3DXVECTOR3(m_pos.x - SCORE_UI_FRAME_DEFAULT_SIZE_X / 2.0f, m_pos.y, m_pos.z),
	                           { SCORE_UI_FRAME_DEFAULT_SIZE_X * m_scale.x, SCORE_UI_FRAME_DEFAULT_SIZE_Y * m_scale.y, 0.0f },
		                       (int)CObject::PRIORITY::UI);
	pFrame->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("score_frame.png"));
	//���_�ʒu�����ɂ���
	pFrame->SetOriginType(CObject2D::ORIGIN_TYPE::LEFT);

	//����UI�̐���
	m_pRankCounter = CCounter::Create(D3DXVECTOR3(m_pos.x + (10.0f * m_scale.x) , m_pos.y , 0.0f),
		                              D3DXVECTOR3(SCORE_UI_RANK_DEFAULT_SIZE_X, SCORE_UI_RANK_DEFAULT_SIZE_Y, 0.0f), 1, "Number.png");
	//�\�����鏇�ʂ��w��
	m_pRankCounter->SetCounterNum(m_nRank);


	//���OUI�̐���


	//�L����UI�̐���
	m_pKillCounter = CCounter::Create(D3DXVECTOR3(m_pos.x + (250.0f * m_scale.x) , m_pos.y , 0.0f),
		                              D3DXVECTOR3(SCORE_UI_RANK_DEFAULT_SIZE_X, SCORE_UI_RANK_DEFAULT_SIZE_Y, 0.0f), 2, "Number.png");
	//�\������L������ݒ�
	m_pKillCounter->SetCounterNum(m_nRank);


	return S_OK;
}

//=============================================================================
// ���f���̏I������
//=============================================================================
void CScoreUi::Uninit(void)
{
	Release();
}

//=============================================================================
// ���f���̍X�V����
//=============================================================================
void CScoreUi::Update(void)
{
	
}

//=============================================================================
// ���f���̕`�揈��
//=============================================================================
void CScoreUi::Draw(void)
{
	
}

//=============================================================================
// ���f���̐���
//=============================================================================
CScoreUi *CScoreUi::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &scale)
{
	//�C���X�^���X�̐���
	CScoreUi *pScoreUi = nullptr;
	if (pScoreUi == nullptr)
	{
		pScoreUi = new CScoreUi();
		if (pScoreUi != nullptr)
		{
			pScoreUi->m_pos = pos;
			pScoreUi->m_scale = scale;
			pScoreUi->Init();
		}
	}
	return pScoreUi;
}