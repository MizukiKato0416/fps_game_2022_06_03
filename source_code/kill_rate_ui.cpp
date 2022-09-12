//=============================================================================
//
// �L������UI���� [kill_rate_ui.cpp]
// Author :��������
//
//=============================================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "kill_rate_ui.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include "counter.h"
#include "letter.h"
#include "networkmanager.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
#define KILL_RATE_UI_RANK_DEFAULT_SIZE_X			(30.0f)			//�����N�̃f�t�H���g�T�C�Y
#define KILL_RATE_UI_RANK_DEFAULT_SIZE_Y			(50.0f)			//�����N�̃f�t�H���g�T�C�Y
#define KILL_RATE_UI_NAME_DEFAULT_SIZE_X			(16.0f)			//���O�̃f�t�H���g�T�C�Y
#define KILL_RATE_UI_NAME_DEFAULT_SIZE_Y			(24.0f)			//���O�̃f�t�H���g�T�C�Y
#define KILL_RATE_UI_MAX_PLAYER_NUM					(4)				//�v���C���[�̍ő�l��
#define KILL_RATE_UI_NAME_POS						(35.0f)			//���O�̈ʒu�����l
#define KILL_RATE_UI_KILL_POS						(375.0f)		//�L���̈ʒu�����l
#define KILL_RATE_UI_DEATH_POS						(465.0f)		//�f�X�̈ʒu�����l

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CKillRateUi::CKillRateUi(CObject::PRIORITY Priority) : CObject(Priority)
{
	m_pos = { 0.0f, 0.0f, 0.0f };
	m_scale = { 0.0f, 0.0f, 0.0f };
	m_nRank = 0;
	m_pKillCounter = nullptr;
	m_pDeathCounter = nullptr;
	m_ranking.clear();
	m_bSetPlayerNum = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CKillRateUi::~CKillRateUi()
{
	
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CKillRateUi::Init(void)
{
	//�ϐ�������
	m_nRank = 1;
	m_bSetPlayerNum = false;

	SetObjType(CObject::OBJTYPE::UI);

	//�t���[���̐���
	CObject2D *pFrame = nullptr;
	pFrame = CObject2D::Create(D3DXVECTOR3(m_pos.x - KILL_RATE_UI_FRAME_DEFAULT_SIZE_X / 2.0f, m_pos.y, m_pos.z),
	                           { KILL_RATE_UI_FRAME_DEFAULT_SIZE_X * m_scale.x, KILL_RATE_UI_FRAME_DEFAULT_SIZE_Y * m_scale.y, 0.0f },
		                       (int)CObject::PRIORITY::UI);
	pFrame->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("result_score_frame_02.png"));
	//���_�ʒu�����ɂ���
	pFrame->SetOriginType(CObject2D::ORIGIN_TYPE::LEFT);
	//���_�𓮂������̂Œ��_�����X�V
	pFrame->SetPos(m_pos, pFrame->GetSize());
	//���_�̏ꏊ���擾���Ĉʒu�ɏ㏑��
	m_pos = pFrame->GetPos();

	//�L����UI�̐���
	m_pKillCounter = CCounter::Create(D3DXVECTOR3(m_pos.x + (KILL_RATE_UI_KILL_POS * m_scale.x), m_pos.y , 0.0f),
		                              D3DXVECTOR3(KILL_RATE_UI_RANK_DEFAULT_SIZE_X * m_scale.x, KILL_RATE_UI_RANK_DEFAULT_SIZE_Y * m_scale.y, 0.0f),
		                              2, "Number.png");
	//�\������L������ݒ�
	m_pKillCounter->SetCounterNum(0);

	//�f�X��UI�̐���
	m_pDeathCounter = CCounter::Create(D3DXVECTOR3(m_pos.x + (KILL_RATE_UI_DEATH_POS * m_scale.x), m_pos.y , 0.0f),
		                               D3DXVECTOR3(KILL_RATE_UI_RANK_DEFAULT_SIZE_X * m_scale.x, KILL_RATE_UI_RANK_DEFAULT_SIZE_Y * m_scale.y, 0.0f),
		                               2, "Number.png");
	//�\������f�X����ݒ�
	m_pDeathCounter->SetCounterNum(0);


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
void CKillRateUi::Uninit(void)
{
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CKillRateUi::Update(void)
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
			m_ranking.push_back({ pData->Player.nNumber ,pData->Player.nKill ,pData->Player.nDeath ,pData->Player.aName[0]});

			for (int nCntEnemy = 0; nCntEnemy < nDataSize; nCntEnemy++)
			{
				//�G�̃L�����Ɣԍ�������
				m_ranking.push_back({ data[nCntEnemy].GetCmmuData()->Player.nNumber, data[nCntEnemy].GetCmmuData()->Player.nKill, 
					                  data[nCntEnemy].GetCmmuData()->Player.nDeath ,data[nCntEnemy].GetCmmuData()->Player.aName[0] });
			}
		}
		else
		{
			//�v���C���[�̃L�����Ɣԍ�������
			m_ranking[0] = { pData->Player.nNumber, pData->Player.nKill ,pData->Player.nDeath ,pData->Player.aName[0] };
			for (int nCntEnemy = 0; nCntEnemy < nDataSize; nCntEnemy++)
			{
				//�G�̃L�����Ɣԍ�������
				m_ranking[nCntEnemy + 1] = { data[nCntEnemy].GetCmmuData()->Player.nNumber, data[nCntEnemy].GetCmmuData()->Player.nKill , 
					                         data[nCntEnemy].GetCmmuData()->Player.nDeath ,data[nCntEnemy].GetCmmuData()->Player.aName[0] };
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
					KillRateRanking rankingBuff = m_ranking[nCntRank];
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

			//�\������f�X����ݒ�
			m_pDeathCounter->SetCounterNum(m_ranking[m_nRank - 1].nDeath);

			//������̐���
			CreateLetter(m_nRank - 1);
		}
		else
		{//����Ȃ�
			for (int nCnt = 0; nCnt < nRankingDataSize; nCnt++)
			{
				//�v���C���[�ԍ��ƈ�v���Ă�����
				if (m_ranking[nCnt].nPlayerNum == pData->Player.nNumber)
				{
					//�\�����郉���L���O�����̃f�[�^�̃����L���O�ɐݒ�
					m_nRank = nCnt + 1;
					//�\������L������ݒ�
					m_pKillCounter->SetCounterNum(m_ranking[nCnt].nKill);

					//�\������f�X����ݒ�
					m_pDeathCounter->SetCounterNum(m_ranking[nCnt].nDeath);

					//������̐���
					CreateLetter(nCnt);
					break;
				}
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CKillRateUi::Draw(void)
{
	
}

//=============================================================================
// ����
//=============================================================================
CKillRateUi *CKillRateUi::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &scale)
{
	//�C���X�^���X�̐���
	CKillRateUi *pKillRateUi = nullptr;
	if (pKillRateUi == nullptr)
	{
		pKillRateUi = new CKillRateUi();
		if (pKillRateUi != nullptr)
		{
			pKillRateUi->m_pos = pos;
			pKillRateUi->m_scale = scale;
			pKillRateUi->Init();
		}
	}
	return pKillRateUi;
}

//=============================================================================
//������������
//=============================================================================
void CKillRateUi::CreateLetter(const int &nRank)
{
	string buf;
	vector<wstring> conbrt_buf;
	int buf_size;

	int nRankSize = m_ranking.size();
	for (int nCountData = 0; nCountData < nRankSize; nCountData++)
	{
		if (m_ranking[nCountData].nPlayerNum == m_ranking[nRank].nPlayerNum)
		{
			buf = m_ranking[nCountData].aName;
		}
	}
	buf_size = buf.size();
	for (int nCntName = 0; nCntName < buf_size; nCntName++)
	{

		// SJIS �� wstring
		int iBufferSize = MultiByteToWideChar(CP_ACP,
			0,
			&buf[nCntName],
			-1,
			(wchar_t*)NULL,
			0);

		// �o�b�t�@�̎擾
		wchar_t* cpUCS2 = new wchar_t[iBufferSize];

		// SJIS �� wstring
		MultiByteToWideChar(CP_ACP,
			0,
			&buf[nCntName],
			-1,
			cpUCS2,
			iBufferSize);

		// string�̐���
		wstring utextbuf(cpUCS2, cpUCS2 + iBufferSize - 1);

		// �o�b�t�@�̔j��
		delete[] cpUCS2;

		conbrt_buf.push_back(utextbuf);
	}

	//�����̔j��
	int nSize = m_name_font.size();
	for (int nCntName = 0; nCntName < nSize; nCntName++)
	{
		if (m_name_font[nCntName] != nullptr)
		{
			m_name_font[nCntName]->Uninit();
			m_name_font[nCntName] = nullptr;
		}
	}
	m_name_font.clear();

	buf_size = conbrt_buf.size();
	for (int count_name = 0; count_name < buf_size; count_name++)
	{
		m_name_font.push_back(new CLetter);

		m_name_font[count_name]->SetPos(D3DXVECTOR3(m_pos.x + (KILL_RATE_UI_NAME_POS * m_scale.x) + (KILL_RATE_UI_NAME_DEFAULT_SIZE_X * 2.0f * count_name), m_pos.y, 0.0f));
		m_name_font[count_name]->SetSize(D3DXVECTOR3(KILL_RATE_UI_NAME_DEFAULT_SIZE_X, KILL_RATE_UI_NAME_DEFAULT_SIZE_Y, 0.0f));
		m_name_font[count_name]->SetText(conbrt_buf[0][count_name]);
		m_name_font[count_name]->SetFontSize(30);
		m_name_font[count_name]->SetFontWeight(50);
		m_name_font[count_name]->Init();
	}
	conbrt_buf.clear();
}
