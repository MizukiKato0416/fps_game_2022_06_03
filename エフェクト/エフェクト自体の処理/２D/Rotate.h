//*****************************************************************************
// (âÒì])ÉGÉtÉFÉNÉgèàóù [Rotate.h]
// Author : ç≤ì°èGó∫
//*****************************************************************************
#ifndef _ROTATE_H_
#define _ROTATE_H_

#include "Effect.h"

class CRotate : public CEffect
{
public:
	CRotate(int nPriority);
	~CRotate();

	HRESULT Init(D3DXVECTOR3 pos,
		D3DXVECTOR2 move,
		D3DCOLORVALUE color,
		D3DCOLORVALUE Mincolor,
		D3DXVECTOR2 Size,
		D3DXVECTOR2 MinSize,
		int nLife,
		int nType,
		float AddRotate,
		int Synthetic);

	void Uninit();
	void Update();
	void Draw();

	static CRotate*Create(D3DXVECTOR3 pos,
		D3DXVECTOR2 move,
		D3DCOLORVALUE color,
		D3DCOLORVALUE Mincolor,
		D3DXVECTOR2 Size,
		D3DXVECTOR2 MinSize,
		int nLife,
		int nType,
		float AddRotate,
		int Synthetic);
private:
	D3DXVECTOR2 m_move;
	float m_Angle;			//äpìx
	float m_Vectl;	//	íºê¸ãóó£

	float m_AddAngle;	//âÒì]â¡éZ
};

#endif // !_MOVEMENT_H_
