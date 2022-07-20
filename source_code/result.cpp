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
			m_Ui[0]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture(""));
			break;
		case 2:
			m_Ui[0]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture(""));
			break;
		case 3:
			m_Ui[0]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture(""));
			break;
		case 4:
			m_Ui[0]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture(""));
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
				m_Ui[0]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture(""));
				break;
			case 2:
				m_Ui[0]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture(""));
				break;
			case 3:
				m_Ui[0]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture(""));
				break;
			case 4:
				m_Ui[0]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture(""));
				break;
			default:
				break;
			}
		}
	}

	CCommunicationData::COMMUNICATION_DATA *pData[MAX_PLAYER + 1];
	float difference = 5.0f;
	pData[0] = CManager::GetInstance()->GetNetWorkmanager()->GetPlayerData()->GetCmmuData();
	for (int count_enemy = 0; count_enemy < max_enemy; count_enemy++)
	{
		pData[count_enemy + 1] = EnemyData[count_enemy].GetCmmuData();
	}

	for (int count_player = 0, count_arry = 0; count_player < MAX_PLAYER + 1; count_player++, count_arry++)
	{
		switch (pData[count_player]->Player.nNumber)
		{
		case 1:
			m_Ui.push_back(CObject2D::Create(D3DXVECTOR3((SCREEN_WIDTH / 2) - 100.0f, 0.0f + (65.0f * difference), 0.0f), D3DXVECTOR3(166.0f, 65.0f, 0.0f), (int)CObject::PRIORITY::UI));
			m_Ui[count_player + 1]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("Basket.png"));
			m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (140.0f), 0.0f + (65.0f * difference), 0.0f), D3DXVECTOR3(50.0f, 65.0f, 0.0f), 2, "kill_num.png"));
			m_Countor[count_arry]->SetCounterNum(pData[count_player]->Player.nKill);
			m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (140.0f + 150.0f), 0.0f + (65.0f * difference), 0.0f), D3DXVECTOR3(50.0f, 65.0f, 0.0f), 2, "death_num.png"));
			m_Countor[count_arry + 1]->SetCounterNum(pData[count_player]->Player.nDeath);
			break;
		case 2:
			m_Ui.push_back(CObject2D::Create(D3DXVECTOR3((SCREEN_WIDTH / 2) - 100.0f, 0.0f + (65.0f * difference), 0.0f), D3DXVECTOR3(166.0f, 65.0f, 0.0f), (int)CObject::PRIORITY::UI));
			m_Ui[count_player + 1]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("box.jpg"));
			m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (140.0f), 0.0f + (65.0f * difference), 0.0f), D3DXVECTOR3(50.0f, 65.0f, 0.0f), 2, "kill_num.png"));
			m_Countor[count_arry]->SetCounterNum(pData[count_player]->Player.nKill);
			m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (140.0f + 150.0f), 0.0f + (65.0f * difference), 0.0f), D3DXVECTOR3(50.0f, 65.0f, 0.0f), 2, "death_num.png"));
			m_Countor[count_arry + 1]->SetCounterNum(pData[count_player]->Player.nDeath);
			break;
		case 3:
			m_Ui.push_back(CObject2D::Create(D3DXVECTOR3((SCREEN_WIDTH / 2) - 100.0f, 0.0f + (65.0f * difference), 0.0f), D3DXVECTOR3(166.0f, 65.0f, 0.0f), (int)CObject::PRIORITY::UI));
			m_Ui[count_player + 1]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("building300.jpg"));
			m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (140.0f), 0.0f + (65.0f * difference), 0.0f), D3DXVECTOR3(50.0f, 65.0f, 0.0f), 2, "kill_num.png"));
			m_Countor[count_arry]->SetCounterNum(pData[count_player]->Player.nKill);
			m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (140.0f + 150.0f), 0.0f + (65.0f * difference), 0.0f), D3DXVECTOR3(50.0f, 65.0f, 0.0f), 2, "death_num.png"));
			m_Countor[count_arry + 1]->SetCounterNum(pData[count_player]->Player.nDeath);
			break;
		case 4:
			m_Ui.push_back(CObject2D::Create(D3DXVECTOR3((SCREEN_WIDTH / 2) - 100.0f, 0.0f + (65.0f * difference), 0.0f), D3DXVECTOR3(166.0f, 65.0f, 0.0f), (int)CObject::PRIORITY::UI));
			m_Ui[count_player + 1]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("Container.jpg"));
			m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (140.0f), 0.0f + (65.0f * difference), 0.0f), D3DXVECTOR3(50.0f, 65.0f, 0.0f), 2, "kill_num.png"));
			m_Countor[count_arry]->SetCounterNum(pData[count_player]->Player.nKill);
			m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (140.0f + 150.0f), 0.0f + (65.0f * difference), 0.0f), D3DXVECTOR3(50.0f, 65.0f, 0.0f), 2, "death_num.png"));
			m_Countor[count_arry + 1]->SetCounterNum(pData[count_player]->Player.nDeath);
			break;
		default:
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
