//=======================================================================
//メイン(システム系)処理
//Aouthor: 加藤　瑞葵
//=======================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//インクルードファイル
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include "d3dx9.h"						//描画に必要
#define DIRECTINPUT_VERSION (0x0800)	//ビルド時の警告対応処理用マクロ
#include "dinput.h"						//入力処理に必要
#include "xaudio2.h"					//サウンド処理に必要
#include <time.h>
#include <XInput.h>
#include <vector>
#include <string>
#include <map>
#include <filesystem>
#include <utility>

using namespace std;

//ライブラリのリンク
#pragma comment(lib,"d3d9.lib")			//描画に必要
#pragma comment(lib,"d3dx9.lib")		//[d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")		//DirectXコンポーネント(部品)使用に必要
#pragma comment(lib,"dinput8.lib")		//入力処理に必要
#pragma comment(lib,"xinput.lib")		//pad入力処理に必要
#pragma comment(lib, "winmm.lib")		//fps表示に必要
#pragma comment(lib, "XAudio2.lib")		//3Dオーディオ

//========================================================
//マクロ定義
//========================================================
#define SCREEN_WIDTH (1280)		//ウィンドウの幅
#define SCREEN_HEIGHT (720)		//ウィンドウの高さ
#define FPS (60)

//========================================================
//画面（モード）の種類
//========================================================

//========================================================
//プロトタイプ宣言
//========================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//行列を使ったベクトルの変換
D3DXVECTOR3 VTransform(D3DXVECTOR3 InV, D3DXMATRIX InM);
bool calcRaySphere(float lx, float ly, float lz, float vx, float vy, float vz, float px, float py, float pz, float r, float &q1x, float &q1y, float &q1z, float &q2x, float &q2y, float &q2z);	// レイとカプセルの貫通点を算出
bool calcRayInfCilinder(float lx, float ly, float lz, float vx, float vy, float vz, float p1x, float p1y, float p1z, float p2x, float p2y, float p2z, float r, float &q1x, float &q1y, float &q1z, float &q2x, float &q2y, float &q2z);	// レイと円柱の貫通点を算出
bool calcRayCapsule(float lx, float ly, float lz, float vx, float vy, float vz, float p1x, float p1y, float p1z, float p2x, float p2y, float p2z, float r, float &q1x, float &q1y, float &q1z, float &q2x, float &q2y, float &q2z);	// レイとカプセルの貫通点を算出
float checkDot(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);	// ∠P1P2P3の内積を算出

#ifdef _DEBUG
int GetFPS(void);
#endif

#endif // !_MAIN_H_