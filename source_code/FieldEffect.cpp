//=============================================================================
// 平面エフェクト処理 [FieldEffect.cpp]
// Author : 佐藤秀亮
//=============================================================================
#include "FieldEffect.h"
#include "manager.h"
#include "renderer.h"
#include "control.h"

#include "straight3d.h"
#include "PresetSetEffect.h"

CFieldEffect::CFieldEffect(PRIORITY priorty) : CPlane::CPlane(priorty)
{

}

CFieldEffect::~CFieldEffect()
{

}

//初期化処理
HRESULT CFieldEffect::Init(D3DXVECTOR3 size,
	D3DXVECTOR3 pos,
	D3DCOLORVALUE FieldColor,
	D3DCOLORVALUE FieldAddColor,
	float Rotate,
	int nTime,
	float MaxSize,
	float AddSize,
	int nSynsetic,
	int nDensity,
	float fDistance,
	D3DCOLORVALUE ParticleColor,
	D3DCOLORVALUE ParticleAddColor,
	int nParticleTex,
	int nParticleLife,
	int nParticleSynthetic,
	float ParticleMove3d,
	float ParticleAddSize,
	float ParticleSize,
	int ParticleTime,
	float ActiveAddSize,
	int FieldTime,
	bool FieldCreate,
	int CreatePreset,
	int AnimPatternType)
{
	CPlane::Init();
	CObject::SetObjType(CObject::OBJTYPE::EFFECT_3D);

	nLife = 600;

	EffectTime = START;

	m_Oliginpos = pos;
	m_OliginTex = D3DXVECTOR2(1.0f, 1.0f);
	m_Oliginsize = size;

	m_pos = pos;
	m_size = sqrtf(size.x * size.x + size.z * size.z);
	SizeY = size.y;
	fRotate = 0;
	fAddRotate = Rotate;
	SetPosField(m_pos, D3DXVECTOR3(m_size, SizeY, {}), fRotate, fRotate);

	m_FieldColor = FieldColor;
	m_FieldAddColor = FieldAddColor;

	m_MaxSize = MaxSize;
	m_AddSize = AddSize;
	m_nTime = nTime;
	m_nSynsetic = nSynsetic;

	m_nDensity = nDensity;
	m_fDistance = fDistance;
	m_ParticleColor = ParticleColor;
	m_ParticleAddColor = ParticleAddColor;
	m_nParticleTex = nParticleTex;
	m_nParticleLife = nParticleLife;
	m_nParticleSynthetic = nParticleSynthetic;
	m_ParticleMove3d = ParticleMove3d;
	m_ParticleAddSize = ParticleAddSize;
	m_ParticleSize = ParticleSize;

	Time = (int)(rand() % ParticleTime) + 1;
	nParticleTime = ParticleTime;
	m_ActiveAddSize = ActiveAddSize;

	m_FieldTime = FieldTime;
	m_FieldCreate = FieldCreate;
	m_CreatePreset = CreatePreset;
	m_FieldTimedelta = FieldTime - 5;
	m_AnimPatternType = AnimPatternType;
	bUninit = false;
	return S_OK;
}

//終了処理
void CFieldEffect::Uninit()
{
	CPlane::Uninit();
}

