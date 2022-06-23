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
#define MAX_PLAYER (3)
#define MAX_MOTION_DATA (64)
#define MAX_ARRAY_DATA (1)
#define MAX_COMMU_DATA (256)

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
		D3DXVECTOR3 Pos;	// プレイヤーの位置
		D3DXVECTOR3 Rot;	// 角度
		int nNumber;	// プレイヤーのインデックス
		float fMotionSpeed;	// モーションのスピード
		char aMotion[MAX_ARRAY_DATA][MAX_MOTION_DATA];	// モーションの
	} PLAYUER_DATA;

	typedef struct
	{
		int nCollEnemy;	// 当てた相手の番号
		int nDamage;	// ダメージ
		bool bUse;	// 使ってるかどうか
	} BULLET_DATA;

	typedef struct
	{
		string sTexPas1;		// テクスチャ
		string sTexPas2;		// テクスチャ
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