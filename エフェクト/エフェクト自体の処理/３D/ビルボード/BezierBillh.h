//=============================================================================
// ベジェ曲線移動ビルボード処理 [BezierBill.h]
// Author : 佐藤秀亮
//=============================================================================
#ifndef _BEZIERBILL_H_
#define _BEZIERBILL_H_

#include "plane.h"
#include "billEffect.h"

class CBezierBill : public CBillEffect
{
public:
	CBezierBill(int nPriority);	//コンストラクタ
	~CBezierBill();	//デストラクタ

	typedef struct
	{
		double x;//ホーミング座標
		double y;
		double z;

		double u;
		int Counter;
		int Counter2;
		int DivNum;

		double P0[3];
		double P1[3];
		double P2[3];
		double P3[3];// 制御点

		bool f;
	} HOMING_BEZIER;

	HRESULT Init(D3DXVECTOR3 Size,
		D3DXVECTOR3 MinSize,
		D3DCOLORVALUE color,
		D3DCOLORVALUE Mincolor,
		int nTex, int nLife,
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
		int TrajectTex,
		int TrajectLife,
		float DistanceTarget,
		int Synthetic,
		int TrajectSynthetic,
		ANIMPATTERN AnimPattern);

	void Uninit();
	void Update();
	void Draw();

	static CBezierBill *Create(D3DXVECTOR3 Size,
		D3DXVECTOR3 MinSize,
		D3DCOLORVALUE color,
		D3DCOLORVALUE Mincolor,
		int nTex, int nLife,
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
		int TrajectTex,
		int TrajectLife,
		float DistanceTarget,
		int Synthetic,
		int TrajectSynthetic,
		ANIMPATTERN AnimPattern);

	void PredictTraject();

private:
	HOMING_BEZIER m_Bezier;

	D3DCOLORVALUE m_TrajectColor;
	D3DCOLORVALUE m_AddTrajectColor;
	D3DCOLORVALUE m_TrajectColor1;
	D3DCOLORVALUE m_AddTrajectColor1;

	float m_TrajectSize;
	float m_TrajectAddSize;

	int m_TjajectTex;
	int m_TrajectLife;

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_Size;
	D3DXVECTOR3 m_Oldpos;

	D3DXVECTOR3 m_Target;	//目標地点
	D3DXVECTOR3 m_ControlBezier;	//制御点
	int m_Speed;	//スピード(ベジェの通過点数)

	float m_fRandAngle;
	float m_fRandAngle2;
	float m_nDistanse;
	float m_XZr;	//角度xz

	int m_TrajectSynthetic;
};

#endif // !_BEZIERBILL_H_
