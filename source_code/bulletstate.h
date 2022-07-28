//====================================================
//
// プレイヤーのヘッダーファイル(tcp_client.h)
// Author : 羽鳥 太一
//
//====================================================
#ifndef _BULLETSTATE_H_
#define _BULLETSTATE_H_

//-------------------------------
// インクルード
//-------------------------------
#include "main.h"
#include "object2d.h"
#include "counter.h"

//-------------------------------
// Classの定義
//-------------------------------
class CBulletState : public CObject
{
public:
	CBulletState(CObject::PRIORITY Priority = CObject::PRIORITY::UI);	// デフォルトコンストラクタ
	~CBulletState();	// デフォルトデストラクタ
	HRESULT Init(void);	// 初期化処理
	void Uninit(void);	// 終了処理
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