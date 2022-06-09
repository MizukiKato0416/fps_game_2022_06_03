//=============================================================================
// (フィールドのビルボード版)3Dエフェクト処理 [ActiveBillboard.h]
// Author : 佐藤秀亮
//=============================================================================
#ifndef _ACTIVEBILLBORARD_H_
#define _ACTIVEBILLBORARD_H_

#include "main.h"
#include "billEffect.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************


class CActiveBillBoard : public CBillEffect
{
public:
	typedef enum
	{
		START,
		ACTIVE,
		END,
		MAX,
	} EFFECT_TIME;


	CActiveBillBoard(int nPriority);
	~CActiveBillBoard();


	HRESULT Init(D3DXVECTOR3 pos,
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
		ANIMPATTERN AnimPattern);

		void Uninit();
	void Update();
	void Draw();

	static CActiveBillBoard *Create(D3DXVECTOR3 pos,
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
		ANIMPATTERN AnimPattern);

private:
	EFFECT_TIME EffectTime;	//エフェクトの今の状態

	D3DXVECTOR3 m_move;	//移動値
	D3DXVECTOR3 m_rot;	//移動値

	int m_ActiveTime;	//稼働時間
	float m_MaxSize;	//最大サイズ
	float m_AddSize;	//加算サイズ
	float m_fSize;	//サイズ

	bool m_PresetCreate;	//プリセット生成するか
	int m_CreatePreset;	//どのプリセットを生成するか

};

#endif