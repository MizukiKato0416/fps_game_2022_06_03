//================================================
//プレイデータ管理処理
//Author:加藤瑞葵
//================================================
#include "play_data.h"

//================================================
//静的メンバ変数宣言
//================================================

//================================================
//デフォルトコンストラクタ
//================================================
CPlayData::CPlayData()
{	
	m_nScore = 0;
	m_pScore = nullptr;
	m_nTime = 0;
}

//================================================
//デストラクタ
//================================================
CPlayData::~CPlayData()
{

}

//================================================
//初期化処理
//================================================
HRESULT CPlayData::Init(void)
{
	return S_OK;
}

//================================================
//終了処理
//================================================
void CPlayData::Uninit(void)
{
	
}