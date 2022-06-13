//*****************************************************************************
// (回転)エフェクト処理 [Rotate.cpp]
// Author : 佐藤秀亮
//*****************************************************************************
#include "Rotate.h"
#include "manager.h"

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CRotate::CRotate(int nPriority) : CEffect(nPriority)
{
	m_Angle = {};
	m_AddAngle = {};
	m_move = {};
	m_Vectl = {};
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CRotate::~CRotate()
{

}

//*****************************************************************************
//	初期化
//*****************************************************************************
HRESULT CRotate::Init(D3DXVECTOR3 pos,
	D3DXVECTOR2 move,
	D3DCOLORVALUE color,
	D3DCOLORVALUE Mincolor,
	D3DXVECTOR2 Size,
	D3DXVECTOR2 MinSize,
	int nLife,
	int nType,
	float AddRotate,
	int Synthetic)
{
	CEffect::Init(pos, color, Mincolor, Size, MinSize, nLife, nType,Synthetic);
	m_Size = Size;
	m_MinSize = MinSize;
	m_Angle = 0;
	m_Vectl = sqrtf(m_Size.x * m_Size.x + m_Size.y * m_Size.y);
	m_move = move;
	m_AddAngle = AddRotate;

	return S_OK;
}

//*****************************************************************************
//終了
//*****************************************************************************
void CRotate::Uninit()
{
	CEffect::Uninit();
}

//*****************************************************************************
//更新
//*****************************************************************************
void CRotate::Update()
{
	D3DXVECTOR3 pos = GetPosition();
	m_Vectl = sqrtf(m_Size.x * m_Size.x + m_Size.y * m_Size.y);
	pos += D3DXVECTOR3(m_move.x, m_move.y, 0.0f);
	m_Angle += m_AddAngle;
	CScene2D::SetRotate(pos, m_Angle, -m_Angle, m_Vectl);


	CEffect::Update();
}

//*****************************************************************************
//描画
//*****************************************************************************
void CRotate::Draw()
{
	CEffect::Draw();
}

//*****************************************************************************
//作成
//*****************************************************************************
CRotate *CRotate::Create(D3DXVECTOR3 pos,
	D3DXVECTOR2 move,
	D3DCOLORVALUE color,
	D3DCOLORVALUE Mincolor,
	D3DXVECTOR2 Size,
	D3DXVECTOR2 MinSize,
	int nLife,
	int nType,
	float AddRotate,
	int Synthetic)
{
	CRotate *pRotate = NULL;
	pRotate = new CRotate(CManager::PRIORITY_EFFECT);		//メモリ確保
																//NULLチェック
	if (pRotate != NULL)
	{
		pRotate->Init(pos, move, color, Mincolor, Size, MinSize, nLife, nType, AddRotate,Synthetic);
	}

	return pRotate;
}
