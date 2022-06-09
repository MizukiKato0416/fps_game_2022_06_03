//====================================================
//
// プレイヤーのヘッダーファイル(tcp_server.h)
// Author : 羽鳥 太一
//
//====================================================
#ifndef _TCP_LISTENER_H_
#define _TCP_LISTENER_H_

//-------------------------------
// インクルード
//-------------------------------
#include "main.h"
#include "communication.h"

//-------------------------------
// Classの定義
//-------------------------------
class CTcpListener
{
public:
	CTcpListener();	// デフォルトコンストラクタ
	~CTcpListener();	// デフォルトデストラクタ
	bool Init(void);	// 初期化
	CCommunication *Accept(void);	// 接続待ち
	void Uninit(void);	// 終了
private:
	SOCKET m_sockServer;	// ソケット
};

#endif // _TCP_CLIENT_H_