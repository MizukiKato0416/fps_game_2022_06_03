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
	static void Recv(void);	// レシーブスレッド
	void SetCommuData(CCommunicationData::COMMUNICATION_DATA data, int number) { m_commu_data[number].SetCmmuData(data); }	// 通信データ設定処理
	static CCommunicationData::COMMUNICATION_DATA *GetCommuData(int number) { return m_commu_data[number].GetCmmuData(); }	// 通信データ取得処理
	CXanimModel *GetSelfModel(void) { return m_model; }
	int GetPlayerNumber(void) { CCommunicationData::COMMUNICATION_DATA *data = m_commu_data[m_my_index].GetCmmuData(); return data->Player.nNumber; }
	//ライフ取得処理
	int GetLife(void) { return m_nLife; }
	//ライフ設定処理
	void SetLife(const int nLife) { m_nLife = nLife; }
	//ワールドマトリックス取得処理
	D3DXMATRIX GetMtx(void) { return m_mtx_wld; }
	//当たり判定用モデル取得処理
	CModelCollision *GetModelCollision(void) { return m_pCollModel; }

private:
	void Attack(void);	// 攻撃
	void Move(void);	// 移動
	static CCommunicationData m_commu_data[MAX_PLAYER];	// 通信データ
	static int m_create_count;	// 生成した数
	static bool m_create_thread;	// スレッドを分けたか
	CXanimModel *m_model;	// モデル
	CModelSingle *m_pGunModel;			// 銃モデル
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_rot;		// 向き
	D3DXMATRIX m_mtx_wld;	// ワールドマトリックス
	int m_my_index;	// 自分のプレイヤー番号
	int m_nLife;			//ライフ
	CModelCollision* m_pCollModel;	//当たり判定のボックス
};

#endif