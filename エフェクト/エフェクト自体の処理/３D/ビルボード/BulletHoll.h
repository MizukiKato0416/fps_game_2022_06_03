//=============================================================================
// 弾痕(仮称)処理 [BulletHoll.h]
// Author : 佐藤秀亮
//=============================================================================
#ifndef _BULLETHOLL_H_
#define _BULLETHOLL_H_

#include "main.h"
#include "billEffect.h"

class CBulletHoll : public CBillEffect
{
public:

	CBulletHoll(int nPriority);
	~CBulletHoll();

	HRESULT Init(D3DXVECTOR3 size,
		D3DXVECTOR3 Addsize,
		D3DXVECTOR3 pos,
		D3DCOLORVALUE color,
		D3DCOLORVALUE Mincolor,
		int nLife,
		int nTex,
		D3DXVECTOR2 TexMove,
		D3DXVECTOR2 TexNum,
		int nAnimCounter,
		D3DXVECTOR2 nSplit,
		D3DXVECTOR3 rot,
		int Synthetic,
		ANIMPATTERN AnimPattern);

	void Uninit();
	void Update();
	void Draw();


	static CBulletHoll *Create(D3DXVECTOR3 size,
		D3DXVECTOR3 Addsize,
		D3DXVECTOR3 pos,
		D3DCOLORVALUE color,
		D3DCOLORVALUE Mincolor,
		int nLife,
		int nTex,
		D3DXVECTOR2 TexMove,
		D3DXVECTOR2 TexNum,
		int nAnimCounter,
		D3DXVECTOR2 nSplit,
		D3DXVECTOR3 rot,
		int Synthetic,
		ANIMPATTERN AnimPattern);

private:
	int nLife;	//寿命
	bool bUninit;
	D3DXVECTOR3 m_pos;
	float m_size;		//現在の大きさ

	D3DXVECTOR3 m_pos1;	//出現位置
	D3DXVECTOR3 m_pos2;	//出現位置
	D3DXVECTOR3 m_pos3;	//出現位置
	D3DXVECTOR3 m_pos4;	//出現位置

	D3DXVECTOR3 m_Rot;	//回転
};

#endif