//=============================================================================
// ベジェ曲線移動ビルボード処理 [BezierBill.cpp]
// Author : 佐藤秀亮
//=============================================================================
#include "BezierBillh.h"
#include "manager.h"

#include "Trajectory.h"
#include "control.h"
//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CBezierBill::CBezierBill(PRIORITY priorty) : CBillEffect(priorty)
{
	m_ControlBezier = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CBezierBill::~CBezierBill()
{

}

//*****************************************************************************
//初期化
//*****************************************************************************
HRESULT CBezierBill::Init(D3DXVECTOR3 Size,
	D3DXVECTOR3 MinSize,
	D3DCOLORVALUE color,
	D3DCOLORVALUE Mincolor,
	string nTex, int nLife,
	D3DXVECTOR2 TexNum,
	D3DXVECTOR2 TexMove,
	int nAnimCounter,
	D3DXVECTOR2 nSplit,
	D3DXVECTOR3 pos,
	D3DXVECTOR3 Target,
	int Speed,
	D3DXVECTOR3 ControlBezier,
	D3DXVECTOR3 rot,
	float TrajectSize,
	D3DCOLORVALUE FastTrajecttcolor,
	D3DCOLORVALUE FastTrajectMincolor,
	D3DCOLORVALUE SecondTrajecttcolor,
	D3DCOLORVALUE SecondTrajectMincolor,
	float TrajectMinSize,
	string TrajectTex,
	int TrajectLife,
	float DistanceTarget,
	int Synthetic,
	int TrajectSynthetic,
	ANIMPATTERN AnimPattern)
{
	CBillEffect::Init(Size, MinSize, color, Mincolor, nTex, nLife, TexNum, TexMove, nAnimCounter, nSplit, AnimPattern);

	m_nSynthenic = Synthetic;
	m_TrajectSynthetic = TrajectSynthetic;

	m_Size = Size;
	m_ControlBezier = ControlBezier;
	m_Speed = Speed;
	m_fRandAngle = CIRCLE2;
	m_fRandAngle2 = CIRCLE2;

	m_TrajectSize = TrajectSize;
	m_TrajectAddSize = TrajectMinSize;

	m_TrajectColor = FastTrajecttcolor;
	m_TrajectColor1 = SecondTrajecttcolor;
	m_AddTrajectColor = FastTrajectMincolor;
	m_AddTrajectColor1 = SecondTrajectMincolor;

	m_TjajectTex = TrajectTex;
	m_TrajectLife = TrajectLife;

	float randCont = float(rand() % (int)m_ControlBezier.x) - float(rand() % (int)m_ControlBezier.x);
	float randCont2 = float(rand() % (int)m_ControlBezier.x) - float(rand() % (int)m_ControlBezier.x);

	if (DistanceTarget <= 0)
	{
		DistanceTarget = 1;
	}

	m_nDistanse = float(rand() % (int)DistanceTarget) - float(rand() % (int)DistanceTarget);
	float d = float(rand() % (int)DistanceTarget) - float(rand() % (int)DistanceTarget);

	m_Target = D3DXVECTOR3(Target.x + cosf(CIRCLE) *  m_nDistanse, Target.y, Target.z + cosf(CIRCLE) * d);

	D3DXVECTOR3 a = m_Target - pos;

	m_XZr = (float)atan2(a.x, a.z);		//角度ｘｚ

	float sx = (1.0 - m_ControlBezier.z) * pos.x + m_ControlBezier.z * m_Target.x;
	float sz = (1.0 - m_ControlBezier.z) * pos.z + m_ControlBezier.z * m_Target.z;

	//ベジェ計算
	//制御点
	double P01[3], P12[3], P23[3];
	double P02[3], P13[3];
	double P03[3];

		m_Bezier.x = 0;
		m_Bezier.y = 0;
		m_Bezier.u = 0;

		//通過点カウンター
		m_Bezier.Counter = 0;
		m_Bezier.Counter2 = 0;

		//通過点
		m_Bezier.DivNum = m_Speed;

		//発生点
		m_Bezier.P0[0] = pos.x;
		m_Bezier.P0[1] = pos.y;
		m_Bezier.P0[2] = pos.z;

		//2次元制御点
		//m_Bezier.P1[0] = pos.x + sinf(m_XZr) * m_ControlBezier.z + randCont;
		//m_Bezier.P1[1] = pos.y + m_ControlBezier.y;
		//m_Bezier.P1[2] = pos.z + cosf(m_XZr) * m_ControlBezier.z - randCont;

		//3次元制御らしいっすよこの処理
		m_Bezier.P1[0] = (sx + randCont * sinf(m_XZr + D3DX_PI / 2));
		m_Bezier.P1[1] = pos.y + m_ControlBezier.y;
		m_Bezier.P1[2] = (sz + randCont * cosf(m_XZr + D3DX_PI / 2));

		m_Bezier.P2[0] = (sx + randCont * sinf(m_XZr + D3DX_PI / 2));
		m_Bezier.P2[1] = pos.y + m_ControlBezier.y;
		m_Bezier.P2[2] = (sz + randCont * cosf(m_XZr + D3DX_PI / 2));

		//目標地点
		m_Bezier.P3[0] = m_Target.x;
		m_Bezier.P3[1] = m_Target.y;
		m_Bezier.P3[2] = m_Target.z;

		m_Bezier.f = true;

	if (m_Bezier.f == true)
	{
		m_Bezier.u = (1.0 / m_Bezier.DivNum) * m_Bezier.Counter;

		//計算式
		P01[0] = (1.0 - m_Bezier.u) * m_Bezier.P0[0] + m_Bezier.u * m_Bezier.P1[0];
		P01[1] = (1.0 - m_Bezier.u) * m_Bezier.P0[1] + m_Bezier.u * m_Bezier.P1[1];
		P01[2] = (1.0 - m_Bezier.u) * m_Bezier.P0[2] + m_Bezier.u * m_Bezier.P1[2];

		P12[0] = (1.0 - m_Bezier.u) * m_Bezier.P1[0] + m_Bezier.u * m_Bezier.P2[0];
		P12[1] = (1.0 - m_Bezier.u) * m_Bezier.P1[1] + m_Bezier.u * m_Bezier.P2[1];
		P12[2] = (1.0 - m_Bezier.u) * m_Bezier.P1[2] + m_Bezier.u * m_Bezier.P2[2];

		P23[0] = (1.0 - m_Bezier.u) * m_Bezier.P2[0] + m_Bezier.u * m_Bezier.P3[0];
		P23[1] = (1.0 - m_Bezier.u) * m_Bezier.P2[1] + m_Bezier.u * m_Bezier.P3[1];
		P23[2] = (1.0 - m_Bezier.u) * m_Bezier.P2[1] + m_Bezier.u * m_Bezier.P3[1];


		//位置
		P02[0] = (1.0 - m_Bezier.u) * P01[0] + m_Bezier.u * P12[0];
		P02[1] = (1.0 - m_Bezier.u) * P01[1] + m_Bezier.u * P12[1];
		P02[2] = (1.0 - m_Bezier.u) * P01[2] + m_Bezier.u * P12[2];

		P13[0] = (1.0 - m_Bezier.u) * P12[0] + m_Bezier.u * P23[0];
		P13[1] = (1.0 - m_Bezier.u) * P12[1] + m_Bezier.u * P23[1];
		P13[2] = (1.0 - m_Bezier.u) * P12[2] + m_Bezier.u * P23[2];

		P03[0] = (1.0 - m_Bezier.u) * P02[0] + m_Bezier.u * P13[0];
		P03[1] = (1.0 - m_Bezier.u) * P02[1] + m_Bezier.u * P13[1];
		P03[2] = (1.0 - m_Bezier.u) * P02[2] + m_Bezier.u * P13[2];


		m_Bezier.x = (int)P03[0];
		m_Bezier.y = (int)P03[1];
		m_Bezier.z = (int)P03[2];

		m_pos = D3DXVECTOR3(m_Bezier.x, m_Bezier.y, m_Bezier.z);

	}
	//PredictTraject();
	m_Oldpos = m_pos;
	SetPos(m_pos);
	return S_OK;
}

//*****************************************************************************
//終了
//*****************************************************************************
void CBezierBill::Uninit()
{
	CBillEffect::Uninit();
}

//*****************************************************************************
//更新
//*****************************************************************************
void CBezierBill::Update()
{
	D3DXVECTOR3 pos;
	m_TrajectSize += m_TrajectAddSize;

	//CScene *pScene = GetScene(CManager::PRIORITY_SET);
	//while (pScene)
	//{
	//	CScene *pSceneNext;
	//	pSceneNext = pScene->GetNext();
	//	if (pScene->GetObjType() == CScene::OBJECTTYPE_PLAYER)
	//	{
	//		pos = pScene->GetPos();
	//	}
	//	pScene = pSceneNext;
	//}

	double P01[3], P12[3], P23[3];
	double P02[3], P13[3];
	double P03[3];

	if (m_Bezier.f == true)
	{
		m_Bezier.u = (1.0 / m_Bezier.DivNum) * m_Bezier.Counter;

		//計算式
		P01[0] = (1.0 - m_Bezier.u) * m_Bezier.P0[0] + m_Bezier.u * m_Bezier.P1[0];
		P01[1] = (1.0 - m_Bezier.u) * m_Bezier.P0[1] + m_Bezier.u * m_Bezier.P1[1];
		P01[2] = (1.0 - m_Bezier.u) * m_Bezier.P0[2] + m_Bezier.u * m_Bezier.P1[2];

		P12[0] = (1.0 - m_Bezier.u) * m_Bezier.P1[0] + m_Bezier.u * m_Bezier.P2[0];
		P12[1] = (1.0 - m_Bezier.u) * m_Bezier.P1[1] + m_Bezier.u * m_Bezier.P2[1];
		P12[2] = (1.0 - m_Bezier.u) * m_Bezier.P1[2] + m_Bezier.u * m_Bezier.P2[2];

		P23[0] = (1.0 - m_Bezier.u) * m_Bezier.P2[0] + m_Bezier.u * m_Bezier.P3[0];
		P23[1] = (1.0 - m_Bezier.u) * m_Bezier.P2[1] + m_Bezier.u * m_Bezier.P3[1];
		P23[2] = (1.0 - m_Bezier.u) * m_Bezier.P2[1] + m_Bezier.u * m_Bezier.P3[2];


		//位置
		P02[0] = (1.0 - m_Bezier.u) * P01[0] + m_Bezier.u * P12[0];
		P02[1] = (1.0 - m_Bezier.u) * P01[1] + m_Bezier.u * P12[1];
		P02[2] = (1.0 - m_Bezier.u) * P01[2] + m_Bezier.u * P12[2];

		P13[0] = (1.0 - m_Bezier.u) * P12[0] + m_Bezier.u * P23[0];
		P13[1] = (1.0 - m_Bezier.u) * P12[1] + m_Bezier.u * P23[1];
		P13[2] = (1.0 - m_Bezier.u) * P12[2] + m_Bezier.u * P23[2];

		P03[0] = (1.0 - m_Bezier.u) * P02[0] + m_Bezier.u * P13[0];
		P03[1] = (1.0 - m_Bezier.u) * P02[1] + m_Bezier.u * P13[1];
		P03[2] = (1.0 - m_Bezier.u) * P02[2] + m_Bezier.u * P13[2];


		m_Bezier.x = (int)P03[0];
		m_Bezier.y = (int)P03[1];
		m_Bezier.z = (int)P03[2];

		m_pos = D3DXVECTOR3(m_Bezier.x, m_Bezier.y, m_Bezier.z);

		m_Bezier.Counter++;
		// もしカウンターが分割数に達していたら０に戻す
		if (m_Bezier.Counter == m_Bezier.DivNum + 2)
		{
			m_Bezier.Counter = 0;
			m_Bezier.f = false;//削除
			m_bUninit = true;
		}
	}


	CTrajectory::Create(
		D3DXVECTOR3(m_pos.x, m_pos.y + m_TrajectSize, m_pos.z),
		D3DXVECTOR3(m_pos.x, m_pos.y - m_TrajectSize, m_pos.z),
		D3DXVECTOR3(m_Oldpos.x, m_Oldpos.y + m_TrajectSize, m_Oldpos.z),
		D3DXVECTOR3(m_Oldpos.x, m_Oldpos.y - m_TrajectSize, m_Oldpos.z),
		m_TrajectColor,
		m_AddTrajectColor,
		m_TrajectColor1,
		m_AddTrajectColor1,
		D3DXVECTOR3(m_TrajectSize, m_TrajectSize, m_TrajectSize),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		m_TjajectTex, m_TrajectLife, m_TrajectSynthetic);



	SetPos(m_pos);
	m_Oldpos = m_pos;
	CBillEffect::Update();
}

//*****************************************************************************
//描画
//*****************************************************************************
void CBezierBill::Draw()
{
	CBillEffect::Draw();
}

//*****************************************************************************
//作成
//*****************************************************************************
CBezierBill *CBezierBill::Create(D3DXVECTOR3 Size,
	D3DXVECTOR3 MinSize,
	D3DCOLORVALUE color,
	D3DCOLORVALUE Mincolor,
	string nTex, int nLife,
	D3DXVECTOR2 TexNum,
	D3DXVECTOR2 TexMove,
	int nAnimCounter,
	D3DXVECTOR2 nSplit,
	D3DXVECTOR3 pos,
	D3DXVECTOR3 Target,
	int Speed,
	D3DXVECTOR3 ControlBezier,
	D3DXVECTOR3 rot,
	float TrajectSize,
	D3DCOLORVALUE FastTrajecttcolor,
	D3DCOLORVALUE FastTrajectMincolor,
	D3DCOLORVALUE SecondTrajecttcolor,
	D3DCOLORVALUE SecondTrajectMincolor,
	float TrajectMinSize,
	string TrajectTex,
	int TrajectLife,
	float DistanceTarget,
	int Synthetic,
	int TrajectSynthetic,
	ANIMPATTERN AnimPattern)
{
	CBezierBill * pBezierBill = NULL;
	pBezierBill = new CBezierBill(CObject::PRIORITY::EFFECT3D);
	if (pBezierBill != NULL)
	{
		pBezierBill->Init(Size, MinSize, color, Mincolor,
			nTex, nLife, TexNum, TexMove,
			nAnimCounter, nSplit,
			pos,
			Target,
			Speed,
			ControlBezier,
			rot,
			TrajectSize,
			FastTrajecttcolor,
			FastTrajectMincolor,
			SecondTrajecttcolor,
			SecondTrajectMincolor,
			TrajectMinSize,
			TrajectTex,
			TrajectLife,
			DistanceTarget,
			Synthetic,
			TrajectSynthetic,
			AnimPattern);
	}
	return pBezierBill;

}

//*****************************************************************************
//予測線
//*****************************************************************************
void CBezierBill::PredictTraject()
{
	double P01[3], P12[3], P23[3];
	double P02[3], P13[3];
	double P03[3];
	m_Oldpos = m_pos;

	for (int i = 0; i < m_Bezier.DivNum; i++)
	{
		if (m_Bezier.f == true)
		{
			m_Bezier.u = (1.0 / m_Bezier.DivNum) * m_Bezier.Counter;

			//計算式
			P01[0] = (1.0 - m_Bezier.u) * m_Bezier.P0[0] + m_Bezier.u * m_Bezier.P1[0];
			P01[1] = (1.0 - m_Bezier.u) * m_Bezier.P0[1] + m_Bezier.u * m_Bezier.P1[1];
			P01[2] = (1.0 - m_Bezier.u) * m_Bezier.P0[2] + m_Bezier.u * m_Bezier.P1[2];

			P12[0] = (1.0 - m_Bezier.u) * m_Bezier.P1[0] + m_Bezier.u * m_Bezier.P2[0];
			P12[1] = (1.0 - m_Bezier.u) * m_Bezier.P1[1] + m_Bezier.u * m_Bezier.P2[1];
			P12[2] = (1.0 - m_Bezier.u) * m_Bezier.P1[2] + m_Bezier.u * m_Bezier.P2[2];

			P23[0] = (1.0 - m_Bezier.u) * m_Bezier.P2[0] + m_Bezier.u * m_Bezier.P3[0];
			P23[1] = (1.0 - m_Bezier.u) * m_Bezier.P2[1] + m_Bezier.u * m_Bezier.P3[1];
			P23[2] = (1.0 - m_Bezier.u) * m_Bezier.P2[1] + m_Bezier.u * m_Bezier.P3[1];


			//位置
			P02[0] = (1.0 - m_Bezier.u) * P01[0] + m_Bezier.u * P12[0];
			P02[1] = (1.0 - m_Bezier.u) * P01[1] + m_Bezier.u * P12[1];
			P02[2] = (1.0 - m_Bezier.u) * P01[2] + m_Bezier.u * P12[2];

			P13[0] = (1.0 - m_Bezier.u) * P12[0] + m_Bezier.u * P23[0];
			P13[1] = (1.0 - m_Bezier.u) * P12[1] + m_Bezier.u * P23[1];
			P13[2] = (1.0 - m_Bezier.u) * P12[2] + m_Bezier.u * P23[2];

			P03[0] = (1.0 - m_Bezier.u) * P02[0] + m_Bezier.u * P13[0];
			P03[1] = (1.0 - m_Bezier.u) * P02[1] + m_Bezier.u * P13[1];
			P03[2] = (1.0 - m_Bezier.u) * P02[2] + m_Bezier.u * P13[2];


			m_Bezier.x = (int)P03[0];
			m_Bezier.y = (int)P03[1];
			m_Bezier.z = (int)P03[2];


			m_Bezier.Counter++;

			//CTrajectory::Create(
			//	D3DXVECTOR3(m_Bezier.x, m_Bezier.y + m_TrajectSize, m_Bezier.z),
			//	D3DXVECTOR3(m_Bezier.x, m_Bezier.y - m_TrajectSize, m_Bezier.z),
			//	D3DXVECTOR3(m_Oldpos.x, m_Oldpos.y + m_TrajectSize, m_Oldpos.z),
			//	D3DXVECTOR3(m_Oldpos.x, m_Oldpos.y - m_TrajectSize, m_Oldpos.z),
			//	m_TrajectColor,
			//	m_AddTrajectColor,
			//	m_TrajectColor1,
			//	m_AddTrajectColor1,
			//	D3DXVECTOR3(m_TrajectSize, m_TrajectSize, m_TrajectSize),
			//	D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			//	m_TjajectTex, m_TrajectLife, m_TrajectSynthetic);

			m_Oldpos = D3DXVECTOR3(m_Bezier.x, m_Bezier.y, m_Bezier.z);
			// もしカウンターが分割数に達していたら０に戻す
			if (m_Bezier.Counter == m_Bezier.DivNum)
			{
				m_Bezier.Counter = 0;
				m_Bezier.f = false;//削除
			}
		}
	}


}
