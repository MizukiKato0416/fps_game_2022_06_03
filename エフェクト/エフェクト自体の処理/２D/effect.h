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
	CEffect(PRIORITY priorty = PRIORITY::EFFECT3D);
	virtual ~CEffect();

	virtual HRESULT Init(D3DXVECTOR3 pos, D3DCOLORVALUE color, D3DCOLORVALUE Mincolor, D3DXVECTOR2 Size, D3DXVECTOR2 MinSize, int nLife, string nType , int Synthetic);
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();


protected:
	bool m_bUninit;
	int m_nLife;
<<<<<<< HEAD
	D3DXVECTOR2 m_Size;			//大きさ
	D3DXVECTOR2 m_MinSize;		//大きさ変動

private:

=======
	D3DXVECTOR2 m_MinSize;		//大きさ変動
	ANIMPATTERN m_AnimPattern;	//アニメーションパターンの動き
>>>>>>> parent of e6eedc6 (Merge branch 'main' of https://github.com/MizukiKato0416/fps_game_2022_06_03)

	//カラー値
	D3DCOLORVALUE m_Color;

	//減らすカラー値
	D3DCOLORVALUE m_MinColor;

	D3DXVECTOR2 m_move;			//移動

	int nSynthetic;
};
#endif
