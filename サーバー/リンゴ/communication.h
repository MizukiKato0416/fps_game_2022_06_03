//====================================================
//
// プレイヤーのヘッダーファイル(tcp_client.h)
// Author : 羽鳥 太一
//
//====================================================
#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

//-------------------------------
// インクルード
//-------------------------------
#include "main.h"
#include "d3dx9.h"

//-------------------------------
// Classの定義
//-------------------------------
class CCommunication
{
public:
	CCommunication();	// デフォルトコンストラクタ
	~CCommunication();	// デフォルトデストラクタ
	bool Init(SOCKET sock);	// オーバーロードInit(接続済みソケット)
	int Send(char *pSendData, int nSendDataSize);	// 送信(送信データ格納先, 送信データサイズ)
	int Recv(char *pRecvData, int nRecvDataSize);	// 受信(受信データ格納先, 受信データ格納先最大サイズ)
	void Uninit(void);	// 終了
	SOCKET GetSocket(void) { return m_socket; }	// ソケットの入手
private:
	SOCKET m_socket;	// ソケット
};

#endif // _TCP_CLIENT_H_