//*****************************************************************************
// エフェクト処理 [Effect.h]
// Author : 佐藤秀亮
//*****************************************************************************

#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "Scene2D.h"

class CEffect : public CScene2D
{
public:
	typedef enum
	{
		ANIMPATTERN_NOMAL,
		ANIMPATTERN_RAND,
		ANIMPATTERN_MAX,
	} ANIMPATTERN;

	CEffect(PRIORITY priorty = PRIORITY::EFFECT3D);
	virtual ~CEffect();

	virtual HRESULT Init(D3DXVECTOR3 pos,
		D3DCOLORVALUE color,
		D3DCOLORVALUE Mincolor,
		D3DXVECTOR2 Size,
		D3DXVECTOR2 MinSize,
		int nLife, string nType,
		int Synthetic,
		D3DXVECTOR2 TexNum,
		D3DXVECTOR2 TexMove,
		int nAnimCounter,
		D3DXVECTOR2 nSplit,
		ANIMPATTERN AnimPattern);

	virtual void Uninit();
	virtual void Update();
	virtual void Draw();


protected:
	bool m_bUninit;
	int m_nLife;
	D3DXVECTOR2 m_MinSize;		//大きさ変動
	ANIMPATTERN m_AnimPattern;	//アニメーションパターンの動き

								//カラー値
	D3DCOLORVALUE m_Color;

	//減らすカラー値
	D3DCOLORVALUE m_MinColor;

private:
	D3DXVECTOR2 m_TexMove;

	D3DXVECTOR2 m_move;			//移動

	int m_nSynthetic;
};
#endif
