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
		string nType,
		float AddRotate,
		int Synthetic,
		D3DXVECTOR2 TexNum,
		D3DXVECTOR2 TexMove,
		int nAnimCounter,
		D3DXVECTOR2 nSplit,
		ANIMPATTERN AnimPattern);

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
		string nType,
		float AddRotate,
		int Synthetic,
		D3DXVECTOR2 TexNum,
		D3DXVECTOR2 TexMove,
		int nAnimCounter,
		D3DXVECTOR2 nSplit,
		ANIMPATTERN AnimPattern,
		D3DXVECTOR2 TexNum,
		D3DXVECTOR2 TexMove,
		int nAnimCounter,
		D3DXVECTOR2 nSplit,
		ANIMPATTERN AnimPattern);
private:
	D3DXVECTOR2 m_move;
	float m_Angle;			//äpìx
	float m_Vectl;	//	íºê¸ãóó£

	float m_AddAngle;	//âÒì]â¡éZ
};

#endif // !_MOVEMENT_H_
