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
	bool GetIsInit(void) { return m_is_init; }	// 初期化しているかの判定の取得
	bool GetIsUninit(void) { return m_is_uninit; }	// 終了しているかの判定の取得
	int GetNumber(void) { return m_my_number; }	// 自分の部屋番号の取得
	void SetNumber(int number) { m_my_number = number; }	// 自分の部屋番号の設定
private:
	SOCKET m_sockServer;	// ソケット
	int m_my_number;	//　自分の部屋番号
	bool m_is_init;	// 初期化しているか
	bool m_is_uninit;	// 終了しているか
};

#endif // _TCP_CLIENT_H_