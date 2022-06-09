//=============================================================================
// (直線)3Dエフェクト処理 [Straight3D.h]
// Author : 佐藤秀亮
//=============================================================================
#ifndef _STRAIGHT3D_H_
#define _STRAIGHT3D_H_

#include "main.h"
#include "billEffect.h"


class CStraight3D : public CBillEffect
{
public:

	//動き
	typedef enum
	{
		STRAIGHT,	//まっすぐ
		TARGET,	//目標に向かう
		MOVE_MAX,
	} MOVE_PATTERN;

	//出現位置
	typedef enum
	{
		FIELD,	//平面
		WALL,	//壁
		SPHERE,	//球
		OTHERS,	//その他（生成時に設定）
		POS_MAX,
	} POS_PATTERN;

	typedef enum	//ランダム
	{
		RANDNONE,	//ランダム無し
		RANDPLUS,	//＋ランダム
		RANDMIN,	//ーランダム
		RANDBOTH,	//両方
		RANDSMOKE,	//煙用
		RANDMAX,
	} RAND_PATTEN;

	CStraight3D(int nPriority);
	~CStraight3D();

	HRESULT Init(D3DXVECTOR3 pos,
		D3DXVECTOR3 Size,
		D3DXVECTOR3 MinSize,
		D3DXVECTOR3 move,
		D3DCOLORVALUE color,
		D3DCOLORVALUE Mincolor,
		int nType,
		int nLife,
		CStraight3D::MOVE_PATTERN Pattrn,
		D3DXVECTOR3 Target,
		int Synsetic,
		float Destance,
		CStraight3D::RAND_PATTEN RandPattern,
		CStraight3D::POS_PATTERN PosPattern,
		D3DXVECTOR2 TexMove,
		D3DXVECTOR2 TexNum,
		int nAnimCounter,
		D3DXVECTOR2 nSplit,
		ANIMPATTERN AnimPattern);

	void Uninit();
	void Update();
	void Draw();

	static CStraight3D *Create(D3DXVECTOR3 pos,
		D3DXVECTOR3 Size,
		D3DXVECTOR3 MinSize,
		D3DXVECTOR3 move,
		D3DCOLORVALUE color,
		D3DCOLORVALUE Mincolor,
		int nType,
		int nLife,
		CStraight3D::MOVE_PATTERN Pattrn,
		D3DXVECTOR3 Target,
		int Synsetic,
		float Destance,
		CStraight3D::RAND_PATTEN RandPattern,
		CStraight3D::POS_PATTERN PosPattern,
		D3DXVECTOR2 TexMove,
		D3DXVECTOR2 TexNum,
		int nAnimCounter,
		D3DXVECTOR2 nSplit,
		ANIMPATTERN AnimPattern);
private:
	D3DXVECTOR3 m_pos;	//出現位置
	D3DXVECTOR3 m_move;	//移動値
	D3DXVECTOR3 m_rot;	//移動値


	MOVE_PATTERN m_Pattern;	//動きのパターン
	RAND_PATTEN m_RandPattern;	//動きランダムパターン

	D3DXVECTOR3 m_Target;	//目標地点

	float m_XZr;	//角度xz
	float m_Yr;	//角度y
};

#endif