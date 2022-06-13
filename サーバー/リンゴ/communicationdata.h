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
#include "d3dx9.h"

//-------------------------------
// マクロ定義
//-------------------------------
#define MAX_PLAYER (3)
#define MAX_BULLET (30)
#define MAX_COMMU_DATA (512)

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
		int nMotion;	// モーションの種類
		float fMotionSpeed;	// モーションのスピード
		D3DXVECTOR3 Pos;	// プレイヤーの位置
		D3DXVECTOR3 Rot;	// 角度
	} PLAYUER_DATA;

	typedef struct
	{
		D3DXVECTOR3 Rot;	// 弾の角度
	} BULLET_DATA;

	typedef struct
	{
		PLAYUER_DATA Player;	// プレイヤーデータ
		BULLET_DATA Bullet[MAX_BULLET];	// 弾データ
		bool bConnect = false;	// 接続確認
	} COMMUNICATION_DATA;
	CCommunicationData();	// デフォルトコンストラクタ
	~CCommunicationData();	// デフォルトデストラクタ
	COMMUNICATION_DATA *GetCommuData(void) { return &m_CommuData; }	// データのセッター
	void SetCommuData(COMMUNICATION_DATA Data) { m_CommuData = Data; }	// データの
private:
	SOCKET m_socket;	// ソケット
	COMMUNICATION_DATA m_CommuData;	// データ
};

#endif // _TCP_CLIENT_H_