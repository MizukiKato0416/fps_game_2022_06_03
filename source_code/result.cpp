//================================================
//リザルト処理
//Author: 加藤瑞葵
//================================================
#include "result.h"
#include "manager.h"
#include "fade.h"
#include "object2D.h"

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
	m_Ui.push_back(CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 0.0f + (100.0f * 2.0f), 0.0f), D3DXVECTOR3(600.0f, 100.0f, 0.0f), (int)CObject::PRIORITY::UI));
	m_Ui[0]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture(""));

	m_Ui.push_back(CObject2D::Create(D3DXVECTOR3((SCREEN_WIDTH / 2) - 100.0f, 0.0f + (65.0f * 5.0f), 0.0f), D3DXVECTOR3(166.0f, 65.0f, 0.0f), (int)CObject::PRIORITY::UI));
	m_Ui[1]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture(""));
	m_Ui.push_back(CObject2D::Create(D3DXVECTOR3((SCREEN_WIDTH / 2) - 100.0f, 0.0f + (65.0f * 6.0f), 0.0f), D3DXVECTOR3(166.0f, 65.0f, 0.0f), (int)CObject::PRIORITY::UI));
	m_Ui[2]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture(""));
	m_Ui.push_back(CObject2D::Create(D3DXVECTOR3((SCREEN_WIDTH / 2) - 100.0f, 0.0f + (65.0f * 7.0f), 0.0f), D3DXVECTOR3(166.0f, 65.0f, 0.0f), (int)CObject::PRIORITY::UI));
	m_Ui[3]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture(""));
	m_Ui.push_back(CObject2D::Create(D3DXVECTOR3((SCREEN_WIDTH / 2) - 100.0f, 0.0f + (65.0f * 8.0f), 0.0f), D3DXVECTOR3(166.0f, 65.0f, 0.0f), (int)CObject::PRIORITY::UI));
	m_Ui[4]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture(""));
	
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

}

//================================================
//描画処理
//================================================
void CResult::Draw(void)
{
	
}
