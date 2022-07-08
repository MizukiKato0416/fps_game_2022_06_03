//*****************************************************************************
// (移動する)エフェクト処理 [Movement.h]
// Author : 佐藤秀亮
//*****************************************************************************
#ifndef _MOVEMENT_H_
#define _MOVEMENT_H_

#include "Effect.h"

class CMovement : public CEffect
{
public:
	typedef enum
	{
		SHAPE_SQUARE,
		SHAPE_FREE,
		SHAPE_MAX,
	} SHAPE_TYPE;

	CMovement(int nPriority);
	~CMovement();

	HRESULT Init(D3DXVECTOR3 pos,
		D3DXVECTOR2 move,
		D3DCOLORVALUE color,
		D3DCOLORVALUE Mincolor,
		D3DXVECTOR2 Size,
		D3DXVECTOR2 MinSize,
		int nLife,
		int nType,
		D3DXVECTOR2 AddMovement,
		int nSynthetic,
		D3DXVECTOR2 TexNum,
		D3DXVECTOR2 TexMove,
		int nAnimCounter,
		D3DXVECTOR2 nSplit,
		ANIMPATTERN AnimPattern,
		SHAPE_TYPE Shapetype,
		float fHight,
		float HigthPos,
		float HigthSize,
		D3DCOLORVALUE Secondcolor,
		D3DCOLORVALUE SecondMincolor);

	void Uninit();
	void Update();
	void Draw();

	static CMovement*Create(D3DXVECTOR3 pos,
		D3DXVECTOR2 move,
		D3DCOLORVALUE color,
		D3DCOLORVALUE Mincolor,
		D3DXVECTOR2 Size,
		D3DXVECTOR2 MinSize,
		int nLife,
		int nType,
		D3DXVECTOR2 AddMovement,
		int nSynthetic,
		D3DXVECTOR2 TexNum,
		D3DXVECTOR2 TexMove,
		int nAnimCounter,
		D3DXVECTOR2 nSplit,
		ANIMPATTERN AnimPattern,
		SHAPE_TYPE Shapetype,
		float fHight,
		float HigthPos,
		float HigthSize,
		D3DCOLORVALUE Secondcolor,
		D3DCOLORVALUE SecondMincolor
		);
private:
	D3DXVECTOR2 m_move;			//移動
	D3DXVECTOR2 m_Addpos;		//慣性

	SHAPE_TYPE m_ShapeType;	//形
	float m_fHigth;
	float m_HigthPos;
	float m_HigthSize;

	//カラー値
	D3DCOLORVALUE m_SecondColor;
	//減らすカラー値
	D3DCOLORVALUE m_SecondMinColor;

};

#endif // !_MOVEMENT_H_
