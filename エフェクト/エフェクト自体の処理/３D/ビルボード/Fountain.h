//=============================================================================
// 3D噴水エフェクト処理 [Fountain.h]
// Author : 佐藤秀亮
//=============================================================================
#ifndef _FOUNTAIN_H_
#define _FOUNTAIN_H_

#include "main.h"
#include "billEffect.h"

class CFountain : public CBillEffect
{
public:
	typedef enum
	{
		HIGHT_NOMAL,
		HIGHT_RAND,
		HIGHT_MAX,
	} HIGHT_PATTERN;


	CFountain(int nPriority);
	~CFountain();

	HRESULT Init(D3DXVECTOR3 pos,
		D3DXVECTOR3 Size,
		D3DXVECTOR3 MinSize,
		D3DCOLORVALUE color,
		D3DCOLORVALUE Mincolor,
		int nTex, int nLife,
		D3DXVECTOR2 TexNum,
		D3DXVECTOR2 TexMove,
		int nAnimCounter,
		D3DXVECTOR2 nSplit,
		ANIMPATTERN AnimPattern,
		D3DXVECTOR3 move,
		D3DXVECTOR3 Target,
		int Diffusion,
		int Synhetic,
		HIGHT_PATTERN HigthPattrn);

	void Uninit();
	void Update();
	void Draw();

	static CFountain *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 Size,
		D3DXVECTOR3 MinSize,
		D3DCOLORVALUE color,
		D3DCOLORVALUE Mincolor,
		int nTex, int nLife,
		D3DXVECTOR2 TexNum,
		D3DXVECTOR2 TexMove,
		int nAnimCounter,
		D3DXVECTOR2 nSplit,
		ANIMPATTERN AnimPattern,
		D3DXVECTOR3 move,
		D3DXVECTOR3 Target,
		int Diffusion,
		int Synhetic,
		HIGHT_PATTERN HigthPattrn);

private:
	D3DXVECTOR3 m_pos;	//出現位置
	D3DXVECTOR3 m_move;	//移動値
	float m_fAngle;
};
#endif