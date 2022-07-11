//====================================================
//
// プレイヤーのヘッダーファイル(tcp_client.h)
// Author : 羽鳥 太一
//
//====================================================
#ifndef _NET_WORK_MANAGER_H_
#define _NET_WORK_MANAGER_H_

//-------------------------------
// インクルード
//-------------------------------
#include "main.h"
#include "communicationdata.h"
#include "tcp_client.h"
#include <thread>

//-------------------------------
// 前方宣言
//-------------------------------
class CTcpClient;

//-------------------------------
// Classの定義
//-------------------------------
class CNetWorkManager
{
public:
	CNetWorkManager();	// デフォルトコンストラクタ
	~CNetWorkManager();	// デフォルトデストラクタ
	void Init(void);	// 初期化処理
	void Uninit(void);	// 終了処理
	void CreateThread(void) { thread recv_th(Recv); recv_th.detach(); }	// スレッドの生成
	static int Send(char *pSendData, int nSendDataSize) { return m_communication->Send(pSendData, nSendDataSize); }	// 送信(送信データ格納先, 送信データサイズ)
	static void Recv(void);	// 受信
	static CCommunicationData *GetPlayerData(void) { return &m_player_data; }	// プレイヤー情報の取得
	static vector<CCommunicationData> GetEnemyData(void) { return m_enemy_data; }	// 敵情報の取得
	static CTcpClient *GetCommunication(void) { return m_communication; }				//通信クラス取得処理
private:
	static CCommunicationData m_player_data;	// プレイヤーデータ
	static vector<CCommunicationData> m_enemy_data;	// 敵データ
	static CTcpClient *m_communication;	// 通信クラス
};

#endif // _TCP_CLIENT_H_