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
#pragma comment(lib, "winmm.lib")
//------------------------
// プロトタイプ宣言
//------------------------
void KeyWait(void);	// キー入力待ち
void CreateRoom(vector<CCommunication*> communication, int room_num);	// 部屋生成処理
void AllAcceptInit(CTcpListener* listener, int room_num);	// 部屋生成処理
void Init(void);	// 初期化
bool calcRaySphere(float lx, float ly, float lz, float vx, float vy, float vz, float px, float py, float pz, float r, float &q1x, float &q1y, float &q1z, float &q2x, float &q2y, float &q2z);	// レイとカプセルの貫通点を算出
bool calcRayInfCilinder(float lx, float ly, float lz, float vx, float vy, float vz, float p1x, float p1y, float p1z, float p2x, float p2y, float p2z, float r, float &q1x, float &q1y, float &q1z, float &q2x, float &q2y, float &q2z);	// レイと円柱の貫通点を算出
bool calcRayCapsule(float lx, float ly, float lz, float vx, float vy, float vz, float p1x, float p1y, float p1z, float p2x, float p2y, float p2z, float r, float &q1x, float &q1y, float &q1z, float &q2x, float &q2y, float &q2z);	// レイとカプセルの貫通点を算出
float checkDot(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);	// ∠P1P2P3の内積を算出


#endif // !_MAIN_H_