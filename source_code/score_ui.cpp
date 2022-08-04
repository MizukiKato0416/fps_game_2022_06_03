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

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
#define SCORE_UI_FRAME_DEFAULT_SIZE_X	(304.0f)		//フレームのデフォルトサイズ
#define SCORE_UI_FRAME_DEFAULT_SIZE_Y	(42.0f)			//フレームのデフォルトサイズ
#define SCORE_UI_RANK_DEFAULT_SIZE_X	(35.0f)			//ランクのデフォルトサイズ
#define SCORE_UI_RANK_DEFAULT_SIZE_Y	(35.0f)			//ランクのデフォルトサイズ

//=============================================================================
// デフォルトコンストラクタ
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
// デフォルトデストラクタ
//=============================================================================
CScoreUi::~CScoreUi()
{
	
}

//=============================================================================
// モデルの初期化処理
//=============================================================================
HRESULT CScoreUi::Init(void)
{
	//変数初期化
	m_nRank = 1;

	SetObjType(CObject::OBJTYPE::UI);

	//フレームの生成
	CObject2D *pFrame = nullptr;
	pFrame = CObject2D::Create(D3DXVECTOR3(m_pos.x - SCORE_UI_FRAME_DEFAULT_SIZE_X / 2.0f, m_pos.y, m_pos.z),
	                           { SCORE_UI_FRAME_DEFAULT_SIZE_X * m_scale.x, SCORE_UI_FRAME_DEFAULT_SIZE_Y * m_scale.y, 0.0f },
		                       (int)CObject::PRIORITY::UI);
	pFrame->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("score_frame.png"));
	//原点位置を左にする
	pFrame->SetOriginType(CObject2D::ORIGIN_TYPE::LEFT);

	//順位UIの生成
	m_pRankCounter = CCounter::Create(D3DXVECTOR3(m_pos.x + (10.0f * m_scale.x) , m_pos.y , 0.0f),
		                              D3DXVECTOR3(SCORE_UI_RANK_DEFAULT_SIZE_X, SCORE_UI_RANK_DEFAULT_SIZE_Y, 0.0f), 1, "Number.png");
	//表示する順位を指定
	m_pRankCounter->SetCounterNum(m_nRank);


	//名前UIの生成


	//キル数UIの生成
	m_pKillCounter = CCounter::Create(D3DXVECTOR3(m_pos.x + (250.0f * m_scale.x) , m_pos.y , 0.0f),
		                              D3DXVECTOR3(SCORE_UI_RANK_DEFAULT_SIZE_X, SCORE_UI_RANK_DEFAULT_SIZE_Y, 0.0f), 2, "Number.png");
	//表示するキル数を設定
	m_pKillCounter->SetCounterNum(m_nRank);


	return S_OK;
}

//=============================================================================
// モデルの終了処理
//=============================================================================
void CScoreUi::Uninit(void)
{
	Release();
}

//=============================================================================
// モデルの更新処理
//=============================================================================
void CScoreUi::Update(void)
{
	
}

//=============================================================================
// モデルの描画処理
//=============================================================================
void CScoreUi::Draw(void)
{
	
}

//=============================================================================
// モデルの生成
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