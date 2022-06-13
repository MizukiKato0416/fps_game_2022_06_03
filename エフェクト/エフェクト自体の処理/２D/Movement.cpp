//*****************************************************************************
// (移動する)エフェクト処理 [Movement.cpp]
// Author : 佐藤秀亮
//*****************************************************************************
#include "Movement.h"
#include "manager.h"

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CMovement::CMovement(int nPriority) : CEffect(nPriority)
{
	m_move = {};
	m_Addpos = {};
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CMovement::~CMovement()
{

}

//*****************************************************************************
//	初期化
//*****************************************************************************
HRESULT CMovement::Init(D3DXVECTOR3 pos,
	D3DXVECTOR2 move,
	D3DCOLORVALUE color,
	D3DCOLORVALUE Mincolor,
	D3DXVECTOR2 Size,
	D3DXVECTOR2 MinSize,
	int nLife,
	int nType,
	D3DXVECTOR2 AddMovement,
	int nSynthetic)
{
	CEffect::Init(pos, color, Mincolor, Size, MinSize, nLife, nType,nSynthetic);

	m_Addpos = AddMovement;
	m_move = move;
	return S_OK;
}

//*****************************************************************************
//終了
//*****************************************************************************
void CMovement::Uninit()
{
	CEffect::Uninit();
}

//*****************************************************************************
//更新
//*****************************************************************************
void CMovement::Update()
{

	D3DXVECTOR3 pos = GetPosition();

	m_move += m_Addpos;	//移動値に慣性を加算

	pos += D3DXVECTOR3(m_move.x, m_move.y, 0.0f);

	SetPosition(pos);

	CEffect::Update();
}

//*****************************************************************************
//描画
//*****************************************************************************
void CMovement::Draw()
{
	CEffect::Draw();
}

//*****************************************************************************
//作成
//*****************************************************************************
CMovement *CMovement::Create(D3DXVECTOR3 pos,
	D3DXVECTOR2 move,
	D3DCOLORVALUE color,
	D3DCOLORVALUE Mincolor,
	D3DXVECTOR2 Size,
	D3DXVECTOR2 MinSize,
	int nLife,
	int nType,
	D3DXVECTOR2 AddMovement,
	int nSynthetic)
{
	CMovement *pMovement = NULL;
	pMovement = new CMovement(CManager::PRIORITY_EFFECT);		//メモリ確保
	//NULLチェック
	if (pMovement != NULL)
	{
		pMovement->Init(pos, move, color, Mincolor, Size, MinSize, nLife, nType, AddMovement, nSynthetic);
	}

	return pMovement;
}
