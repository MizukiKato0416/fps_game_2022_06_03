//=============================================================================
// 球状エフェクト処理 [SphereEffect.cpp]
// Author : 佐藤秀亮
//=============================================================================
#include "SphereEffect.h"
#include "manager.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CSphereEffect::CSphereEffect(int nPriority) : CSphere::CSphere(nPriority)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CSphereEffect::~CSphereEffect(void)
{

}

//=============================================================================
// 初期化
//=============================================================================
HRESULT CSphereEffect::Init(
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
	D3DXVECTOR2 nSplit)
{
	if (SphereType == SPHERE)
	{
		CSphere::Init(pos, rot, fSize, Vtx * 2, Vtx, nTex, nSynthetic, TexNum);
	}
	else if (SphereType == HALF_SPHERE)
	{
		CSphere::Init(pos, rot, fSize, Vtx * 4, Vtx, nTex, nSynthetic, TexNum);
	}
	else
	{
		CSphere::Init(pos, rot, fSize, Vtx * 2, Vtx, nTex, nSynthetic, TexNum);
	}
	m_Color = Color;
	m_AddColor = AddColor;

	m_TexMove = TexMove;

	m_fSize = fSize;
	m_fAddSize = fAddSize;
	m_nLife = nLife;

	m_MaxSplit = nSplit;
	m_PatternSize = D3DXVECTOR2(1.0f / m_MaxSplit.x, 1.0f / m_MaxSplit.y);
	m_nAnimCount = nAnimCounter;
	m_nSetAnimCnt = nAnimCounter;

	SetPos(pos);

	return S_OK;
}

//=============================================================================
// 終了
//=============================================================================
void CSphereEffect::Uninit()
{
	CSphere::Uninit();
}

//=============================================================================
// 更新
//=============================================================================
void CSphereEffect::Update()
{
	D3DXVECTOR3 pos = GetPos();

	m_fSize += m_fAddSize;
	if (m_fSize < 0)
	{
		m_bUninit = true;
	}

	//カラー変更
	m_Color.r += m_AddColor.r;
	m_Color.g += m_AddColor.g;
	m_Color.b += m_AddColor.b;
	m_Color.a += m_AddColor.a;

	//カラーが0以下の時
	if (m_Color.r < 0)
	{
		m_Color.r = 0;
	}
	if (m_Color.g < 0)
	{
		m_Color.g = 0;
	}
	if (m_Color.b < 0)
	{
		m_Color.b = 0;
	}
	if (m_Color.a < 0)
	{
		m_Color.a = 0;
	}

	//カラーが255以上の時
	if (m_Color.r > 255)
	{
		m_Color.r = 255;
	}
	if (m_Color.g > 255)
	{
		m_Color.g = 255;
	}
	if (m_Color.b > 255)
	{
		m_Color.b = 255;
	}
	if (m_Color.a > 255)
	{
		m_Color.a = 255;
	}

	//テクスチャアニメーション
	if (m_nAnimCount >= 0)
	{
		m_nAnimCount--;
		if (m_nAnimCount < 0)
		{
			m_nAnimCount = m_nSetAnimCnt;
			m_nSplit.x++;
			m_nSplit.y++;
		}
		if (m_nSplit > m_MaxSplit)
		{
			m_nSplit.x = 0;
			m_nSplit.y = 0;
		}
	}
	else if(m_nAnimCount <= -1)
	{

	}
	CSphere::SetSize(pos,m_fSize);
	CSphere::SetColor(m_Color);
	CSphere::SetTexUV(m_TexMove);
	CSphere::SetAnimTexUV(D3DXVECTOR2(m_nSplit.x * m_PatternSize.x, m_nSplit.y * m_PatternSize.y));

	CSphere::Update();
}

//=============================================================================
// 描画
//=============================================================================
void CSphereEffect::Draw()
{
	CSphere::Draw();
}

//=============================================================================
// 作成
//=============================================================================
CSphereEffect *CSphereEffect::Create(
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
	D3DXVECTOR2 nSplit)
{
	CSphereEffect *pSphereEffect = new CSphereEffect(CManager::PRIORITY_EFFECT);

	if (pSphereEffect != NULL)
	{
		pSphereEffect->Init(pos, rot, fSize,
			nTex, Color, AddColor,
			nLife, nSynthetic,
			fAddSize, Vtx,
			SphereType,
			TexMove, TexNum,
			nAnimCounter,
			nSplit);
	}

	return pSphereEffect;

}