//更新処理
void CFieldEffect::Update()
{
	m_pos = GetPos();

	float fAngle;
	float fAngle2;

	//カラーの変動
	m_FieldColor.r += m_FieldAddColor.r;
	m_FieldColor.g += m_FieldAddColor.g;
	m_FieldColor.b += m_FieldAddColor.b;
	m_FieldColor.a += m_FieldAddColor.a;

	//カラーが0以下の時
	if (m_FieldColor.r < 0)
	{
		m_FieldColor.r = 0;
	}
	if (m_FieldColor.g < 0)
	{
		m_FieldColor.g = 0;
	}
	if (m_FieldColor.b < 0)
	{
		m_FieldColor.b = 0;
	}
	if (m_FieldColor.a < 0)
	{
		m_FieldColor.a = 0;
	}

	//カラーが255以上の時
	if (m_FieldColor.r > 256)
	{
		m_FieldColor.r = 255;
	}
	if (m_FieldColor.g > 256)
	{
		m_FieldColor.g = 255;
	}
	if (m_FieldColor.b > 256)
	{
		m_FieldColor.b = 255;
	}
	if (m_FieldColor.a > 256)
	{
		m_FieldColor.a = 255;
	}

	float moveMin = m_ParticleMove3d * -1 / 1.5f;
	int Life = m_nParticleLife ;
	float Size = m_ParticleAddSize / 1.5f;
	float AddR = fAddRotate * 30;
	//フィールドの動き
	switch (EffectTime)
	{
	case(START)://展開中
		m_size += m_AddSize;
		fRotate += AddR;

		if (m_size > m_MaxSize)
		{
			EffectTime = ACTIVE;


			for (int i = 0; i < m_nDensity + 20; i++)
			{
				fAngle = CIRCLE;
				fAngle2 = CIRCLE;

				CStraight3D::Create(
					D3DXVECTOR3(
						m_fDistance  * sinf(fAngle) * cosf(fAngle2),
						m_fDistance / 2 * cosf(fAngle),
						m_fDistance  * sinf(fAngle) * sinf(fAngle2)
					),

					D3DXVECTOR3(m_ParticleSize, m_ParticleSize, 0.0f),
					D3DXVECTOR3(m_ParticleAddSize, m_ParticleAddSize, 0.0f),
					D3DXVECTOR3(moveMin, 0.0f, 0.0f),
					m_ParticleColor,
					m_ParticleAddColor,
					m_nParticleTex,
					m_nParticleLife,
					CStraight3D::TARGET,
					m_pos,
					m_nParticleSynthetic,0,
					(CStraight3D::RAND_PATTEN)0,
					(CStraight3D::POS_PATTERN)0,
					D3DXVECTOR2(0.0f, 0.0f),
					D3DXVECTOR2(1.0f, 1.0f),
					0,
					D3DXVECTOR2(1.0f,1.0f),
					(CBillEffect::ANIMPATTERN)m_AnimPatternType);
			}

		}
		break;
	case(ACTIVE):	//動き中
		fRotate += fAddRotate;

		Time--;
		m_size += m_ActiveAddSize;
		if (m_size < 0)
		{
			m_size = 0;
		}

		if (Time <= 0)
		{
			for (int i = 0; i < m_nDensity; i++)
			{
				fAngle = CIRCLE;
				fAngle2 = CIRCLE;

				CStraight3D::Create(
					m_pos,

					D3DXVECTOR3(m_ParticleSize, m_ParticleSize, 0.0f),
					D3DXVECTOR3(m_ParticleAddSize, m_ParticleAddSize, 0.0f),
					D3DXVECTOR3(m_ParticleMove3d, 0.0f, 0.0f),
					m_ParticleColor,
					m_ParticleAddColor,
					m_nParticleTex,
					m_nParticleLife,
					CStraight3D::TARGET,
					m_pos,
					m_nParticleSynthetic,
					m_fDistance,
					(CStraight3D::RAND_PATTEN)0,
					(CStraight3D::POS_PATTERN)2,
					D3DXVECTOR2(0.0f,0.0f),
					D3DXVECTOR2(1.0f, 1.0f),
					0,
					D3DXVECTOR2(1.0f, 1.0f),
					(CBillEffect::ANIMPATTERN)m_AnimPatternType);
				Time = (int)(rand() % nParticleTime) + 1;
			}
		}

		if (m_FieldCreate == true)
		{
			m_FieldTimedelta++;
			if (m_FieldTimedelta >= m_FieldTime)
			{
				CPresetEffect::SetEffect3D(m_CreatePreset, D3DXVECTOR3(0.0f, 0.0f, 0.0f), {});
				m_FieldTimedelta = 0;
			}
		}

		m_nTime--;
		if (m_nTime < 0)
		{
			EffectTime = END;
		}
		break;
	case(END):	//終了
		m_size -= m_AddSize;

		if (m_size < 10)
		{
			bUninit = true;

			for (int i = 0; i < m_nDensity + 10; i++)
			{
				fAngle = CIRCLE;
				fAngle2 = CIRCLE;

				CStraight3D::Create(
					m_pos,
					D3DXVECTOR3(m_ParticleSize, m_ParticleSize, 0.0f),
					D3DXVECTOR3(Size, Size, 0.0f),
					D3DXVECTOR3(moveMin / 2 , 0.0f, 0.0f),
					m_ParticleColor,
					m_ParticleAddColor,
					m_nParticleTex,
					Life,
					CStraight3D::TARGET,
					m_pos,
					m_nParticleSynthetic,
					m_fDistance,
					(CStraight3D::RAND_PATTEN)0,
					(CStraight3D::POS_PATTERN)2,
					D3DXVECTOR2(0.0f, 0.0f),
					D3DXVECTOR2(1.0f, 1.0f),
					0,
					D3DXVECTOR2(1.0f, 1.0f),
					(CBillEffect::ANIMPATTERN)m_AnimPatternType);
			}

		}
		break;
	default:

		break;
	}

	nLife--;
	if (nLife < 0)
	{
		bUninit = true;
	}

	ColorChange(m_FieldColor);
	SetPos(m_pos);
	SetPosField(m_pos, D3DXVECTOR3(m_size, SizeY, {}), fRotate, -fRotate);

	if (bUninit == true)
	{
		Uninit();
	}
}

