//===========================================
//プレイヤー処理
//Author:加藤瑞葵
//===========================================
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "object.h"
#include "communicationdata.h"

//前方宣言
class CGunModel;
class CSparkle;
class CWind;
class CXanimModel;

//================================================
//マクロ定義
//================================================
#define MAX_PLAYER_MODEL					(5)			//モデルの数

//================================================
//クラスの定義
//================================================
//クラスの定義
class CPlayer : public CObject
{
public:
	//メンバ関数
	CPlayer(CObject::PRIORITY Priority = CObject::PRIORITY::PLAYER);				//コンストラクタ
	~CPlayer();												//デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPlayer *Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot);

	D3DXVECTOR3 GetRot(void) { return m_rot; }												//プレイヤーの向き取得処理
	void SetRot(const D3DXVECTOR3 &rot) { m_rot = rot; }									//プレイヤーの向き設定処理
	D3DXVECTOR3 GetOffsetPos(void) { return m_offsetPos; }									//プレイヤーのオフセット位置取得処理
	void SetOffsetPos(const D3DXVECTOR3 &offsetPos) { m_offsetPos = offsetPos; }			//プレイヤーのオフセット位置設定処理
	void SetMove(const D3DXVECTOR3 &move) { m_move = move; }								//移動量設定処理
	D3DXVECTOR3 GetMove(void) { return m_move; }											//移動量取得処理
	static void CPlayer::Collision(CObject *&pSubjectObject, const float &fObjRadius);		//当たり判定処理
	bool CPlayer::CollisionOnly(CObject *&pSubjectObject, const float &fObjRadius);			//衝突判定のみの処理
	D3DXMATRIX *GetMtx(void) { return &m_mtxWorld; }										//ワールドマトリックス取得処理
	void SetObjParent(const bool bObjeParent) { m_bObjParent = bObjeParent; }				//オブジェクトとの親子関係設定処理
	bool GetObjParent(void) { return m_bObjParent; }										//オブジェクトとの親子関係取得処理
	D3DXMATRIX *GetMtxParent(void) { return m_mtxWorldParent; }								//ワールドマトリックス取得処理
	void SetMtxParent(D3DXMATRIX *mtx) { m_mtxWorldParent = mtx; }							//ワールドマトリックス設定処理
	CGunModel *GetGunModel(void) { return m_pGunModel; }									//銃取得処理

private:
	//メンバ関数
	void Move(void);								//移動処理
	void Rotate(void);								//回転処理
	void Jump(void);								//ジャンプ処理
	void Shoot(void);								//射撃処理
	void Chest(void);								//腰の処理

	//メンバ変数
	CCommunicationData m_commu_data;	// 通信データ
	D3DXVECTOR3 m_pos;								//位置
	D3DXVECTOR3 m_offsetPos;						//プレイヤーが子になったときのオフセット位置
	D3DXVECTOR3 m_posOld;							//1フレーム前の位置
	D3DXVECTOR3 m_move;								//ポリゴンの移動量
	D3DXVECTOR3	m_size;								//サイズ
	D3DXVECTOR3 m_rot;								//向き
	CGunModel *m_pGunModel;						//銃モデルのポインタ
	D3DXMATRIX m_mtxWorld;							//ワールドマトリックス
	float m_fObjectiveRot;							//目的の向き
	float m_fNumRot;								//向きを変える量
	bool m_bRotate;									//回転しているかどうか
	D3DXMATRIX *m_mtxWorldParent;					//親のワールドマトリックス
	bool m_bObjParent;								//モデル以外との親子関係をつけるかどうか
	bool m_bJump;									//ジャンプしているかどうか
	CXanimModel *m_pAnimModel;						//モデル
	float m_fAnimSpeed;								//アニメーションのスピード
	float m_fMoveSpeed;								//移動する速さ
	int m_nCounter;									//次の弾が出るまでのカウンター
};

#endif // !_PLAYER_H_