//====================================================
//
// �R�~���j�P�[�V�����f�[�^�̃w�b�_�[�t�@�C��(communicationdata.cpp)
// Author : �H�� ����
//
//====================================================
//=============================================================================
// �C���N���[�h
//=============================================================================
#include "bulletstate.h"
#include "manager.h"
#include "counter.h"

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CBulletState::CBulletState(CObject::PRIORITY Priority) : CObject(Priority)
{

}

//=============================================================================
// �f�t�H���g�f�X�g���N�^
//=============================================================================
CBulletState::~CBulletState()
{

}

//=============================================================================
// ����������
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
// ����������
//=============================================================================
void CBulletState::Uninit(void)
{
	
}

//=============================================================================
// ����������
//=============================================================================
void CBulletState::Update(void)
{

}

//=============================================================================
// ����������
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