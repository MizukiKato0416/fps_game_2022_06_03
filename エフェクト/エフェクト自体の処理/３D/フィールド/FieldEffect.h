//=============================================================================
// 平面エフェクト処理 [FieldEffect.h]
// Author : 佐藤秀亮
//=============================================================================
#ifndef _FIELDEFFECT_H_
#define _FIELDEFFECT_H_

#include "main.h"
#include "plane.h"

class CFieldEffect : public CPlane
{
public:
	typedef enum
	{
		START,
		ACTIVE,
		END,
		MAX,
	} EFFECT_TIME;

	CFieldEffect(int nPriority);
	~CFieldEffect();

	HRESULT Init(D3DXVECTOR3 size,
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
		int AnimPatternType);

	void Uninit();
	void Update();
	void Draw();

	static CFieldEffect *Create(D3DXVECTOR3 size,
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
		int AnimPatternType);

private:
	EFFECT_TIME EffectTime;	//エフェクトの今の状態

	int nLife;	//寿命
	int nParticleTime;	//パーティクルの発生間隔
	int Time;		//パーティクル間隔計算

	float fRotate;	//現在の回転
	float fAddRotate;	//回転の加算
	D3DCOLORVALUE m_FieldColor;	//パーティクルカラー
	D3DCOLORVALUE m_FieldAddColor;	//パーティクルカラー増減

	D3DXVECTOR3 m_pos;
	float m_MaxSize;	//目標サイズ
	float m_size;		//現在の大きさ
	float m_AddSize;	//大きさの加算
	float m_ActiveAddSize;	//アクティブ中の大きさの加算

	int m_nTime;	//アクティブ中の時間
	int m_nSynsetic;	//合成

	int m_nDensity;	//密度
	float m_fDistance;	//パーティクル発生間隔距離
	D3DCOLORVALUE m_ParticleColor;	//パーティクルカラー
	D3DCOLORVALUE m_ParticleAddColor;	//パーティクルカラー増減
	int m_nParticleTex;	//テクスチャ
	int m_nParticleLife;	//寿命
	int m_nParticleSynthetic;	//合成
	float m_ParticleMove3d;	//移動値
	float m_ParticleAddSize;	//パーティクルサイズ加算
	float m_ParticleSize;	//パーティクルサイズ

	float SizeY;

	int m_FieldTime;	//フィールド生成間隔
	int m_FieldTimedelta;	//フィールド生成間隔計算

	bool m_FieldCreate;	//フィールド生成するか
	int m_CreatePreset;	//どのプリセットを生成するか
	int m_AnimPatternType;
	bool bUninit;
};

#endif