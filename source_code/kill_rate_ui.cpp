//=============================================================================
//
// キルレのUI処理 [kill_rate_ui.cpp]
// Author :加藤瑞葵
//
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include "kill_rate_ui.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include "counter.h"
#include "letter.h"
#include "networkmanager.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
#define KILL_RATE_UI_RANK_DEFAULT_SIZE_X			(30.0f)			//ランクのデフォルトサイズ
#define KILL_RATE_UI_RANK_DEFAULT_SIZE_Y			(50.0f)			//ランクのデフォルトサイズ
#define KILL_RATE_UI_NAME_DEFAULT_SIZE_X			(16.0f)			//名前のデフォルトサイズ
#define KILL_RATE_UI_NAME_DEFAULT_SIZE_Y			(24.0f)			//名前のデフォルトサイズ
#define KILL_RATE_UI_MAX_PLAYER_NUM					(4)				//プレイヤーの最大人数
#define KILL_RATE_UI_NAME_POS						(35.0f)			//名前の位置調整値
#define KILL_RATE_UI_KILL_POS						(375.0f)		//キルの位置調整値
#define KILL_RATE_UI_DEATH_POS						(465.0f)		//デスの位置調整値

//=============================================================================
// コンストラクタ
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
// デストラクタ
//=============================================================================
CKillRateUi::~CKillRateUi()
{
	
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CKillRateUi::Init(void)
{
	//変数初期化
	m_nRank = 1;
	m_bSetPlayerNum = false;

	SetObjType(CObject::OBJTYPE::UI);

	//フレームの生成
	CObject2D *pFrame = nullptr;
	pFrame = CObject2D::Create(D3DXVECTOR3(m_pos.x - KILL_RATE_UI_FRAME_DEFAULT_SIZE_X / 2.0f, m_pos.y, m_pos.z),
	                           { KILL_RATE_UI_FRAME_DEFAULT_SIZE_X * m_scale.x, KILL_RATE_UI_FRAME_DEFAULT_SIZE_Y * m_scale.y, 0.0f },
		                       (int)CObject::PRIORITY::UI);
	pFrame->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("result_score_frame_02.png"));
	//原点位置を左にする
	pFrame->SetOriginType(CObject2D::ORIGIN_TYPE::LEFT);
	//原点を動かしたので頂点情報を更新
	pFrame->SetPos(m_pos, pFrame->GetSize());
	//原点の場所を取得して位置に上書き
	m_pos = pFrame->GetPos();

	//キル数UIの生成
	m_pKillCounter = CCounter::Create(D3DXVECTOR3(m_pos.x + (KILL_RATE_UI_KILL_POS * m_scale.x), m_pos.y , 0.0f),
		                              D3DXVECTOR3(KILL_RATE_UI_RANK_DEFAULT_SIZE_X * m_scale.x, KILL_RATE_UI_RANK_DEFAULT_SIZE_Y * m_scale.y, 0.0f),
		                              2, "Number.png");
	//表示するキル数を設定
	m_pKillCounter->SetCounterNum(0);

	//デス数UIの生成
	m_pDeathCounter = CCounter::Create(D3DXVECTOR3(m_pos.x + (KILL_RATE_UI_DEATH_POS * m_scale.x), m_pos.y , 0.0f),
		                               D3DXVECTOR3(KILL_RATE_UI_RANK_DEFAULT_SIZE_X * m_scale.x, KILL_RATE_UI_RANK_DEFAULT_SIZE_Y * m_scale.y, 0.0f),
		                               2, "Number.png");
	//表示するデス数を設定
	m_pDeathCounter->SetCounterNum(0);


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
void CKillRateUi::Uninit(void)
{
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CKillRateUi::Update(void)
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
			m_ranking.push_back({ pData->Player.nNumber ,pData->Player.nKill ,pData->Player.nDeath ,pData->Player.aName[0]});

			for (int nCntEnemy = 0; nCntEnemy < nDataSize; nCntEnemy++)
			{
				//敵のキル数と番号を入れる
				m_ranking.push_back({ data[nCntEnemy].GetCmmuData()->Player.nNumber, data[nCntEnemy].GetCmmuData()->Player.nKill, 
					                  data[nCntEnemy].GetCmmuData()->Player.nDeath ,data[nCntEnemy].GetCmmuData()->Player.aName[0] });
			}
		}
		else
		{
			//プレイヤーのキル数と番号を入れる
			m_ranking[0] = { pData->Player.nNumber, pData->Player.nKill ,pData->Player.nDeath ,pData->Player.aName[0] };
			for (int nCntEnemy = 0; nCntEnemy < nDataSize; nCntEnemy++)
			{
				//敵のキル数と番号を入れる
				m_ranking[nCntEnemy + 1] = { data[nCntEnemy].GetCmmuData()->Player.nNumber, data[nCntEnemy].GetCmmuData()->Player.nKill , 
					                         data[nCntEnemy].GetCmmuData()->Player.nDeath ,data[nCntEnemy].GetCmmuData()->Player.aName[0] };
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
					KillRateRanking rankingBuff = m_ranking[nCntRank];
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

			//表示するデス数を設定
			m_pDeathCounter->SetCounterNum(m_ranking[m_nRank - 1].nDeath);

			//文字列の生成
			CreateLetter(m_nRank - 1);
		}
		else
		{//するなら
			for (int nCnt = 0; nCnt < nRankingDataSize; nCnt++)
			{
				//プレイヤー番号と一致していたら
				if (m_ranking[nCnt].nPlayerNum == pData->Player.nNumber)
				{
					//表示するランキングをこのデータのランキングに設定
					m_nRank = nCnt + 1;
					//表示するキル数を設定
					m_pKillCounter->SetCounterNum(m_ranking[nCnt].nKill);

					//表示するデス数を設定
					m_pDeathCounter->SetCounterNum(m_ranking[nCnt].nDeath);

					//文字列の生成
					CreateLetter(nCnt);
					break;
				}
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CKillRateUi::Draw(void)
{
	
}

//=============================================================================
// 生成
//=============================================================================
CKillRateUi *CKillRateUi::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &scale)
{
	//インスタンスの生成
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
//文字生成処理
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

		// SJIS → wstring
		int iBufferSize = MultiByteToWideChar(CP_ACP,
			0,
			&buf[nCntName],
			-1,
			(wchar_t*)NULL,
			0);

		// バッファの取得
		wchar_t* cpUCS2 = new wchar_t[iBufferSize];

		// SJIS → wstring
		MultiByteToWideChar(CP_ACP,
			0,
			&buf[nCntName],
			-1,
			cpUCS2,
			iBufferSize);

		// stringの生成
		wstring utextbuf(cpUCS2, cpUCS2 + iBufferSize - 1);

		// バッファの破棄
		delete[] cpUCS2;

		conbrt_buf.push_back(utextbuf);
	}

	//文字の破棄
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
