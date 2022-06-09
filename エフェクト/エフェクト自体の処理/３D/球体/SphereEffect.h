//=============================================================================
// 球状エフェクト処理 [SphereEffect.h]
// Author : 佐藤秀亮
//=============================================================================
#ifndef _SPHEREEFFECT_H_
#define _SPHEREEFFECT_H_

#include "Sphere.h"

class CSphereEffect : public CSphere
{
public:

	typedef enum
	{
		SPHERE,
		HALF_SPHERE,
		MAX,
	} SPHERE_TYPE;

	CSphereEffect(int nPriority);	//コンストラクタ
	~CSphereEffect();	//デストラクタ

	HRESULT Init(
		D3DXVECTOR3 pos,
		float rot,
		float fSize,
		int nTex,
		D3DCOLORVALUE Color, D3DCOLORVALUE AddColor,
		int nLife, int nSynthetic,
		float fAddSize, int Vtx,
		SPHERE_TYPE SphereType,
		D3DXVECTOR2 TexMove,
		D3DXVECTOR2 TexNum,
		int nAnimCounter,
		D3DXVECTOR2 nSplit);

	void Uninit();
	void Update();
	void Draw();

	static CSphereEffect * Create(
		D3DXVECTOR3 pos,
		float rot,
		float fSize,
		int nTex,
		D3DCOLORVALUE Color, D3DCOLORVALUE AddColor,
		int nLife, int nSynthetic,
		float fAddSize, int Vtx,
		SPHERE_TYPE SphereType,
		D3DXVECTOR2 TexMove,
		D3DXVECTOR2 TexNum,
		int nAnimCounter,
		D3DXVECTOR2 nSplit);




private:
	D3DCOLORVALUE m_Color;
	D3DCOLORVALUE m_AddColor;
	D3DXVECTOR2 m_TexMove;

	float m_fSize;
	float m_fAddSize;
};
#endif// !_SPHEREEFFECT_H_