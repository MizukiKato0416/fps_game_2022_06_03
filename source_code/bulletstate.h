//====================================================
//
// �v���C���[�̃w�b�_�[�t�@�C��(tcp_client.h)
// Author : �H�� ����
//
//====================================================
#ifndef _BULLETSTATE_H_
#define _BULLETSTATE_H_

//-------------------------------
// �C���N���[�h
//-------------------------------
#include "main.h"
#include "object2d.h"
#include "counter.h"

//-------------------------------
// Class�̒�`
//-------------------------------
class CBulletState : public CObject
{
public:
	CBulletState(CObject::PRIORITY Priority = CObject::PRIORITY::UI);	// �f�t�H���g�R���X�g���N�^
	~CBulletState();	// �f�t�H���g�f�X�g���N�^
	HRESULT Init(void);	// ����������
	void Uninit(void);	// �I������
	void Update(void);
	void Draw(void);
	static CBulletState *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void SetBulletMax(int state) { m_bullet_state[1]->SetCounterNum(state); }
	void AddBulletNow(int state) { m_bullet_state[0]->AddCounter(state); }
	void SetBulletNow(int state) { m_bullet_state[0]->SetCounterNum(state); }
	void SetCol(D3DXCOLOR col) { m_bullet_state[0]->SetCol(col); }
private:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_size;
	CObject2D *m_state_frame;
	vector<CCounter*> m_bullet_state;
};

#endif // _TCP_CLIENT_H_