//=============================================================================
// ãOê’èàóù [Trajectory.h]
// Author : ç≤ì°èGó∫
//=============================================================================

#include "SetMesh.h"

class CTrajectory : public CSetMesh
{
public:
	CTrajectory(int nPriority);
	~CTrajectory();

	HRESULT Init(D3DXVECTOR3 Vtxpos1,
		D3DXVECTOR3 Vtxpos2,
		D3DXVECTOR3 VtxOldpos1,
		D3DXVECTOR3 VtxOldpos2,
		D3DCOLORVALUE color,
		D3DCOLORVALUE Mincolor,
		D3DCOLORVALUE Trajectcolor,
		D3DCOLORVALUE TrajectMincolor,
		D3DXVECTOR3 Size,
		D3DXVECTOR3 MinSize,
		int nTex,
		int nLife,
		int Synthetic);

	void Uninit();
	void Update();
	void Draw();

	static CTrajectory *Create(D3DXVECTOR3 Vtxpos1, 
		D3DXVECTOR3 Vtxpos2, 
		D3DXVECTOR3 VtxOldpos1, 
		D3DXVECTOR3 VtxOldpos2, 
		D3DCOLORVALUE color, 
		D3DCOLORVALUE Mincolor, 
		D3DCOLORVALUE Trajectcolor,
		D3DCOLORVALUE TrajectMincolor, 
		D3DXVECTOR3 Size, 
		D3DXVECTOR3 MinSize,
		int nTex,
		int nLife,
		int Synthetic);
private:

};