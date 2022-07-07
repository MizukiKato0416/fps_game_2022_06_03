//*****************************************************************************
// エフェクト情報の格納＆設置処理 [PresetEffect.h]
// Author : 佐藤秀亮
//*****************************************************************************
#ifndef _PRESETEFFECT_H_
#define _PRESETEFFECT_H_
#include "main.h"
#include "scene.h"

//*****************************************************************************
// マクロ
//*****************************************************************************
#define MAX_EFFECTPATTERN_2D (8)
#define MAX_EFFECTPATTERN_3D (8)

#define MAX_ORDER_3D (8)

class CPresetEffect : CScene
{
public:
	//2D用のエフェクトステータス集
	typedef struct
	{
		int m_nPattern;			//動きのパターン
		D3DXVECTOR3 m_pos;		//位置
		float m_fRotate;		//回転
		D3DXVECTOR3 m_Endpos;	//向かうべき位置
		D3DXVECTOR2 m_move;		//移動量
		D3DXVECTOR2 m_Addmove;	//移動量加算
		int m_nDiffusion;		//拡散率
		int m_nDestroyvec;		//消えるベクトル
		float m_fSize;			//サイズ
		float m_fAddSize;		//サイズ加算
		D3DCOLORVALUE m_Col;			//カラー
		D3DCOLORVALUE m_Changecolor;	//カラー加算
		int m_nLife;				//寿命
		int m_nDensity;			//密度
		bool m_bColorRandR;		//色ランダムR
		bool m_bColorRandG;		//色ランダムG
		bool m_bColorRandB;		//色ランダムB
		bool m_bMousePos;
		int Synthetic;	//合成
		string nTexture;	//テクスチャ
		float m_Distance;	//距離
		D3DXVECTOR2 m_TexMove;	//テクスチャ移動
		D3DXVECTOR2 m_TexNum;	//テクスチャ枚数
		int m_AnimPatternType;
		D3DXVECTOR2 m_TexSplit;	//分割数
		int AnimCnt;	//アニメーションカウント
		int m_nType;	//パターン
		int m_fHigth;	//高さ
		float m_SecondSize;
		D3DCOLORVALUE m_SecondCol;			//2番目カラー
		D3DCOLORVALUE m_SecondChangecolor;	//2番目カラー加算
	} EFFECT_STATE2D;

	//3Dの情報
	typedef struct
	{
		int m_nPattern;			//動きのパターン
		float m_fRotate;		//回転
		float m_move;		//移動量
		float m_Addmove;	//移動量加算
		int m_nDiffusion;		//拡散率
		float m_fSize;			//サイズ
		float m_fAddSize;		//サイズ加算
		float m_MaxSize;		//最大サイズ
		float m_fParticleSize;			//サイズ
		float m_fParticleAddSize;		//サイズ加算
		int m_Active;
		D3DCOLORVALUE m_Col;			//カラー
		D3DCOLORVALUE m_Changecolor;	//カラー加算
		D3DCOLORVALUE m_SecondCol;			//2番目カラー
		D3DCOLORVALUE m_SecondChangecolor;	//2番目カラー加算
		int m_ParticleSynthetic;	//パーティクル合成
		int m_nLife;				//寿命
		int m_nDensity;			//密度
		int m_nDistance;		//発生距離
		int TrajectTop;	//発生モデル１
		int TrajectCur;	//発生モデル2
		D3DXVECTOR3 move3d;	//移動
		int RandMove;	//ランダム移動値
		bool m_bColorRandR;		//色ランダムR
		bool m_bColorRandG;		//色ランダムG
		bool m_bColorRandB;		//色ランダムB
		int Synthetic;	//合成
		string nTexture;	//テクスチャ
		int ParticleTime;
		D3DXVECTOR3 pos;	//位置
		float m_fActiveAddSize;	//アクティブ中の大きさ変更
		int m_FieldTime;	//フィールド生成間隔
		bool m_fieldCreate;	//フィールド生成するか
		int mCreatePreset;	//どのプリセットを生成するか
		int m_nSecondTime;	//2番目の時間計算
		int m_nVtx;	//頂点数
		int m_nType;	//タイプ
		D3DXVECTOR2 m_TexMove;	//テクスチャ移動
		D3DXVECTOR2 m_TexNum;	//テクスチャ枚数
		int m_SecondType;	
		D3DXVECTOR2 m_TexSplit;	//分割数
		int AnimCnt;	//アニメーションカウント
		float m_fHigth;	//高さ
		int m_AnimPatternType;
		D3DXVECTOR3 m_ControlBezier;
		D3DCOLORVALUE m_TherdCol;			//3番目カラー
		D3DCOLORVALUE m_TherdChangecolor;	//3番目カラー加算
		int m_SecondTex;
	} EFFECT_STATE3D;


