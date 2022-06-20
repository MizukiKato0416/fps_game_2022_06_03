//====================================================
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
#define MAX_BULLET (30)
#define MAX_PLAYER (3)
#define MAX_MOTION_DATA (64)
#define MAX_DATA (1)
#define MAX_COMMU_DATA (2048)

//-------------------------------
// Classの定義
//-------------------------------
class CCommunicationData
{
public:
	//---------------------------
	// 構造体
	//---------------------------
	typedef struct
	{
		int nNumber;	// プレイヤーのインデックス
		float fMotionSpeed;	// モーションのスピード
		char aMotion[MAX_DATA][MAX_MOTION_DATA];	// モーションの種類
		D3DXVECTOR3 Pos;	// プレイヤーの位置
		D3DXVECTOR3 Rot;	// 角度
	} PLAYUER_DATA;

	typedef struct
	{
		D3DXVECTOR3 BigenPos;	// 弾の位置
		D3DXVECTOR3 EndPos;	// 弾の位置
		int nCollEnemy;	// 当てた相手の番号
		bool bEnemyHit;	// 当たったどうか
		bool bObjectHit;	// 当たったどうか
	} BULLET_DATA;

	typedef struct
	{
		PLAYUER_DATA Player;	// プレイヤーデータ
		BULLET_DATA Bullet[MAX_BULLET];	// 弾データ
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