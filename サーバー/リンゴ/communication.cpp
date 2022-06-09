//====================================================
//
// プレイヤーのヘッダーファイル(tcp_client.cpp)
// Author : 羽鳥 太一
//
//====================================================
//-------------------------------
// インクルード
//-------------------------------
#include "communication.h"

//-------------------------------
// デフォルトコンストラクタ
//-------------------------------
CCommunication::CCommunication()
{
	m_socket = INVALID_SOCKET;
}

//-------------------------------
// デフォルトデストラクタ
//-------------------------------
CCommunication::~CCommunication()
{

}

//-------------------------------
// オーバーロードInit(接続済みソケット)
//-------------------------------
bool CCommunication::Init(SOCKET sock)
{
	m_socket = sock;

	return true;
}

//-------------------------------
// 送信
//-------------------------------
int CCommunication::Send(char *pSendData, int nSendDataSize)
{
	int nSize = send(m_socket, pSendData, nSendDataSize, 0);	// 何処に, 何を, 何バイト, 通信の種類

	return nSize;
}

//-------------------------------
// 受信
//-------------------------------
int CCommunication::Recv(char *pRecvData, int nRecvDataSize)
{
	memset(pRecvData, 0, sizeof(pRecvData));

	int nSize = recv(m_socket, pRecvData, nRecvDataSize, 0);

	return nSize;
}

//-------------------------------
// 終了
//-------------------------------
void CCommunication::Uninit(void)
{
	if (m_socket == INVALID_SOCKET)
	{
		return;
	}

	//------------------------
	// 接続切断
	//------------------------
	closesocket(m_socket);	// 接続受付用ソケット
	m_socket = INVALID_SOCKET;
}