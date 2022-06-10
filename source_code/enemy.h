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
	CCommunicationData::COMMUNICATION_DATA *GetCommuData(void) { return m_commu_data.GetCmmuData(); }	// ゲッダー
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	// 生成
	static void Recv(CCommunicationData::COMMUNICATION_DATA *my_data);	// レシーブスレッド
private:
	void Attack(void);	// 攻撃
	void Move(void);	// 移動
	CXanimModel *m_model;	// モデル
	CCommunicationData m_commu_data;	// 通信データ
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_rot;		// 向き
	D3DXMATRIX m_mtx_wld;	// ワールドマトリックス
	int m_my_index;	// 自分のプレイヤー番号
};

#endif