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
#include "networkmanager.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
#define SCORE_UI_FRAME_DEFAULT_SIZE_X	(304.0f)		//�t���[���̃f�t�H���g�T�C�Y
#define SCORE_UI_FRAME_DEFAULT_SIZE_Y	(42.0f)			//�t���[���̃f�t�H���g�T�C�Y
#define SCORE_UI_RANK_DEFAULT_SIZE_X	(15.0f)			//�����N�̃f�t�H���g�T�C�Y
#define SCORE_UI_RANK_DEFAULT_SIZE_Y	(15.0f)			//�����N�̃f�t�H���g�T�C�Y

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScoreUi::CScoreUi(CObject::PRIORITY Priority) : CObject(Priority)
{
	m_pos = { 0.0f, 0.0f, 0.0f };
	m_scale = { 0.0f, 0.0f, 0.0f };
	m_nRank = 0;
	m_pRankCounter = nullptr;
	m_pKillCounter = nullptr;
	m_ranking.clear();
	m_bSetPlayerNum = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScoreUi::~CScoreUi()
{
	
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CScoreUi::Init(void)
{
	//�ϐ�������
	m_nRank = 1;
	m_bSetPlayerNum = false;

	SetObjType(CObject::OBJTYPE::UI);

	//�t���[���̐���
	CObject2D *pFrame = nullptr;
	pFrame = CObject2D::Create(D3DXVECTOR3(m_pos.x - SCORE_UI_FRAME_DEFAULT_SIZE_X / 2.0f, m_pos.y, m_pos.z),
	                           { SCORE_UI_FRAME_DEFAULT_SIZE_X * m_scale.x, SCORE_UI_FRAME_DEFAULT_SIZE_Y * m_scale.y, 0.0f },
		                       (int)CObject::PRIORITY::UI);
	pFrame->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("score_frame.png"));
	//���_�ʒu�����ɂ���
	pFrame->SetOriginType(CObject2D::ORIGIN_TYPE::LEFT);
	//���_�𓮂������̂Œ��_�����X�V
	pFrame->SetPos(m_pos, pFrame->GetSize());
	//���_�̏ꏊ���擾���Ĉʒu�ɏ㏑��
	m_pos = pFrame->GetPos();

	//����UI�̐���
	m_pRankCounter = CCounter::Create(D3DXVECTOR3(m_pos.x + (30.0f * m_scale.x), m_pos.y , 0.0f),
		                              D3DXVECTOR3(SCORE_UI_RANK_DEFAULT_SIZE_X * m_scale.x, SCORE_UI_RANK_DEFAULT_SIZE_Y * m_scale.y, 0.0f),
		                              1, "Number.png");
	//�\�����鏇�ʂ��w��
	m_pRankCounter->SetCounterNum(m_nRank);


	//���OUI�̐���


	//�L����UI�̐���
	m_pKillCounter = CCounter::Create(D3DXVECTOR3(m_pos.x + (268.0f * m_scale.x), m_pos.y , 0.0f),
		                              D3DXVECTOR3(SCORE_UI_RANK_DEFAULT_SIZE_X * m_scale.x, SCORE_UI_RANK_DEFAULT_SIZE_Y * m_scale.y, 0.0f),
		                              2, "Number.png");
	//�\������L������ݒ�
	m_pKillCounter->SetCounterNum(0);


	//�G�̃f�[�^�擾
	vector<CCommunicationData> data = CManager::GetInstance()->GetNetWorkmanager()->GetEnemyData();

	int nDataSize = data.size();
	//�G��������
	if (nDataSize > 0)
	{

		//�v���C���[�̃f�[�^�擾
		CCommunicationData::COMMUNICATION_DATA *pData = CManager::GetInstance()->GetNetWorkmanager()->GetPlayerData()->GetCmmuData();

		if (m_ranking.size() == 0)
		{
			//�v���C���[�̃L�����Ɣԍ�������
			m_ranking.push_back({ pData->Player.nNumber ,pData->Player.nKill });

			for (int nCntEnemy = 0; nCntEnemy < nDataSize; nCntEnemy++)
			{
				//�G�̃L����������
				m_ranking.push_back({ data[nCntEnemy].GetCmmuData()->Player.nNumber, data[nCntEnemy].GetCmmuData()->Player.nKill });
			}
		}
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CScoreUi::Uninit(void)
{
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CScoreUi::Update(void)
{
	//�G�̃f�[�^�擾
	vector<CCommunicationData> data = CManager::GetInstance()->GetNetWorkmanager()->GetEnemyData();

	int nDataSize = data.size();
	//�G��������
	if (nDataSize > 0)
	{
		
		//�v���C���[�̃f�[�^�擾
		CCommunicationData::COMMUNICATION_DATA *pData = CManager::GetInstance()->GetNetWorkmanager()->GetPlayerData()->GetCmmuData();

		if (m_ranking.size() == 0)
		{
			//�v���C���[�̃L�����Ɣԍ�������
			m_ranking.push_back({ pData->Player.nNumber ,pData->Player.nKill });

			for (int nCntEnemy = 0; nCntEnemy < nDataSize; nCntEnemy++)
			{
				//�G�̃L�����Ɣԍ�������
				m_ranking.push_back({ data[nCntEnemy].GetCmmuData()->Player.nNumber, data[nCntEnemy].GetCmmuData()->Player.nKill });
			}
		}
		else
		{
			//�v���C���[�̃L�����Ɣԍ�������
			m_ranking[0] = { pData->Player.nNumber, pData->Player.nKill };
			for (int nCntEnemy = 0; nCntEnemy < nDataSize; nCntEnemy++)
			{
				//�G�̃L�����Ɣԍ�������
				m_ranking[nCntEnemy + 1] = { data[nCntEnemy].GetCmmuData()->Player.nNumber, data[nCntEnemy].GetCmmuData()->Player.nKill };
			}
		}

		int nRankingDataSize = m_ranking.size();
		//�\�[�g����
		for (int nCnt = 0; nCnt < nRankingDataSize; nCnt++)
		{
			for (int nCntRank = 0; nCntRank < nRankingDataSize - 1; nCntRank++)
			{
				//����̃L���������̐l�̃L������菭�Ȃ��Ƃ�
				if (m_ranking[nCntRank].nKill < m_ranking[nCntRank + 1].nKill)
				{
					//�ꎞ�ۑ�
					Ranking rankingBuff = m_ranking[nCntRank];
					//����ւ�
					m_ranking[nCntRank] = m_ranking[nCntRank + 1];
					//���
					m_ranking[nCntRank + 1] = rankingBuff;
				}
			}
		}

		//�v���C���[�̔ԍ�����\������f�[�^���Q�Ƃ��Ȃ��Ȃ�
		if (!m_bSetPlayerNum)
		{
			//�\������L������ݒ�
			m_pKillCounter->SetCounterNum(m_ranking[m_nRank - 1].nKill);
			//�\�����鏇�ʂ��w��
			m_pRankCounter->SetCounterNum(m_nRank);
		}
		else
		{//����Ȃ�
			for (int nCnt = 0; nCnt < nRankingDataSize; nCnt++)
			{
				//�v���C���[�ԍ��ƈ�v���Ă�����
				if (m_ranking[nCnt].nPlayerNum == pData->Player.nNumber)
				{
					//�\������L������ݒ�
					m_pKillCounter->SetCounterNum(m_ranking[nCnt].nKill);
					//�\�����鏇�ʂ��w��
					m_pRankCounter->SetCounterNum(nCnt + 1);
					break;
				}
			}
		}
		
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CScoreUi::Draw(void)
{
	
}

//=============================================================================
// ����
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