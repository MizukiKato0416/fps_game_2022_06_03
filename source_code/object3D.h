//===========================================
//3Dポリゴン処理
//Author:加藤瑞葵
//===========================================
#ifndef _SCENE3D_H_
#define _SCENE3D_H_
#include "object.h"
#include "main.h"

//================================================
//マクロ定義
//================================================
#define FVF_VERTEX_3D (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)		//頂点フォーマット3D
#define VERTEX_3D_NUM (4)														//頂点数

//========================================================
//構造体の定義3D
//========================================================
typedef struct
{
	D3DXVECTOR3 pos;		//頂点座標
	D3DXVECTOR3 nor;		//法線ベクトル
	D3DCOLOR col;			//頂点カラー
	D3DXVECTOR2 tex;		//テクスチャ座標
} VERTEX_3D;

//================================================
//クラスの定義
//================================================
//クラスの定義
class CObject3D : public CObject
{
public:
	//ポリゴンの原点の種類
	enum class ORIGIN_TYPE
	{
		CENTER = 0,			//中央
		LEFT,				//左端
		RIGHT,				//右端
		TOP,				//上端
		LOWER,				//下端
		MAX
	};

	CObject3D(CObject::PRIORITY Priority = CObject::PRIORITY::POLYGON_3D);	//コンストラクタ
	~CObject3D();									//デストラクタ

												//メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CObject3D *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &size, const D3DXVECTOR3 &rot);
	D3DXVECTOR3 GetPos(void);											//位置取得処理
	void SetPos(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &size);		//位置設定処理
	void SetTex(const int &nPattern, const int &nMaxPattern);			//テクスチャアニメーション設定処理
	void SetTex(const float &fMoveTexU, const float &fMoveTexV);		//テクスチャ移動設定処理
	void SetCol(const D3DCOLORVALUE &col);								//カラーバリュー設定処理
	void SetCol(const D3DXCOLOR &col);									//カラー設定処理
	D3DXCOLOR GetCol(void);												//カラー取得処理
	void SetCulling(const bool &bCulling);								//カリング設定処理
	void SetAlphaBlend(const bool &bAlphaBlend);						//加算合成設定処理
	void SetAlphaTest(const bool &bAlphaTest);							//αアテスト設定処理
	void SetLighting(const bool &bLighting);							//ライティング設定処理
	void BindTexture(const LPDIRECT3DTEXTURE9 &pTexture);				//テクスチャ設定処理
	void SetOriginType(const ORIGIN_TYPE &originType) { m_OriginType = originType; }		//原点の種類設定処理

private:

protected:
	D3DXVECTOR3 m_pos;							//ポリゴンの位置
	D3DXVECTOR3 m_vtxPos[VERTEX_3D_NUM];		//頂点の位置
	D3DXVECTOR3	m_size;							//サイズ
	D3DXVECTOR3 m_rot;							//向き
	D3DXVECTOR3 m_vtxRot[VERTEX_3D_NUM];		//頂点の向き
	LPDIRECT3DTEXTURE9 m_pTexture;				//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;			//頂点バッファへのポインタ
	D3DXMATRIX m_mtxWorld;						//ワールドマトリックス
	D3DXMATRIX m_vtxMtxWorld[VERTEX_3D_NUM];	//頂点のワールドマトリックス
	bool m_bCulling;							//カリングを行うか行わないか
	bool m_bAlphaBlend;							//加算合成するかどうか
	bool m_bAlphaTest;							//αテストするかどうか
	bool m_bLighting;							//ライティング有効かどうか
	ORIGIN_TYPE m_OriginType;					//原点の種類
};

#endif // !_SCENE3D_H_