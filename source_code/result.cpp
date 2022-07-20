//================================================
//���U���g����
//Author: ��������
//================================================
#include "result.h"
#include "manager.h"
#include "fade.h"
#include "object2D.h"
#include "communicationdata.h"
#include "manager.h"
#include "networkmanager.h"
#include "counter.h"

//================================================
//�X�V�����}�N��
//===============================================
#define DISPLAY_FADE (500)

//================================================
//�f�t�H���g�R���X�g���N�^
//================================================
CResult::CResult(CObject::PRIORITY Priority) :CObject(Priority)
{

}

//================================================
//�f�X�g���N�^
//================================================
CResult::~CResult()
{

}

//================================================
//����������
//================================================
HRESULT CResult::Init(void)
{
	CCommunicationData::COMMUNICATION_DATA *pPlayerData = CManager::GetInstance()->GetNetWorkmanager()->GetPlayerData()->GetCmmuData();
	vector<CCommunicationData> EnemyData = CManager::GetInstance()->GetNetWorkmanager()->GetEnemyData();
	int max_enemy = EnemyData.size();

	m_Ui.push_back(CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 0.0f + (100.0f * 2.0f), 0.0f), D3DXVECTOR3(600.0f, 100.0f, 0.0f), (int)CObject::PRIORITY::UI));
	if (pPlayerData->Player.bWin == true)
	{
		switch (pPlayerData->Player.nNumber)
		{
		case 1:
			m_Ui[0]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("win_player1.png"));
			break;
		case 2:
			m_Ui[0]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("win_player2.png"));
			break;
		case 3:
			m_Ui[0]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("win_player3.png"));
			break;
		case 4:
			m_Ui[0]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("win_player4.png"));
			break;
		default:
			break;
		}
	}
	for (int count_enemy = 0; count_enemy < max_enemy; count_enemy++)
	{
		CCommunicationData::COMMUNICATION_DATA *pEnemyData = EnemyData[count_enemy].GetCmmuData();
		if (pEnemyData->Player.bWin == true)
		{
			switch (pEnemyData->Player.nNumber)
			{
			case 1:
				m_Ui[0]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("win_player1.png"));
				break;
			case 2:
				m_Ui[0]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("win_player2.png"));
				break;
			case 3:
				m_Ui[0]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("win_player3.png"));
				break;
			case 4:
				m_Ui[0]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("win_player4.png"));
				break;
			default:
				break;
			}
		}
	}

	m_Ui.push_back(CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, (SCREEN_WIDTH / 2) - 150.0f, 0.0f), D3DXVECTOR3(549.0f * 0.8f, 486.0f * 0.8f, 0.0f), (int)CObject::PRIORITY::UI));
	m_Ui[1]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("p_scoreboard.png"));

	CCommunicationData::COMMUNICATION_DATA *pData[MAX_PLAYER + 1];
	float difference = 5.0f;
	pData[0] = CManager::GetInstance()->GetNetWorkmanager()->GetPlayerData()->GetCmmuData();
	for (int count_enemy = 0; count_enemy < max_enemy; count_enemy++)
	{
		pData[count_enemy + 1] = EnemyData[count_enemy].GetCmmuData();
	}

	for (int count_player = 0, countor = 0; count_player < MAX_PLAYER + 1; count_player++, countor += 2)
	{
		switch (pData[count_player]->Player.nNumber)
		{
		case 1:
			m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (200.0f), 0.0f + (83.5f * difference), 0.0f), D3DXVECTOR3(25.0f, 32.5f, 0.0f), 2, "kill_num.png"));
			m_Countor[countor]->SetCounterNum(pData[count_player]->Player.nKill);
			m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (200.0f + 65.0f), 0.0f + (83.5f * difference), 0.0f), D3DXVECTOR3(25.0f, 32.5f, 0.0f), 2, "death_num.png"));
			m_Countor[countor + 1]->SetCounterNum(pData[count_player]->Player.nDeath);
			break;
		case 2:
			m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (200.0f), 0.0f + (83.5f * difference), 0.0f), D3DXVECTOR3(25.0f, 32.5f, 0.0f), 2, "kill_num.png"));
			m_Countor[countor]->SetCounterNum(pData[count_player]->Player.nKill);
			m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (200.0f + 65.0f), 0.0f + (83.5f * difference), 0.0f), D3DXVECTOR3(25.0f, 32.5f, 0.0f), 2, "death_num.png"));
			m_Countor[countor + 1]->SetCounterNum(pData[count_player]->Player.nDeath);
			break;
		case 3:
			m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (200.0f), 0.0f + (83.5f * difference), 0.0f), D3DXVECTOR3(25.0f, 32.5f, 0.0f), 2, "kill_num.png"));
			m_Countor[countor]->SetCounterNum(pData[count_player]->Player.nKill);
			m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (200.0f + 65.0f), 0.0f + (83.5f * difference), 0.0f), D3DXVECTOR3(25.0f, 32.5f, 0.0f), 2, "death_num.png"));
			m_Countor[countor + 1]->SetCounterNum(pData[count_player]->Player.nDeath);
			break;
		case 4:
			m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (200.0f), 0.0f + (83.5f * difference), 0.0f), D3DXVECTOR3(25.0f, 32.5f, 0.0f), 2, "kill_num.png"));
			m_Countor[countor]->SetCounterNum(pData[count_player]->Player.nKill);
			m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (200.0f + 65.0f), 0.0f + (83.5f * difference), 0.0f), D3DXVECTOR3(25.0f, 32.5f, 0.0f), 2, "death_num.png"));
			m_Countor[countor + 1]->SetCounterNum(pData[count_player]->Player.nDeath);
			break;
		default:
			m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (200.0f), 0.0f + (83.5f * difference), 0.0f), D3DXVECTOR3(25.0f, 32.5f, 0.0f), 2, "kill_num.png"));
			m_Countor[countor]->SetCounterNum(pData[count_player]->Player.nKill);
			m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (200.0f + 65.0f), 0.0f + (83.5f * difference), 0.0f), D3DXVECTOR3(25.0f, 32.5f, 0.0f), 2, "death_num.png"));
			m_Countor[countor + 1]->SetCounterNum(pData[count_player]->Player.nDeath);
			break;
		}
		difference += 1.0f;
	}

	return S_OK;
}

//================================================
//�I������
//================================================
void CResult::Uninit(void)
{	
	//�I�u�W�F�N�g�̔j��
	Release();

	//�ʐM�؂�
	CTcpClient *pClient = CManager::GetInstance()->GetNetWorkmanager()->GetCommunication();
	pClient->Uninit();

	//�R�~���f�[�^������
	CManager::GetInstance()->GetNetWorkmanager()->Reset();
}

//================================================
//�X�V����
//================================================
void CResult::Update(void)
{
	m_nCount++;
	if (m_nCount >= DISPLAY_FADE)
	{
		//�t�F�[�h�擾����
		CFade *pFade;
		pFade = CManager::GetInstance()->GetFade();

		if (pFade->GetFade() == CFade::FADE_NONE)
		{
			pFade->SetFade(CManager::MODE::TITLE);
		}
	}
}

//================================================
//�`�揈��
//================================================
void CResult::Draw(void)
{
	
}
