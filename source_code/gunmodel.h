//=============================================================================
//
// テクスチャ処理 [texture.h]
// Author : 羽鳥太一
//
//=============================================================================
#ifndef _GUN_MODEL_H_
#define _GUN_MODEL_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "object.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModel;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CGunModel : CObject
{
public:
	CGunModel(CObject::PRIORITY Priority = CObject::PRIORITY::MODEL);	// デフォルトコンストラクタ
	~CGunModel();	// デフォルトデストラクタ
	HRESULT Init(void);	// ポリゴンの初期化
	void Uninit(void);	// ポリゴンの終了
	void Update(void);	// ポリゴンの更新
	void Draw(void);	// ポリゴンの描画
	static CGunModel *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 muzzle_pos, string type);	// 生成
	void SetPos(const D3DXVECTOR3 &pos) { m_pos = pos; CObject::SetPos(m_pos); }		//位置設定処理
	D3DXVECTOR3 GetPos(void) { return m_pos; }											//位置取得処理
	void SetRot(const D3DXVECTOR3 &rot) { m_rot = rot; }								//回転設定処理
	D3DXVECTOR3 GetRot(void) { return m_rot; }											//回転取得処理
	D3DXMATRIX GetMuzzleMtx(void) { return m_mtx_muzzle_world; }
	CModel *GetModel(void) { return m_model; }											//モデル取得処理
private:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_muzzle_pos;
	D3DXMATRIX m_mtx_world;				//ワールドマトリックス
	D3DXMATRIX m_mtx_muzzle_world;				//ワールドマトリックス
	string m_type;
	CModel *m_model;					//モデルのポインタ
};

#endif