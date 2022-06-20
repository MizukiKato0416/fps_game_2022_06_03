//==================================================
//
// メインヘッダー(main.h)
// Author：羽鳥太一
//
//==================================================
#ifndef _MAIN_H_
#define _MAIN_H_
//------------------------
// インクルード
//------------------------
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

using namespace std;

//------------------------
// 前方宣言
//------------------------
class CCommunication;
class CTcpListener;

//------------------------
// マクロ定義
//------------------------
#define MAX_IP_NUM (16)

#pragma comment(lib, "ws2_32.lib")

//------------------------
// プロトタイプ宣言
//------------------------
void KeyWait(void);	// キー入力待ち
void CreateRoom(CTcpListener *listener, int room_num);	// 部屋生成処理
void StopOrSurver(void);

#endif // !_MAIN_H_