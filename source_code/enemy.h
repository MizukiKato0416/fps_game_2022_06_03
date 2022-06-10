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
	CCommunicationData::COMMUNICATION_DATA *GetCommuData(int index) { return m_commu_data[index].GetCmmuData(); }	// ゲッダー
	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	// 生成
	static void Recv(void);	// レシーブスレッド
private:
	void Attack(void);	// 攻撃
	void Move(void);	// 移動
	static CCommunicationData m_commu_data[MAX_PLAYER];	// 通信データ
	CXanimModel *m_model[MAX_PLAYER];	// モデル
	D3DXVECTOR3 m_pos[MAX_PLAYER];		// 位置
	D3DXVECTOR3 m_rot[MAX_PLAYER];		// 向き
	D3DXMATRIX m_mtx_wld[MAX_PLAYER];	// ワールドマトリックス
	int m_my_index[MAX_PLAYER];	// 自分のプレイヤー番号
};

#endif