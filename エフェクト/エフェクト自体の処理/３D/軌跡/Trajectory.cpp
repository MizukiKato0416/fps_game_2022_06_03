 //=============================================================================
// 軌跡処理 [Trajectory.cpp]
// Author : 佐藤秀亮
//=============================================================================
#include "Trajectory.h"
#include "manager.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CTrajectory::CTrajectory(int nPriority) : CSetMesh::CSetMesh(nPriority)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CTrajectory::~CTrajectory(void)
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CTrajectory::Init(D3DXVECTOR3 Vtxpos1, D3DXVECTOR3 Vtxpos2, D3DXVECTOR3 VtxOldpos1, D3DXVECTOR3 VtxOldpos2, D3DCOLORVALUE color, D3DCOLORVALUE Mincolor, D3DCOLORVALUE Trajectcolor,D3DCOLORVALUE TrajectMincolor,D3DXVECTOR3 Size, D3DXVECTOR3 MinSize, int nTex, int nLife,
	int Synthetic)
{
	CSetMesh::Init(Vtxpos1, Vtxpos2, VtxOldpos1, VtxOldpos2, color, Mincolor, Trajectcolor, TrajectMincolor, Size, MinSize, nTex, nLife, CMeshEffect::TYPE_TRAJECTORY, Synthetic);
	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CTrajectory::Uninit(void)
{
	CSetMesh::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CTrajectory::Update(void)
{
	CSetMesh::Update();
}

//=============================================================================
// 描画
//=============================================================================
void CTrajectory::Draw(void)
{
	CSetMesh::Draw();
}

//=============================================================================
// 作成
//=============================================================================
CTrajectory *CTrajectory::Create(D3DXVECTOR3 Vtxpos1, D3DXVECTOR3 Vtxpos2, D3DXVECTOR3 VtxOldpos1, D3DXVECTOR3 VtxOldpos2, D3DCOLORVALUE color, D3DCOLORVALUE Mincolor, D3DCOLORVALUE Trajectcolor, D3DCOLORVALUE TrajectMincolor, D3DXVECTOR3 Size, D3DXVECTOR3 MinSize, int nTex, int nLife,
	int Synthetic)
{
	CTrajectory *pTrajectory = NULL;
	pTrajectory = new CTrajectory(CManager::PRIORITY_EFFECT);	//メモリ確保

	//NULLチェック
	if (pTrajectory != NULL)
	{
		pTrajectory->Init(Vtxpos1, Vtxpos2, VtxOldpos1, VtxOldpos2, color, Mincolor, Trajectcolor, TrajectMincolor, Size, MinSize, nTex, nLife, Synthetic);
	}

	return pTrajectory;
}
