//====================================================
//
// プレイヤーのヘッダーファイル(tcp_client.cpp)
// Author : 羽鳥 太一
//
//====================================================
//-------------------------------
// インクルード
//-------------------------------
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include "tcp_client.h"

//-------------------------------
// デフォルトコンストラクタ
//-------------------------------
CTcpClient::CTcpClient()
{
	m_socket = INVALID_SOCKET;
}

//-------------------------------
// デフォルトデストラクタ
//-------------------------------
CTcpClient::~CTcpClient()
{

}

//-------------------------------
// WSASの初期化
//-------------------------------
void CTcpClient::WSASInit(void)
{
	WSADATA  wsaData;
	int nErr = WSAStartup(WINSOCK_VERSION, &wsaData);
}

//-------------------------------
// WSASの終了処理
//-------------------------------
void CTcpClient::WSASUninit(void)
{
	WSACleanup();
}

//-------------------------------
// 初期化
//-------------------------------
bool CTcpClient::Init(void)
{
	FILE *pFile;
	char aFile[2][64];

	pFile = fopen("data/severdata.txt", "r");

	if (pFile != NULL)
	{
		while (true)
		{
			fscanf(pFile, "%s", &aFile[0]);
			if (strcmp(aFile[0], "PORT_NUM") == 0) // PORT_NUMの文字列を見つけたら
			{
				fscanf(pFile, "%s", &aFile[1]);
				fscanf(pFile, "%d", &m_nPort);
			}
			if (strcmp(aFile[0], "IP_NUM") == 0) // MAX_WAITの文字列を見つけたら
			{
				fscanf(pFile, "%s", &aFile[1]);
				fscanf(pFile, "%s", m_Ip.c_str());
			}
			if (strcmp(aFile[0], "END_SCRIPT") == 0) //END_SCRIPTの文字列を見つけたら
			{
				break;
			}
		}
	}

	fclose(pFile);

	//------------------------
	// ソケット作成
	//------------------------
	m_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (m_socket == INVALID_SOCKET)
	{
		return false;
	}

	return true;
}

//-------------------------------
// 通信
//-------------------------------
bool CTcpClient::Connect(void)
{
	//------------------------
	// 接続設定
	//------------------------
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;	// どの通信か
	addr.sin_port = htons(m_nPort);	// ポート番号
	addr.sin_addr.S_un.S_addr = inet_addr(m_Ip.c_str());

	//------------------------
	// 接続
	//------------------------
	if (connect(m_socket, (struct sockaddr*)&addr, sizeof(addr)) != 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

//-------------------------------
// 送信
//-------------------------------
int CTcpClient::Send(char *pSendData, int nSendDataSize)
{
	int nRecvSize = send(m_socket, pSendData, nSendDataSize, 0);	// どのソケット, 何を, 何バイト, 通信の種類
	if (nRecvSize < 0)
	{
		Uninit();
	}

	return nRecvSize;
}

//-------------------------------
// 受信
//-------------------------------
int CTcpClient::Recv(char *pRecvData, int nRecvDataSize)
{
	memset(pRecvData, 0, sizeof(pRecvData));
	int nRecvSize = recv(m_socket, pRecvData, nRecvDataSize, 0);	// どのソケット, どこに, 最大容量, 通信の種類

	return nRecvSize;
}

//-------------------------------
// 終了
//-------------------------------
void CTcpClient::Uninit(void)
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