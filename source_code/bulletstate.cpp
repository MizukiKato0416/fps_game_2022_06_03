//====================================================
//
// コミュニケーションデータのヘッダーファイル(communicationdata.cpp)
// Author : 羽鳥 太一
//
//====================================================
//=============================================================================
// インクルード
//=============================================================================
#include "bulletstate.h"
#include "manager.h"
#include "counter.h"

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CBulletState::CBulletState(CObject::PRIORITY Priority) : CObject(Priority)
{

}

//=============================================================================
// デフォルトデストラクタ
//=============================================================================
CBulletState::~CBulletState()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBulletState::Init(void)
{
	m_state_frame = CObject2D::Create(m_pos, m_size, static_cast<int>(CObject::PRIORITY::UI));
	m_state_frame->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("bullet_state.png"));

	m_bullet_state.push_back(CCounter::Create(D3DXVECTOR3(m_pos.x - 55.0f, m_pos.y - 10.0f, 0.0f), D3DXVECTOR3(50.0f, 50.0f, 0.0f), 2, "Number.png"));
	m_bullet_state.push_back(CCounter::Create(D3DXVECTOR3(m_pos.x + 55.0f, m_pos.y + 10.0f, 0.0f), D3DXVECTOR3(50.0f, 50.0f, 0.0f), 2, "Number.png"));

	return S_OK;
}

//=============================================================================
// 初期化処理
//=============================================================================
void CBulletState::Uninit(void)
{
	
}

//=============================================================================
// 初期化処理
//=============================================================================
void CBulletState::Update(void)
{

}

//=============================================================================
// 初期化処理
//=============================================================================
void CBulletState::Draw(void)
{
	
}

CBulletState *CBulletState::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CBulletState *bullet_state = new CBulletState;

	bullet_state->m_pos = pos;
	bullet_state->m_size = size;

	bullet_state->Init();

	return bullet_state;
}