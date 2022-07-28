//=============================================================================
// 3D噴水エフェクト処理 [Fountain.cpp]
// Author : 佐藤秀亮
//=============================================================================
#include "Fountain.h"
#include "manager.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CFountain::CFountain(int nPriority) : CBillEffect(nPriority)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CFountain::~CFountain()
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CFountain::Init(D3DXVECTOR3 pos,
	D3DXVECTOR3 Size,
	D3DXVECTOR3 MinSize,
	D3DCOLORVALUE color,
	D3DCOLORVALUE Mincolor,
	int nTex, int nLife,
	D3DXVECTOR2 TexNum,
	D3DXVECTOR2 TexMove,
	int nAnimCounter,
	D3DXVECTOR2 nSplit,
	ANIMPATTERN AnimPattern,
	D3DXVECTOR3 move,
	D3DXVECTOR3 Target,
	int Diffusion,
	int Synhetic,
	HIGHT_PATTERN HigthPattrn)
{
	CBillEffect::Init(Size, MinSize, color, Mincolor, nTex, nLife, TexNum, TexMove, nAnimCounter, nSplit, AnimPattern);

	D3DXVECTOR3 T = Target;
	D3DXVECTOR3 V = pos - Target;



	m_fAngle = atan2(V.x, V.z);

	//ランダム拡散率
	int Lowest = Diffusion;
	if (Lowest <= 0)
	{
		Lowest = 1;
	}
	float randAngle = float(rand() % Lowest) - float(rand() % Lowest);
	randAngle /= 100.0f;
	m_fAngle += randAngle;

	if (HigthPattrn == HIGHT_RAND)
	{
		move.y * 100;

		float randHight = float(rand() % (int)move.y) - float(rand() % (int)move.y);
		move.y = randHight;

		m_move = move;
	}
	else
	{
		m_move = move;
	}
	m_pos = pos;

	m_nSynthenic = Synhetic;
	SetPos(m_pos);
	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CFountain::Uninit()
{
	CBillEffect::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CFountain::Update()
{
	m_pos += D3DXVECTOR3(sinf(m_fAngle) *m_move.x, m_move.y, cosf(m_fAngle) * m_move.x);

	m_move.y += m_move.z;

	SetPos(m_pos);
	CBillEffect::Update();
}

//=============================================================================
// 描画
//=============================================================================
void CFountain::Draw()
{
	CBillEffect::Draw();
}

//=============================================================================
// 作成
//=============================================================================
CFountain *CFountain::Create(D3DXVECTOR3 pos,
	D3DXVECTOR3 Size,
	D3DXVECTOR3 MinSize,
	D3DCOLORVALUE color,
	D3DCOLORVALUE Mincolor,
	int nTex, int nLife,
	D3DXVECTOR2 TexNum,
	D3DXVECTOR2 TexMove,
	int nAnimCounter,
	D3DXVECTOR2 nSplit,
	ANIMPATTERN AnimPattern,
	D3DXVECTOR3 move,
	D3DXVECTOR3 Target,
	int Diffusion,
	int Synhetic,
	HIGHT_PATTERN HigthPattrn)
{
	CFountain *pFountain = new CFountain(CManager::PRIORITY_EFFECT);

	if (pFountain != NULL)
	{
		pFountain->Init(pos, Size, MinSize, color, Mincolor, nTex, nLife, TexNum,
			TexMove, nAnimCounter, nSplit,
			AnimPattern, move, Target,
			Diffusion, Synhetic, HigthPattrn);
	}

	return pFountain;
}