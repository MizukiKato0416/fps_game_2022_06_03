//=============================================================================
//
// プレイヤーが盛っている銃処理 [texture.h]
// Author : 加藤瑞葵
//
//=============================================================================
#ifndef _GUN_PLAYER_H_
#define _GUN_PLAYER_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "object.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CModelSingle;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CGunPlayer : CObject
{
public:
	CGunPlayer(CObject::PRIORITY Priority = CObject::PRIORITY::GUN);	// デフォルトコンストラクタ
	~CGunPlayer();	// デフォルトデストラクタ
	HRESULT Init(void);	// ポリゴンの初期化
	void Uninit(void);	// ポリゴンの終了
	void Update(void);	// ポリゴンの更新
	void Draw(void);	// ポリゴンの描画
	static CGunPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 muzzle_pos, string type);	// 生成
	void SetPos(const D3DXVECTOR3 &pos) { m_pos = pos; CObject::SetPos(m_pos); }		//位置設定処理
	D3DXVECTOR3 GetPos(void) { return m_pos; }											//位置取得処理
	void SetRot(const D3DXVECTOR3 &rot) { m_rot = rot; }								//回転設定処理
	D3DXVECTOR3 GetRot(void) { return m_rot; }											//回転取得処理
	D3DXMATRIX GetMuzzleMtx(void) { return m_mtx_muzzle_world; }						//マズルのマトリックス取得処理
	CModelSingle *GetModel(void) { return m_model; }									//モデル取得処理
	void SetMtxParent(D3DXMATRIX *mtx) { m_mtxParent = mtx; }							//親のマトリックス取得処理
	void Shot(void);																	//射撃処理
	void ADS(void);																		//ADS処理

private:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_muzzle_pos;
	D3DXMATRIX m_mtx_muzzle_world;				//ワールドマトリックス
	string m_type;
	CModelSingle *m_model;					//モデルのポインタ
	D3DXMATRIX *m_mtxParent;
	int m_nCounter;							//連射用カウンター
	bool m_bShot;							//撃ったかどうか
	bool m_bAds;							//ADSしているかどうか
};

#endif