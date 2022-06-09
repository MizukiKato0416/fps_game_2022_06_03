//====================================================
//
// プレイヤーのヘッダーファイル(tcp_client.h)
// Author : 羽鳥 太一
//
//====================================================
#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_

//-------------------------------
// インクルード
//-------------------------------
#include "main.h"

#pragma comment(lib, "ws2_32.lib")

//-------------------------------
// Classの定義
//-------------------------------
class CTcpClient
{
public:
	CTcpClient();	// デフォルトコンストラクタ
	~CTcpClient();	// デフォルトデストラクタ
	bool Init(void);	// 初期化(IPアドレス, ポート番号)
	bool Connect(void);	// 接続
	int Send(char *pSendData, int nSendDataSize);	// 送信(送信データ格納先, 送信データサイズ)
	int Recv(char *pRecvData, int nRecvDataSize);	// 受信(受信データ格納先, 受信データ格納先最大サイズ)
	void Uninit(void);	// 終了
private:
	SOCKET m_socket;	// ソケット
	string m_ip;	// IPアドレス
	int m_port;	// ポート番号
};

#endif // _TCP_CLIENT_H_