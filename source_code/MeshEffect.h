//=============================================================================
// エフェクト用ポリゴン処理 [MeshEffect.h]
// Author : 佐藤秀亮
//=============================================================================
#ifndef _MESHEFFECT_H_
#define _MESHEFFECT_H_

#include "main.h"
#include "plane.h"
#define MAX_TEX (32)

class CMeshEffect : public CPlane
{
public:
	typedef enum
	{
		TYPE_TRAJECTORY,	//軌跡
		TYPE_TRAJECTPLAYER,
		TYPE_MAX,
	} EFFECT_TYPE;

	CMeshEffect(PRIORITY priorty = PRIORITY::EFFECT3D);	//コンストラクタ
	~CMeshEffect();	//デストラクタ

	HRESULT Init(D3DXVECTOR3 Vtxpos1, D3DXVECTOR3 Vtxpos2, D3DXVECTOR3 VtxOldpos1, D3DXVECTOR3 VtxOldpos2, D3DXVECTOR3 /*Size*/, string nTex, int nLife, EFFECT_TYPE nType);	//初期化
	void Uninit(void);	//終了
	void Update(void);	//更新
	void Draw(void);	//描画

	void SetTexture(int nTex);		//指定された番号のテクスチャを貼れるように

	void ColorChange(D3DCOLORVALUE color, D3DCOLORVALUE color2);
	void SetSize();

	static void CreateTextureMesh();
	static void UninitTextureMesh();
protected:
	bool m_bUninit;	//抹消フラグ
	int m_Synthetic;

private:
	string m_pTexture;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	//頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff = NULL;	//インデックスバッファ
	D3DXMATRIX m_mtxWorld;	//ワールドマトリックス

	D3DXVECTOR3 m_Max;
	D3DXVECTOR3 m_Max2;

	D3DXVECTOR3 m_PosOld;
	D3DXVECTOR3 m_PosOld2;

	int m_nLife;	//寿命

	int m_nVtx;		//頂点数
	int m_nIndx;	//インデックスの数

	EFFECT_TYPE m_atype;	//エフェクトのタイプ

	static int m_nMaxTex;
};
#endif // !_MESHEFFECT_H_
