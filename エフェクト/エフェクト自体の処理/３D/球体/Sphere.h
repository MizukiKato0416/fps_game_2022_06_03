//=============================================================================
// 球状処理 [Sphere.h]
// Author : 佐藤秀亮
//=============================================================================
#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "main.h"
#include "plane.h"

#define MAX_TEXTURE (16)

class CSphere : public CScene3D
{
public:
	CSphere(int nPriority);	//コンストラクタ
	~CSphere();	//デストラクタ

	HRESULT Init(D3DXVECTOR3 pos, float rot,
		float fSize, int Vertical,
		int Line, int nTex,
		int Synthetic, D3DXVECTOR2 TexNum);
	void Uninit();
	void Update();
	void Draw();

	D3DXVECTOR3 GetSherePos() { return m_Pos; }

	void SetColor(D3DCOLORVALUE Color);
	void SetSize(D3DXVECTOR3 pos ,float Size);
	void Rotate(float Rotate);

	void SetTexUV(D3DXVECTOR2 TexUV);
	void SetAnimTexUV(D3DXVECTOR2 TexPattern);


	static  void CreateTextureSphere(void);
	static void UninitTextureSphere();


protected:
	bool m_bUninit;	//抹消フラグ
	int m_nLife;	//寿命

	D3DXVECTOR2 m_PatternSize;
	int m_nAnimCount;
	int m_nSetAnimCnt;
	D3DXVECTOR2 m_nSplit;
	D3DXVECTOR2 m_MaxSplit;
private:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_TEXTURE];	//テクスチャ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	//頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff = NULL;	//インデックスバッファ

	D3DXMATRIX m_mtxWorld;	//ワールドマトリックス
	D3DXVECTOR3 m_Pos;	//位置
	D3DXVECTOR2 m_TexUV;	//テクスチャ座標
	D3DXVECTOR2 m_TexMoveUV;	//テクスチャ座標の移動

	D3DXVECTOR2 m_PatternUV;

	float m_Rot;	//回転
	float m_Rot2;	//回転

	int m_nVertical;	//横の分割数
	int m_nLine;		//縦の分割数
	int m_nTex;		//テクスチャ

	int m_nVtx;	//頂点数
	int m_nIdx;	//インデックス数
	int m_Synthetic;	//合成

	float m_fRotate;	//回転

	float m_fRadius;	//半径
	int m_Primithive;	//プリミティブ

	static int m_nMaxTex;

};

#endif // !_MESHEFFECT_H_
