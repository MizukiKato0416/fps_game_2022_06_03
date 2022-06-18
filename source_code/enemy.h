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

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CEnemy : public CObject
{
public:
	CEnemy(CObject::PRIORITY Priority = CObject::PRIORITY::PLAYER);	// デフォルトコンストラクタ
	~CEnemy();	// デフォルトデストラクタ
	HRESULT Init(void);	// ポリゴンの初期化
	void Uninit(void);	// ポリゴンの終了
	void Update(void);	// ポリゴンの更新
	void Draw(void);	// ポリゴンの描画
	static CEnemy *Create(void);	// 生成
	static void Recv(void);	// レシーブスレッド
	void SetCommuData(CCommunicationData::COMMUNICATION_DATA data, int number) { m_commu_data[number].SetCmmuData(data); }	// 通信データ設定処理
	CCommunicationData::COMMUNICATION_DATA *GetCommuData(int number) { return m_commu_data[number].GetCmmuData(); }	// 通信データ取得処理

private:
	void Attack(void);	// 攻撃
	void Move(void);	// 移動
	static CCommunicationData m_commu_data[MAX_PLAYER];	// 通信データ
	CXanimModel *m_model[MAX_PLAYER];	// モデル
	CModelSingle *m_pGunModel[MAX_PLAYER];			// 銃モデル
	D3DXVECTOR3 m_pos[MAX_PLAYER];		// 位置
	D3DXVECTOR3 m_rot[MAX_PLAYER];		// 向き
	D3DXMATRIX m_mtx_wld[MAX_PLAYER];	// ワールドマトリックス
	int m_my_index[MAX_PLAYER];	// 自分のプレイヤー番号
#ifdef _DEBUG
	static bool m_debug_cennect;	// デバッグ用
#endif //_DEBUG
};

#endif