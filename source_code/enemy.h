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
class CModelCollision;
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
	static void Recv(CCommunicationData *data);	// レシーブスレッド
	void SetCommuData(CCommunicationData::COMMUNICATION_DATA data) { m_commu_data.SetCmmuData(data); }	// 通信データ設定処理
	CCommunicationData::COMMUNICATION_DATA *GetCommuData(void) { return m_commu_data.GetCmmuData(); }	// 通信データ取得処理
	CXanimModel *GetSelfModel(void) { return m_model; }
	int GetPlayerNumber(void) { CCommunicationData::COMMUNICATION_DATA *data = m_commu_data.GetCmmuData(); return data->Player.nNumber; }
	int GetLife(void) { return m_nLife; }		//ライフ取得処理
	void SetLife(const int nLife) { m_nLife = nLife; }	//ライフ設定処理
	D3DXMATRIX GetMtx(void) { return m_mtx_wld; }	//ワールドマトリックス取得処理
	CModelCollision *GetModelCollision(void) { return m_pCollModel; }	//当たり判定用モデル取得処理

private:
	void Attack(void);	// 攻撃
	void Move(void);	// 移動
	CCommunicationData m_commu_data;	// 通信データ
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
	int m_nLife;			//ライフ
	CModelCollision *m_pCollModel;	//当たり判定のボックス
};

#endif