	CPresetEffect();
	~CPresetEffect();


	//読み込んだエフェクトの情報を格納するやつ
	static void SetEffectState2D(int nPattern,
		D3DXVECTOR3 pos,
		float fRotate,
		D3DXVECTOR2 move,
		D3DXVECTOR2 Addmove,
		int Diffusion,
		int Destroyvec,
		float fSize,
		float m_fAddSize,
		D3DCOLORVALUE col,
		D3DCOLORVALUE Changecolor,
		int nLife,
		int nDensity,
		bool bColorRandR,
		bool bColorRandG,
		bool bColorRandB,
		bool bMousePos,
		int Synthetic,
		string Texture,
		float Distance,
		D3DXVECTOR2 m_TexMove,
		D3DXVECTOR2 m_TexNum,
		int m_AnimPatternType,
		D3DXVECTOR2 m_TexSplit,
		int AnimCnt,
		int nType,
		float fHigth,
		float SecondSize,
		D3DCOLORVALUE m_SecondCol,
		D3DCOLORVALUE m_SecondChangecolor);

	//読み込んだエフェクトの情報を格納するやつ3D
	static void SetEffectState3D(
		int nPattern,
		float m_fRotate,
		float m_move,
		float m_Addmove,
		int m_nDiffusion,
		float m_fSize,
		float m_fAddSize,
		float m_MaxSize,
		float m_fParticleSize,
		float m_fParticleAddSize,
		int m_Active,
		D3DCOLORVALUE m_Col,
		D3DCOLORVALUE m_Changecolor,
		D3DCOLORVALUE m_SecondCol,
		D3DCOLORVALUE m_SecondChangecolor,
		int m_ParticleSynthetic,
		int m_nLife,
		int m_nDensity,
		int TrajectTop,
		int TrajectCur,
		D3DXVECTOR3 move3d,
		int RandMove,
		bool bColorRandR,
		bool bColorRandG,
		bool bColorRandB,
		int Synthetic,
		string Texture,
		int nDistance,
		int ParticleTime,
		D3DXVECTOR3 pos,
		float m_fActiveAddSize,
		int m_FieldTime,
		bool m_fieldCreate,
		int mCreatePreset,
		int m_nSecondTime,
		int m_nVtx,
		int m_nType,
		D3DXVECTOR2 m_TexMove,
		D3DXVECTOR2 m_TexNum,
		int m_nSecondType,
		D3DXVECTOR2 m_TexSplit,
		int AnimCnt,
		float fHigth,
		int AnimPatternType,
		D3DXVECTOR3 ControlBezier,
		D3DCOLORVALUE TherdCol,
		D3DCOLORVALUE TherdChangecolor,
		int SecondTex);

	typedef struct
	{
		int nDeley;
		int nPresetNum;
		int m_nOrder[MAX_ORDER_3D];
		D3DXVECTOR3 pos[MAX_ORDER_3D];
		D3DXVECTOR3 Endpos[MAX_ORDER_3D];
		bool bOne[MAX_ORDER_3D];
	} ORDER_PRESET;

	static void SetEffect2D(int nPattern, D3DXVECTOR3 pos, D3DXVECTOR3 Endpos);
	static void SetEffect3D(int nPattern, D3DXVECTOR3 pos, D3DXVECTOR3 Endpos);


	//ディレイ付き再生
	void SetOrderPreset(int nDeley, int nPresetNum);
	static void CallOrder3D(int nPattern, D3DXVECTOR3 pos, D3DXVECTOR3 Endpos);
	//void DeleySet(int nPattern);

	void CPresetEffect::SetOrder(int nOrder, int nPattern);

	static CPresetEffect *CreateOrderMenu(int nDeley, int nPresetNum, int nOrder[MAX_ORDER_3D]);

	static void ResetPattern() { m_nEffectPattern = 0; }
	static void ResetOrder() { m_nMaxOrderCount = 0; }

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit();
	void Update();
	void Draw();

	void ResetDeley(int n) { nCntDeley[n]= 0; }
private:
	static EFFECT_STATE2D m_EffectState2D[MAX_EFFECTPATTERN_2D];
	static EFFECT_STATE3D m_EffectState3D[MAX_EFFECTPATTERN_3D];
	static ORDER_PRESET m_Order3D[MAX_ORDER_3D][MAX_ORDER_3D];

	static int m_nEffectPattern;	//複数のやつ読み込めるようにするやつ

	static int m_nMaxOrderCount;	//呼び出す最大数カウント
	static int m_PrticleCreateTime;	//パーティクル生成間隔計算

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_EndPos;
	int nCntDeley[MAX_ORDER_3D];
};
#endif // !_PRESETEFFECT_H_