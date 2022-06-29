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
#include <iostream>
#include <vector>
#include "d3dx9.h"

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

//------------------------
// ライブラリのリンク
//------------------------
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib,"d3d9.lib")			//描画に必要
#pragma comment(lib,"d3dx9.lib")		//[d3d9.lib]の拡張ライブラリ

//------------------------
// プロトタイプ宣言
//------------------------
void KeyWait(void);	// キー入力待ち
void CreateRoom(vector<CCommunication*> communication, int room_num);	// 部屋生成処理
void AllAccept(CTcpListener* listener, int room_num);	// 部屋生成処理
void CountUp(void);	// カウントアップ用
void Init(void);	// 初期化

#endif // !_MAIN_H_