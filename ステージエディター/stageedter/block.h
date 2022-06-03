//=============================================================================
//
// ブロック処理 [block.h]
// Author : 羽鳥太一
//
//=============================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "scene.h"
#include "model.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModel;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CBlock : public CScene
{
public:
	CBlock(LAYER_TYPE Layer = LAYER_TYPE::LAYER_00);	// デフォルトコンストラクタ
	~CBlock();	// デフォルトデストラクタ
	HRESULT Init(void);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int type);
	void Uninit(void);	// ポリゴンの終了
	void Update(void);	// ポリゴンの更新
	void Draw(void);	// ポリゴンの描画
	void SetUse(bool bUse) { m_bUse = bUse; }
	void SetPos(D3DXVECTOR3 pos) { m_Pos = pos; }	// 位置のセッター
	void SetRot(D3DXVECTOR3 rot) { m_Rot = rot; }	// 向きのセッター
	void SetMatrix(D3DXMATRIX mtx) { m_pModel->SetMatrix(mtx); }	// ワールドマトリックスのゲッダー
	void SetScale(D3DXVECTOR3 scale) { m_pModel->SetScale(scale); }	// ワールドマトリックスのゲッダー
	bool GetUse(void) { return m_bUse; }
	D3DXVECTOR3 GetPos(void) { return m_Pos; }	// 位置のセッター
	D3DXVECTOR3 GetRot(void) { return m_Rot; }	// 向きのセッター
	D3DXMATRIX GetMatrix(void) { return m_pModel->GetMatrix(); }	// ワールドマトリックスのゲッダー
	D3DXVECTOR3 GetScale(void) { return m_pModel->GetScale(); }
	D3DXVECTOR3 GetVMin(void) { return m_pModel->GetVMin(); }
	D3DXVECTOR3 GetVMax(void) { return m_pModel->GetVMax(); }
	static CBlock *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int type);
private:
	D3DXMATRIX m_MtxWold;	//ワールドマトリックス
	D3DXVECTOR3 m_Pos;		//位置
	D3DXVECTOR3 m_Rot;		//向き
	D3DXVECTOR3 m_Scale;		//向き
	CModel *m_pModel;	// モデル
	bool m_bUse;
};

#endif