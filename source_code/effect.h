//*****************************************************************************
// �G�t�F�N�g���� [Effect.h]
// Author : �����G��
//*****************************************************************************

#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "Scene2D.h"

class CEffect : public CScene2D
{
public:
	CEffect(PRIORITY priorty = PRIORITY::EFFECT2D);
	virtual ~CEffect();

	virtual HRESULT Init(D3DXVECTOR3 pos, D3DCOLORVALUE color, D3DCOLORVALUE Mincolor, D3DXVECTOR2 Size, D3DXVECTOR2 MinSize, int nLife, string nType , int Synthetic);
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();


protected:
	bool m_bUninit;
	int m_nLife;
	D3DXVECTOR2 m_Size;			//�傫��
	D3DXVECTOR2 m_MinSize;		//�傫���ϓ�

private:


	//�J���[�l
	D3DCOLORVALUE m_Color;

	//���炷�J���[�l
	D3DCOLORVALUE m_MinColor;

	D3DXVECTOR2 m_move;			//�ړ�

	int nSynthetic;
};
#endif
