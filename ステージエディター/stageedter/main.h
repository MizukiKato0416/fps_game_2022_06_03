//=============================================================================
//
// メイン処理 [main.h]
// Author : 羽鳥太一
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include "d3dx9.h"

using namespace std;

#define DIRECTINPUT_VERSION (0x0800) //ビルド時の警告対処用マクロ
#include "dinput.h"	//入力処理に必要
#include "xaudio2.h"

//*****************************************************************************
// ライブラリファイルのリンク
//*****************************************************************************
#pragma comment(lib,"d3d9.lib")		//描画に必要
#pragma comment(lib,"d3dx9.lib")	//[d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")	//DirectXコンポーネント（部品）使用に必要
#pragma comment (lib, "winmm.lib")
#pragma comment(lib,"dinput8.lib")	//入力処理に必要

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCREEN_WIDTH (1280) //ウィンドウの幅
#define SCREEN_HEIGHT (720) //ウィンドウの高さ
#define SCREEN_CENTER_X (SCREEN_WIDTH / 2)	// スクリーンの真ん中
#define SCREEN_CENTER_Y (SCREEN_HEIGHT / 2)	// スクリーンの真ん中
#define FPS (60)		// FPSの値
#define PASCHAR_MAX (64)	// char配列用

//*****************************************************************************
//プロトタイプ宣言
//*****************************************************************************
int GetFPS(void);

#endif