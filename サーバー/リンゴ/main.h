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

// lx, ly, lz : レイの始点
// vx, vy, vz : レイの方向ベクトル
// px, py, pz : 球の中心点の座標
// r : 球の半径
// q1x, q1y, q1z: 衝突開始点（戻り値）
// q2x, q2y, q2z: 衝突終了点（戻り値）

bool calcRaySphere(
	float lx, float ly, float lz,
	float vx, float vy, float vz,
	float px, float py, float pz,
	float r,
	float &q1x, float &q1y, float &q1z,
	float &q2x, float &q2y, float &q2z
);

// lx, ly, lz : レイの始点
// vx, vy, vz : レイの方向ベクトル
// p1x, p1y, p1z: 円柱軸の1点
// p2x, p2y, p2z: 円柱軸のもう1点
// r : 円柱の半径
// q1x, q1y, q1z: 貫通開始点（戻り値）
// q2x, q2y, q2z: 貫通終了点（戻り値）

bool calcRayInfCilinder(
	float lx, float ly, float lz,
	float vx, float vy, float vz,
	float p1x, float p1y, float p1z,
	float p2x, float p2y, float p2z,
	float r,
	float &q1x, float &q1y, float &q1z,
	float &q2x, float &q2y, float &q2z
);

// レイとカプセルの貫通点を算出
// lx, ly, lz : レイの始点
// vx, vy, vz : レイの方向ベクトル
// p1x, p1y, p1z: カプセル軸の端点P1
// p2x, p2y, p2z: カプセル軸の端点P2
// r : カプセルの半径
// q1x, q1y, q1z: 貫通開始点（戻り値）
// q2x, q2y, q2z: 貫通終了点（戻り値）
bool calcRayCapsule(
	float lx, float ly, float lz,
	float vx, float vy, float vz,
	float p1x, float p1y, float p1z,
	float p2x, float p2y, float p2z,
	float r,
	float &q1x, float &q1y, float &q1z,
	float &q2x, float &q2y, float &q2z
);

// ∠P1P2P3の内積を算出
// x1, y1, z1: 点P1
// x2, y2, z2: 点P2
// x3, y3, z3: 点P3
float checkDot(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);


#endif // !_MAIN_H_