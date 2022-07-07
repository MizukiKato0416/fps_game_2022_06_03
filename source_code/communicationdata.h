﻿//====================================================
//
// コミュニケーションデータのヘッダーファイル(communicationdata.h)
// Author : 羽鳥 太一
//
//====================================================
#ifndef _COMUNICASION_DATA_H_
#define _COMUNICASION_DATA_H_

//-------------------------------
// インクルード
//-------------------------------
#include "main.h"

//-------------------------------
// マクロ定義
//-------------------------------
#define MAX_PLAYER (3)
#define MAX_MOTION_DATA (64)
#define MAX_ARRAY_DATA (1)
#define MAX_COMMU_DATA (512)
#define SEND_COUNTER	(10)		//何フレームに一回サーバーから情報が送られてくるか

//-------------------------------
// Classの定義
//-------------------------------
class CCommunicationData
{
public:
	//---------------------------
	// 列挙型
	//---------------------------
	enum class COMMUNICATION_TYPE
	{
		SEND_TO_PLAYER = 0,	// プレイヤーにsend
		SEND_TO_ENEMY,	// 敵にsend
		SEND_TO_ENEMY_AND_PLAYER,
		MAX,
	};
	enum class HIT_TYPE
	{
		NONE = 0,	// 当たっていない
		MESHFIELD,	// メッシュフィールドに当たった
		MODEL,		// モデルに当たった
		ENEMY,		// 敵に当たった
		MAX
	};

	//---------------------------
	// 構造体
	//---------------------------
	typedef struct
	{
		D3DXVECTOR3 Pos;	// プレイヤーの位置
		D3DXVECTOR3 Rot;	// 角度
		D3DXVECTOR3 CamV;	// カメラの位置
		D3DXVECTOR3 CamR;	// カメラの注視点
		D3DXVECTOR3 HitPos[SEND_COUNTER];	// 当てた場所
		D3DXMATRIX ModelMatrix;	// モデルのマトリックス
		int nNumber;	// プレイヤーのインデックス
		int nFrameCount;	// フレームカウント
		int nHitDamage;	// ダメージ
		int nNumShot;	//何発撃ったか
		float fMotionSpeed;	// モーションのスピード
		char aMotion[MAX_ARRAY_DATA][MAX_MOTION_DATA];	// モーションの種類
		bool bHit;	// 当たったか
		HIT_TYPE type[SEND_COUNTER];
	} PLAYUER_DATA;

	bool bUse;	// 使ってるかどうか
	typedef struct
	{
		D3DXVECTOR3 hitPos;	//当てた場所
		HIT_TYPE type;		//当たったオブジェクトの種類
		int nDamage;		//弾が持っているダメージ量
		float fDiffer;		//当たった場所までの距離
		bool bUse;			//使ってるかどうか
	} BULLET_DATA;

	typedef struct
	{
		D3DXVECTOR3 BigenPos;	// 弾の始点
		D3DXVECTOR3 EndPos;		// 弾の終点
		D3DXVECTOR3 Rot;		// 弾の向き
		D3DXVECTOR3 Size;		// 弾のサイズ
		float fSpeed;			// 速さ
	} BALLISTIC_DATA;

	typedef struct
	{
		PLAYUER_DATA Player;	// プレイヤーデータ
		BULLET_DATA Bullet;	// 弾データ
		BALLISTIC_DATA Ballistic;	// 弾道
		COMMUNICATION_TYPE SendType;	// sendoの種類
		bool bConnect;	// 接続確認
	} COMMUNICATION_DATA;

	CCommunicationData();	// デフォルトコンストラクタ
	~CCommunicationData();	// デフォルトデストラクタ
	COMMUNICATION_DATA *GetCmmuData(void) { return &m_CommuData; }	// データのセッター
	void SetCmmuData(COMMUNICATION_DATA Data) { m_CommuData = Data; }	// データのゲッダー
private:
	COMMUNICATION_DATA m_CommuData;	// データ
};

#endif // _TCP_CLIENT_H_