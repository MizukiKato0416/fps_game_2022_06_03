//*****************************************************************************
// 回転エフェクトの親処理(Rotate3d.cpp)
// Author : 佐藤秀亮
//*****************************************************************************
#include "Rotate3d.h"
#include "manager.h"

#include "control.h"
#include "straight3d.h"
#include "Trajectory.h"

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CRotate3D::CRotate3D(int nPriority) : CPlane(nPriority)
{

}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CRotate3D::~CRotate3D()
{

}

//*****************************************************************************
//初期化
//*****************************************************************************
HRESULT CRotate3D::Init(D3DXVECTOR3 SetSize,
	D3DXVECTOR3 pos,
	D3DXVECTOR2 Tex,
	D3DXVECTOR3 Size,
	D3DXVECTOR3 AddSize,
	D3DCOLORVALUE Color,
	D3DCOLORVALUE AddColor,
	float Distance,
	float AddDistance,
	float fAngle,
	float fAddAngle,
	int nTex,
	int Synthetic,
	int nLife,
	int nParticleLife,
	int nBuckTime,
	float fActive,
	int AnimPattern,
	EFFECT_TYPE EffectType,
	MOVE_TYPE MoveType)
{
	CPlane::Init(SetSize, pos, Tex);

	m_EffectTime = START;
	m_Size = Size;
	m_AddSize = AddSize;
	m_Color = Color;
	m_MinColor = AddColor;
	m_nDistanse = Distance;
	m_nBuckDistanse = -Distance;

	switch (MoveType)
	{
	case(TYPE_NOMAL):
		m_nAddDistance = AddDistance;
		break;
	case(TYPE_MOVERAND):
		m_nAddDistance = float(rand() % (int)AddDistance) + 1.0f;
		break;
	default:
		m_nAddDistance = AddDistance;
		break;
	}
	m_fAngle = fAngle;
	m_fAddAngle = fAddAngle;
	m_nTex = nTex;
	m_nSynthetic = Synthetic;
	m_nLife = nLife;
	m_ParticleLife = nParticleLife;
	m_nBuckTime = nBuckTime;
	m_fActive = fActive;

	m_fRandAngle = CIRCLE2;
	m_fRandAngle2 = CIRCLE2;
	m_EffectType = EffectType;
	m_PatternAnim = AnimPattern;

	m_pos = D3DXVECTOR3(
		pos.x + m_nDistanse * sinf(m_fRandAngle + m_fAngle) * cosf(m_fRandAngle2 + m_fAngle),
		pos.y + m_nDistanse * cosf(m_fRandAngle + m_fAngle),
		pos.z + m_nDistanse * sinf(m_fRandAngle + m_fAngle) * sinf(m_fRandAngle2 + m_fAngle));;

	m_Oldpos = m_pos;

	SetPos(m_pos);
	return S_OK;
}

//*****************************************************************************
//終了
//*****************************************************************************
void CRotate3D::Uninit()
{
	CPlane::Uninit();
}

//*****************************************************************************
//更新
//*****************************************************************************
void CRotate3D::Update()
{
	D3DXVECTOR3 pos = GetPos();


	switch (m_EffectTime)
	{
	case(START):
		m_nDistanse += m_nAddDistance;

		if (m_nDistanse > m_fActive)
		{
			m_EffectTime = ACTIVE;
		}
		break;
	case(ACTIVE):
		m_nBuckTime--;
		if (m_nBuckTime < 0)
		{
			m_EffectTime = END;
		}
		break;
	case(END):
		m_nDistanse -= m_nAddDistance;
		if (m_nDistanse < 0)
		{
			m_bUninit = true;
		}

		break;
	}

	m_fAngle += m_fAddAngle;

	m_pos = D3DXVECTOR3(
		pos.x + m_nDistanse * sinf(m_fRandAngle + m_fAngle) * cosf(m_fRandAngle2 + m_fAngle),
		pos.y + m_nDistanse * cosf(m_fRandAngle + m_fAngle),
		pos.z + m_nDistanse * sinf(m_fRandAngle + m_fAngle) * sinf(m_fRandAngle2 + m_fAngle));


	if (m_nDistanse < 0)
	{
		m_bUninit = true;
	}
	switch(m_EffectType)
	{
	case(TYPE_PARTICLE):
		CStraight3D::Create(m_pos,
			m_Size,
			m_AddSize,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			m_Color,
			m_MinColor,
			m_nTex, m_ParticleLife,
			CStraight3D::STRAIGHT, {}, m_nSynthetic,
			0.0f,
			(CStraight3D::RAND_PATTEN)0,
			(CStraight3D::POS_PATTERN)3,
			D3DXVECTOR2(0.0f, 0.0f),
			D3DXVECTOR2(1.0f, 1.0f),
			0,
			D3DXVECTOR2(1.0f, 1.0f),
			(CBillEffect::ANIMPATTERN)m_PatternAnim);
		break;
	case(TYPE_TRAJECT):
		CTrajectory::Create(
			D3DXVECTOR3(m_pos.x, m_pos.y + m_Size.x, m_pos.z),
			D3DXVECTOR3(m_pos.x, m_pos.y - m_Size.x, m_pos.z),
			D3DXVECTOR3(m_Oldpos.x, m_Oldpos.y + m_Size.x, m_Oldpos.z),
			D3DXVECTOR3(m_Oldpos.x, m_Oldpos.y - m_Size.x, m_Oldpos.z),
			m_Color,
			m_MinColor,
			m_Color,
			m_MinColor,
			m_Size,
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			m_nTex,
			m_ParticleLife,
			m_nSynthetic
		);
		break;
	}

	m_Oldpos = m_pos;
	m_nLife--;
	if (m_nLife < 0)
	{
		m_bUninit = true;
	}

	if (m_bUninit == true)
	{
		Uninit();
	}
}


//*****************************************************************************
//作成
//*****************************************************************************
CRotate3D *CRotate3D::Create(D3DXVECTOR3 SetSize,
	D3DXVECTOR3 pos,
	D3DXVECTOR2 Tex,
	D3DXVECTOR3 Size,
	D3DXVECTOR3 AddSize,
	D3DCOLORVALUE Color,
	D3DCOLORVALUE AddColor,
	float Distance,
	float AddDistance,
	float fAngle,
	float fAddAngle,
	int nTex,
	int Synthetic,
	int nLife,
	int nParticleLife,
	int nBuckTime,
	float fActive,
	int AnimPattern,
	EFFECT_TYPE EffectType,
	MOVE_TYPE MoveType)
{
	CRotate3D * pRotate3D = NULL;
	pRotate3D = new CRotate3D(CManager::PRIORITY_EFFECT);
	if (pRotate3D != NULL)
	{
		pRotate3D->Init(SetSize,
			pos, Tex,
			Size,
			AddSize,
			Color,
			AddColor,
			Distance,
			AddDistance,
			fAngle,
			fAddAngle,
			nTex, Synthetic, nLife,
			nParticleLife,
			nBuckTime,
			fActive,
			AnimPattern,
			EffectType,
			MoveType);
	}
	return pRotate3D;
}
