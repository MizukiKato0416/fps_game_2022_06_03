//=============================================================================
//
// Xファイルモデル処理 [sceneX.h]
// Author : 羽鳥太一&鶴間俊樹
//
//=============================================================================
#ifndef _SCENEX_H_
#define _SCENEX_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXT_FILE_NAME_MODEL "data/TEXT/load_model.txt"

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CModel
{
public:
	typedef struct
	{
		LPD3DXMESH pMesh;	//メッシュ（頂点情報）へのポインタ
		LPD3DXBUFFER pBuffMat;	//マテリアルへのポインタ
		DWORD nNumMat;	//マテリアルの数
	}ModelData;
	CModel();	// デフォルトコンストラクタ
	~CModel();	// デフォルトデストラクタ
	virtual HRESULT Init(int type);	// ポリゴンの初期化
	virtual HRESULT Init(void);	// ポリゴンの初期化
	virtual void Uninit(void);	// ポリゴンの終了
	virtual void Update(void);	// ポリゴンの更新
	virtual void Draw(bool bUse);	// ポリゴンの描画
	static HRESULT Load(void);	//モデルデータの読み込み
	static CModel *Create(int type);	// 生成
	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }	//位置座標の設定
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }	//角度の設定
	void SetScale(D3DXVECTOR3 scale) { m_Scale = scale; }	//角度の設定
	void SetMatrix(D3DXMATRIX mtx) { m_MtxWold = mtx; }	// ワールドマトリックスのゲッダー
	void SetPrent(CModel *pModel) { m_pPrent = pModel; }	// 親のセッター
	D3DXVECTOR3 GetPos(void) { return m_Pos; }		//位置座標の取得
	D3DXVECTOR3 GetRot(void) { return m_Rot; }		//角度の取得
	D3DXVECTOR3 GetScale(void) { return m_Scale; }
	D3DXMATRIX GetMatrix(void) { return m_MtxWold; }	// ワールドマトリックスのゲッダー
	D3DXVECTOR3 GetVMin(void) { return m_vtxMin; }
	D3DXVECTOR3 GetVMax(void) { return m_vtxMax; }
private:
	static ModelData *m_aModelData;	//全モデルの情報
	static int m_nModelMax;	// 読み込むテクスチャの最大数
	static char **m_aFilePas;	// ファイルのパス
	int m_nType;	//モデルの種類
	CModel *m_pPrent;	// 親のモデル
	D3DXMATRIX m_MtxWold;	//ワールドマトリックス
	LPDIRECT3DTEXTURE9 m_pTexture[64];	// テクスチャ
	D3DXVECTOR3 m_vtxMin, m_vtxMax;	//頂点の端情報
	D3DXVECTOR3 m_Pos;		//位置
	D3DXVECTOR3 m_Rot;		//向き
	D3DXVECTOR3 m_Scale;
};

#endif
