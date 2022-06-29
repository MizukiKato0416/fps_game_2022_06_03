//=============================================================================
//
// 敵処理 [enemy.h]
// Author : 羽鳥太一
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "object.h"
#include "communicationdata.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CXanimModel;
class CModelSingle;
class CGunModel;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEnemy : public CObject
{
public:
	CEnemy(CObject::PRIORITY Priority = CObject::PRIORITY::ENEMY);	// デフォルトコンストラクタ
	~CEnemy();	// デフォルトデストラクタ
	HRESULT Init(void);	// ポリゴンの初期化
	void Uninit(void);	// ポリゴンの終了
	void Update(void);	// ポリゴンの更新
	void Draw(void);	// ポリゴンの描画
	static CEnemy *Create(void);	// 生成
	CXanimModel *GetSelfModel(void) { return m_model; }
	int GetLife(void) { return m_nLife; }		//ライフ取得処理
	void SetLife(const int nLife) { m_nLife = nLife; }	//ライフ設定処理
	D3DXMATRIX GetMtx(void) { return m_mtx_wld; }	//ワールドマトリックス取得処理

private:
	void Attack(void);	// 攻撃
	void Move(void);	// 移動
	static int m_all_count;	// 敵のカウンター
	CXanimModel *m_model;	// モデル
	CGunModel *m_pGunModel;			// 銃モデル
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_recvPos;	// 受け取った位置
	D3DXVECTOR3 m_posOld;	// 前の位置
	D3DXVECTOR3 m_rot;		// 向き
	D3DXVECTOR3 m_recvRot;	// 受け取った向き
	D3DXVECTOR3 m_rotOld;	// 前の向き
	D3DXVECTOR3 m_size;		// サイズ
	D3DXMATRIX m_mtx_wld;	// ワールドマトリックス
	int m_my_number;	// 自分の配列
	int m_nLife;			//ライフ
};

#endif