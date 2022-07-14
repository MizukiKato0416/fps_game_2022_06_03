//================================================
//リザルト処理
//Author: 加藤瑞葵
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
//更新処理マクロ
//===============================================
#define DISPLAY_FADE (500)

//================================================
//デフォルトコンストラクタ
//================================================
CResult::CResult(CObject::PRIORITY Priority) :CObject(Priority)
{

}

//================================================
//デストラクタ
//================================================
CResult::~CResult()
{

}

//================================================
//初期化処理
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

	m_Ui.push_back(CObject2D::Create(D3DXVECTOR3((SCREEN_WIDTH / 2) - 100.0f, 0.0f + (65.0f * 5.0f), 0.0f), D3DXVECTOR3(166.0f, 65.0f, 0.0f), (int)CObject::PRIORITY::UI));
	m_Ui[1]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("Basket.png"));
	m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (140.0f), 0.0f + (65.0f * 5.0f), 0.0f), D3DXVECTOR3(50.0f, 65.0f, 0.0f), 2, "kill_num.png"));
	m_Countor[0]->SetCounterNum(pPlayerData->Player.nKill);
	m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (140.0f + 150.0f), 0.0f + (65.0f * 5.0f), 0.0f), D3DXVECTOR3(50.0f, 65.0f, 0.0f), 2, "death_num.png"));
	m_Countor[1]->SetCounterNum(pPlayerData->Player.nDeath);

	m_Ui.push_back(CObject2D::Create(D3DXVECTOR3((SCREEN_WIDTH / 2) - 100.0f, 0.0f + (65.0f * 6.0f), 0.0f), D3DXVECTOR3(166.0f, 65.0f, 0.0f), (int)CObject::PRIORITY::UI));
	m_Ui[2]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("box.jpg"));
	m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (140.0f), 0.0f + (65.0f * 6.0f), 0.0f), D3DXVECTOR3(50.0f, 65.0f, 0.0f), 2, "kill_num.png"));
	m_Countor[2]->SetCounterNum(pPlayerData->Player.nKill);
	m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (140.0f + 150.0f), 0.0f + (65.0f * 6.0f), 0.0f), D3DXVECTOR3(50.0f, 65.0f, 0.0f), 2, "death_num.png"));
	m_Countor[3]->SetCounterNum(pPlayerData->Player.nDeath);

	m_Ui.push_back(CObject2D::Create(D3DXVECTOR3((SCREEN_WIDTH / 2) - 100.0f, 0.0f + (65.0f * 7.0f), 0.0f), D3DXVECTOR3(166.0f, 65.0f, 0.0f), (int)CObject::PRIORITY::UI));
	m_Ui[3]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("building300.jpg"));
	m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (140.0f), 0.0f + (65.0f * 7.0f), 0.0f), D3DXVECTOR3(50.0f, 65.0f, 0.0f), 2, "kill_num.png"));
	m_Countor[4]->SetCounterNum(pPlayerData->Player.nKill);
	m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (140.0f + 150.0f), 0.0f + (65.0f * 7.0f), 0.0f), D3DXVECTOR3(50.0f, 65.0f, 0.0f), 2, "death_num.png"));
	m_Countor[5]->SetCounterNum(pPlayerData->Player.nDeath);

	m_Ui.push_back(CObject2D::Create(D3DXVECTOR3((SCREEN_WIDTH / 2) - 100.0f, 0.0f + (65.0f * 8.0f), 0.0f), D3DXVECTOR3(166.0f, 65.0f, 0.0f), (int)CObject::PRIORITY::UI));
	m_Ui[4]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("Container.jpg"));
	m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (140.0f), 0.0f + (65.0f * 8.0f), 0.0f), D3DXVECTOR3(50.0f, 65.0f, 0.0f), 2, "kill_num.png"));
	m_Countor[6]->SetCounterNum(pPlayerData->Player.nKill);
	m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (140.0f + 150.0f), 0.0f + (65.0f * 8.0f), 0.0f), D3DXVECTOR3(50.0f, 65.0f, 0.0f), 2, "death_num.png"));
	m_Countor[7]->SetCounterNum(pPlayerData->Player.nDeath);

	return S_OK;
}

//================================================
//終了処理
//================================================
void CResult::Uninit(void)
{	
	//オブジェクトの破棄
	Release();
}

//================================================
//更新処理
//================================================
void CResult::Update(void)
{
	m_nCount++;
	if (m_nCount >= DISPLAY_FADE)
	{
		//フェード取得処理
		CFade *pFade;
		pFade = CManager::GetInstance()->GetFade();

		if (pFade->GetFade() == CFade::FADE_NONE)
		{
			pFade->SetFade(CManager::MODE::TITLE);
		}
	}
}

//================================================
//描画処理
//================================================
void CResult::Draw(void)
{
	
}
