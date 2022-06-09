//=============================================================================
// (フィールドのビルボード版)3Dエフェクト処理 [ActiveBillboard.h]
// Author : 佐藤秀亮
//=============================================================================
#include "ActiveBillboard.h"
#include "manager.h"


//=============================================================================
// コンストラクタ
//=============================================================================
CActiveBillBoard::CActiveBillBoard(int nPriority) : CBillEffect(nPriority)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CActiveBillBoard::~CActiveBillBoard()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CActiveBillBoard::Init(D3DXVECTOR3 pos,
	D3DXVECTOR3 Size,
	D3DXVECTOR3 MinSize,
	D3DXVECTOR3 move,
	D3DCOLORVALUE color,
	D3DCOLORVALUE Mincolor,
	int nType,
	int nLife,
	int Synsetic,
	int FieldTime,
	float MaxSize,
	float AddSize,
	bool PresetCreate,
	int nPattern,
	D3DXVECTOR2 TexMove,
	D3DXVECTOR2 TexNum,
	int nAnimCounter,
	D3DXVECTOR2 nSplit,
	ANIMPATTERN AnimPattern)
{
	CBillEffect::Init(Size, MinSize, color, Mincolor, nType, nLife, TexNum, TexMove, nAnimCounter, nSplit, AnimPattern);
	m_nSynthenic = Synsetic;
	m_CreatePreset = nPattern;
	m_MinSize = MinSize;
	m_fSize = sqrtf(Size.x * Size.x + Size.y * Size.y);

	m_MaxSize = MaxSize;
	m_AddSize = AddSize;
	m_move = move;
	m_PresetCreate = PresetCreate;
	m_ActiveTime = FieldTime;
	EffectTime = START;

	SetPos(pos);
	BillboardSize(m_fSize);

	return S_OK;
}


//=============================================================================
// 終了
//=============================================================================
void CActiveBillBoard::Uninit()
{
	CBillEffect::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CActiveBillBoard::Update()
{
	D3DXVECTOR3 pos = GetPos();


	switch (EffectTime)
	{
	case(START):
		pos += m_move;

		m_fSize += m_AddSize;
		if (m_fSize > m_MaxSize)
		{
			EffectTime = ACTIVE;
		}
		break;
	case(ACTIVE):
		if (m_fSize < 0)
		{
			m_fSize = 0;
		}
		m_ActiveTime--;
		if (m_ActiveTime < 0)
		{
			EffectTime = END;
		}
		break;
	case(END):
		m_fSize -= m_AddSize;
		if (m_fSize < 10)
		{
			m_bUninit = true;
		}
		break;
	default:
		break;
	}


	SetPos(pos);
	BillboardSize(m_fSize);

	CBillEffect::Update();
}


//=============================================================================
// 描画
//=============================================================================
void CActiveBillBoard::Draw()
{
	CBillEffect::Draw();
}

//=============================================================================
// 作成
//=============================================================================
CActiveBillBoard *CActiveBillBoard::Create(D3DXVECTOR3 pos,
	D3DXVECTOR3 Size,
	D3DXVECTOR3 MinSize,
	D3DXVECTOR3 move,
	D3DCOLORVALUE color,
	D3DCOLORVALUE Mincolor,
	int nType,
	int nLife,
	int Synsetic,
	int FieldTime,
	float MaxSize,
	float AddSize,
	bool PresetCreate,
	int nPattern,
	D3DXVECTOR2 TexMove,
	D3DXVECTOR2 TexNum,
	int nAnimCounter,
	D3DXVECTOR2 nSplit,
	ANIMPATTERN AnimPattern)
{
	CActiveBillBoard *pStraight3D = new CActiveBillBoard(CManager::PRIORITY_EFFECT);

	if (pStraight3D != NULL)
	{
		pStraight3D->Init(pos, Size, MinSize, move, color, Mincolor, nType,
			nLife, Synsetic,
			FieldTime, MaxSize,
			AddSize,
			PresetCreate,
			nPattern,
			TexMove,
			TexNum,
			nAnimCounter,
			nSplit,
			AnimPattern);
	}

	return pStraight3D;

}
