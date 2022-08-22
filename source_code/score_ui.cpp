//=============================================================================
//
// スコアのUI処理 [score_ui.cpp]
// Author :加藤瑞葵
//
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include "score_ui.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include "counter.h"
#include "networkmanager.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
#define SCORE_UI_FRAME_DEFAULT_SIZE_X	(304.0f)		//フレームのデフォルトサイズ
#define SCORE_UI_FRAME_DEFAULT_SIZE_Y	(42.0f)			//フレームのデフォルトサイズ
#define SCORE_UI_RANK_DEFAULT_SIZE_X	(15.0f)			//ランクのデフォルトサイズ
#define SCORE_UI_RANK_DEFAULT_SIZE_Y	(15.0f)			//ランクのデフォルトサイズ

//=============================================================================
// コンストラクタ
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
// デストラクタ
//=============================================================================
CScoreUi::~CScoreUi()
{
	
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScoreUi::Init(void)
{
	//変数初期化
	m_nRank = 1;
	m_bSetPlayerNum = false;

	SetObjType(CObject::OBJTYPE::UI);

	//フレームの生成
	CObject2D *pFrame = nullptr;
	pFrame = CObject2D::Create(D3DXVECTOR3(m_pos.x - SCORE_UI_FRAME_DEFAULT_SIZE_X / 2.0f, m_pos.y, m_pos.z),
	                           { SCORE_UI_FRAME_DEFAULT_SIZE_X * m_scale.x, SCORE_UI_FRAME_DEFAULT_SIZE_Y * m_scale.y, 0.0f },
		                       (int)CObject::PRIORITY::UI);
	pFrame->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("score_frame.png"));
	//原点位置を左にする
	pFrame->SetOriginType(CObject2D::ORIGIN_TYPE::LEFT);
	//原点を動かしたので頂点情報を更新
	pFrame->SetPos(m_pos, pFrame->GetSize());
	//原点の場所を取得して位置に上書き
	m_pos = pFrame->GetPos();

	//順位UIの生成
	m_pRankCounter = CCounter::Create(D3DXVECTOR3(m_pos.x + (30.0f * m_scale.x), m_pos.y , 0.0f),
		                              D3DXVECTOR3(SCORE_UI_RANK_DEFAULT_SIZE_X * m_scale.x, SCORE_UI_RANK_DEFAULT_SIZE_Y * m_scale.y, 0.0f),
		                              1, "Number.png");
	//表示する順位を指定
	m_pRankCounter->SetCounterNum(m_nRank);


	//名前UIの生成


	//キル数UIの生成
	m_pKillCounter = CCounter::Create(D3DXVECTOR3(m_pos.x + (268.0f * m_scale.x), m_pos.y , 0.0f),
		                              D3DXVECTOR3(SCORE_UI_RANK_DEFAULT_SIZE_X * m_scale.x, SCORE_UI_RANK_DEFAULT_SIZE_Y * m_scale.y, 0.0f),
		                              2, "Number.png");
	//表示するキル数を設定
	m_pKillCounter->SetCounterNum(0);


	//敵のデータ取得
	vector<CCommunicationData> data = CManager::GetInstance()->GetNetWorkmanager()->GetEnemyData();

	int nDataSize = data.size();
	//敵がいたら
	if (nDataSize > 0)
	{

		//プレイヤーのデータ取得
		CCommunicationData::COMMUNICATION_DATA *pData = CManager::GetInstance()->GetNetWorkmanager()->GetPlayerData()->GetCmmuData();

		if (m_ranking.size() == 0)
		{
			//プレイヤーのキル数と番号を入れる
			m_ranking.push_back({ pData->Player.nNumber ,pData->Player.nKill });

			for (int nCntEnemy = 0; nCntEnemy < nDataSize; nCntEnemy++)
			{
				//敵のキル数を入れる
				m_ranking.push_back({ data[nCntEnemy].GetCmmuData()->Player.nNumber, data[nCntEnemy].GetCmmuData()->Player.nKill });
			}
		}
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScoreUi::Uninit(void)
{
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CScoreUi::Update(void)
{
	//敵のデータ取得
	vector<CCommunicationData> data = CManager::GetInstance()->GetNetWorkmanager()->GetEnemyData();

	int nDataSize = data.size();
	//敵がいたら
	if (nDataSize > 0)
	{
		
		//プレイヤーのデータ取得
		CCommunicationData::COMMUNICATION_DATA *pData = CManager::GetInstance()->GetNetWorkmanager()->GetPlayerData()->GetCmmuData();

		if (m_ranking.size() == 0)
		{
			//プレイヤーのキル数と番号を入れる
			m_ranking.push_back({ pData->Player.nNumber ,pData->Player.nKill });

			for (int nCntEnemy = 0; nCntEnemy < nDataSize; nCntEnemy++)
			{
				//敵のキル数と番号を入れる
				m_ranking.push_back({ data[nCntEnemy].GetCmmuData()->Player.nNumber, data[nCntEnemy].GetCmmuData()->Player.nKill });
			}
		}
		else
		{
			//プレイヤーのキル数と番号を入れる
			m_ranking[0] = { pData->Player.nNumber, pData->Player.nKill };
			for (int nCntEnemy = 0; nCntEnemy < nDataSize; nCntEnemy++)
			{
				//敵のキル数と番号を入れる
				m_ranking[nCntEnemy + 1] = { data[nCntEnemy].GetCmmuData()->Player.nNumber, data[nCntEnemy].GetCmmuData()->Player.nKill };
			}
		}

		int nRankingDataSize = m_ranking.size();
		//ソートする
		for (int nCnt = 0; nCnt < nRankingDataSize; nCnt++)
		{
			for (int nCntRank = 0; nCntRank < nRankingDataSize - 1; nCntRank++)
			{
				//今回のキル数が次の人のキル数より少ないとき
				if (m_ranking[nCntRank].nKill < m_ranking[nCntRank + 1].nKill)
				{
					//一時保存
					Ranking rankingBuff = m_ranking[nCntRank];
					//入れ替え
					m_ranking[nCntRank] = m_ranking[nCntRank + 1];
					//代入
					m_ranking[nCntRank + 1] = rankingBuff;
				}
			}
		}

		//プレイヤーの番号から表示するデータを参照しないなら
		if (!m_bSetPlayerNum)
		{
			//表示するキル数を設定
			m_pKillCounter->SetCounterNum(m_ranking[m_nRank - 1].nKill);
			//表示する順位を指定
			m_pRankCounter->SetCounterNum(m_nRank);
		}
		else
		{//するなら
			for (int nCnt = 0; nCnt < nRankingDataSize; nCnt++)
			{
				//プレイヤー番号と一致していたら
				if (m_ranking[nCnt].nPlayerNum == pData->Player.nNumber)
				{
					//表示するキル数を設定
					m_pKillCounter->SetCounterNum(m_ranking[nCnt].nKill);
					//表示する順位を指定
					m_pRankCounter->SetCounterNum(nCnt + 1);
					break;
				}
			}
		}
		
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CScoreUi::Draw(void)
{
	
}

//=============================================================================
// 生成
//=============================================================================
CScoreUi *CScoreUi::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &scale)
{
	//インスタンスの生成
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