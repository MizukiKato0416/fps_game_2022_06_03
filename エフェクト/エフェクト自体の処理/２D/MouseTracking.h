//*****************************************************************************
// (マウスに向かって飛んでく)エフェクト処理 [MouseTracking.h]
// Author : 佐藤秀亮
//*****************************************************************************
#ifndef _MOUSETRACKING_H_
#define _MOUSETRACKING_H_

#include "effect.h"

class CMouseTracking : public CEffect
{
public:
	CMouseTracking(PRIORITY priorty = PRIORITY::EFFECT3D);
	~CMouseTracking();

	HRESULT Init(D3DXVECTOR3 pos,
		D3DXVECTOR2 move,
		D3DCOLORVALUE color,
		D3DCOLORVALUE Mincolor,
		D3DXVECTOR2 Size,
		D3DXVECTOR2 MinSize,
		int nLife, string nType,
		D3DXVECTOR3 Endpos,
		int Diffusion,
		int UninitVectl,
		int Synthetic,
		int Distance,
		D3DXVECTOR3 Playerpos,
		D3DXVECTOR3 rot,
		D3DXVECTOR2 TexNum,
		D3DXVECTOR2 TexMove,
		int nAnimCounter,
		D3DXVECTOR2 nSplit,
		ANIMPATTERN AnimPattern);

	void Uninit();
	void Update();
	void Draw();

	static CMouseTracking *Create(D3DXVECTOR3 pos,
		D3DXVECTOR2 move,
		D3DCOLORVALUE color,
		D3DCOLORVALUE Mincolor,
		D3DXVECTOR2 Size,
		D3DXVECTOR2 MinSize,
		int nLife, string nType,
		D3DXVECTOR3 Endpos,
		int Diffusion,
		int UninitVectl,
		int Synthetic,
		int Distance,
		D3DXVECTOR3 Playerpos,
		D3DXVECTOR3 rot,
		D3DXVECTOR2 TexNum,
		D3DXVECTOR2 TexMove,
		int nAnimCounter,
		D3DXVECTOR2 nSplit,
		ANIMPATTERN AnimPattern);

private:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR2 m_move;
	D3DXVECTOR3 m_Vec;
	D3DXVECTOR3 m_Endpos;	//出現位置
	D3DXVECTOR3 m_PlayerPos;	//現在位置
	D3DXVECTOR3 m_SerectRot;

	float m_fAngle;
	float m_Vectl;
	int m_UninitVectl;
	float m_Distance;
};

#endif // !_MOUSETRACKING_H_
