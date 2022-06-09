//=============================================================================
// É|ÉäÉSÉìÇÃê›íË [SetMesh.h]
// Author : ç≤ì°èGó∫
//=============================================================================
#ifndef _SETMESH_H_
#define _SETMESH_H_

#include "MeshEffect.h"

class CSetMesh : public CMeshEffect
{
public:
	CSetMesh(int nPriority = 3);
	virtual ~CSetMesh();

	virtual HRESULT Init(D3DXVECTOR3 Vtxpos1, D3DXVECTOR3 Vtxpos2,
		D3DXVECTOR3 VtxOldpos1, D3DXVECTOR3 VtxOldpos2,
		D3DCOLORVALUE color,D3DCOLORVALUE Mincolor,
		D3DCOLORVALUE Trajectcolor, D3DCOLORVALUE TrajectMincolor,
		D3DXVECTOR3 Size, D3DXVECTOR3 MinSize,
		int nTex, int nLife,
		EFFECT_TYPE nType,
		int Synthetic);

	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

private:

	D3DCOLORVALUE m_Color;
	D3DCOLORVALUE m_MinColor;

	D3DCOLORVALUE m_TrajectColor;
	D3DCOLORVALUE m_TrajectMinColor;

	D3DXVECTOR3 m_move;

	D3DXVECTOR3 m_Size;
	D3DXVECTOR3 m_MinSize;

};
#endif // ! _SETMESH_H_