//描画処理
void CFieldEffect::Draw()
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = CManager::GetRenderer()->GetDevice();     //デバイスを取得する

	//Zテスト関係
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	//アルファテスト関係
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);


	if (m_nSynsetic == 0)
	{
		//加算合成関係
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	else if (m_nSynsetic == 1)
	{
		//減算合成の設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	//それ以外の数値は加算合成に
	else
	{
		//加算合成関係
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	//ラインティングを無視する
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//カリングオフ
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	CPlane::Draw();
	//カリングオン
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//Zテスト関係
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//アルファテスト関係
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);

	//通常合成に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//位置によっては映らないようにする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

//作成
CFieldEffect *CFieldEffect::Create(D3DXVECTOR3 size,
	D3DXVECTOR3 pos,
	D3DCOLORVALUE FieldColor,
	D3DCOLORVALUE FieldAddColor,
	float Rotate,
	int nTime,
	float MaxSize,
	float AddSize,
	int nSynsetic,
	int nDensity,
	float fDistance,
	D3DCOLORVALUE ParticleColor,
	D3DCOLORVALUE ParticleAddColor,
	int nParticleTex,
	int nParticleLife,
	int nParticleSynthetic,
	float ParticleMove3d,
	float ParticleAddSize,
	float ParticleSize,
	int ParticleTime,
	float ActiveAddSize,
	int FieldTime,
	bool FieldCreate,
	int CreatePreset,
	int AnimPatternType)
{
	CFieldEffect *pFieldEffect;
	pFieldEffect = new CFieldEffect(CObject::PRIORITY::EFFECT3D);
	if (pFieldEffect != NULL)
	{
		pFieldEffect->Init(size,
			pos,
			FieldColor,
			FieldAddColor,
			Rotate,
			nTime,
			MaxSize,
			AddSize,
			nSynsetic,
			nDensity,
			fDistance,
			ParticleColor,
			ParticleAddColor,
			nParticleTex,
			nParticleLife,
			nParticleSynthetic,
			ParticleMove3d,
			ParticleAddSize,
			ParticleSize,
			ParticleTime,
			ActiveAddSize,
			FieldTime,
			FieldCreate,
			CreatePreset,
			AnimPatternType);

		pFieldEffect->SetTexture(nParticleTex);
	}

	return pFieldEffect;
}
