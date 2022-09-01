//=============================================================================
//
// メイン処理 [main.cpp]
// Author : 加藤瑞葵
//
//=============================================================================
#include "main.h"
#include "manager.h"
#include "sound.h"

//===========================================================
// マクロ定義
//===========================================================
#define CLASS_NAME		"AppClass"			// ウインドウのクラス名
#define WINDOW_NAME		"Yuggdracil"		// ウインドウのキャプション名

//===========================================================
// プロトタイプ宣言
//===========================================================

//===========================================================
// グローバル変数:
//===========================================================

#ifdef _DEBUG
int	g_nCountFPS;	// FPSカウンタ
#endif

//=============================================================================
// メイン関数
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	HWND hWnd;
	MSG msg;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	
	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// 指定したクライアント領域を確保するために必要なウィンドウ座標を計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウの作成
	hWnd = CreateWindow(CLASS_NAME,
						WINDOW_NAME,
						WS_OVERLAPPEDWINDOW,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						(rect.right - rect.left),
						(rect.bottom - rect.top),
						NULL,
						NULL,
						hInstance,
						NULL);

	//初期化処理(ウィンドウを生成してから行う)(DirectX本体の生成を行う)
	if (FAILED(CManager::GetInstance()->Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	// 分解能を設定
	timeBeginPeriod(1);

	// フレームカウント初期化
	dwCurrentTime =
	dwFrameCount = 0;
	dwExecLastTime = 
	dwFPSLastTime = timeGetTime();

	// ウインドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// メッセージループ
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();	// 現在の時間を取得
			if((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5秒ごとに実行
#ifdef _DEBUG
				// FPSを算出
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
				dwFPSLastTime = dwCurrentTime;	// 現在の時間を保存
				dwFrameCount = 0;
			}

			if((dwCurrentTime - dwExecLastTime) >= (1000 / FPS))
			{// 1/60秒経過
				dwExecLastTime = dwCurrentTime;	// 現在の時間を保存

				//新しいウィンドウハンドル
				HWND newhWind;
				//ウィンドウのサイズ情報を入れる変数
				RECT lprc = {};
				//ウィンドウのサイズを取得
				if (GetClientRect(hWnd, &lprc))
				{
					//ウィンドウのサイズが規定から変更されていたら
					if (lprc.right != SCREEN_WIDTH || lprc.bottom != SCREEN_HEIGHT)
					{
						//ウィンドウを新しく生成する
						newhWind = CreateWindow(CLASS_NAME,
							                    WINDOW_NAME,
							                    WS_OVERLAPPEDWINDOW,
							                    CW_USEDEFAULT,
							                    CW_USEDEFAULT,
							                    (rect.right - rect.left),
							                    (rect.bottom - rect.top),
							                    NULL,
							                    NULL,
							                    hInstance,
							                    NULL);

						// ウインドウの表示
						ShowWindow(newhWind, nCmdShow);
						UpdateWindow(newhWind);

						//ウィンドウの削除
						//DestroyWindow(hWnd);
						//上書き
						//hWnd = newhWind;
						//マネージャーに設定
						CManager::GetInstance()->SetWindowHandle(hWnd);
						//音を鳴らす
						CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL::COUNT_DOWN_SE);
					}
				}

				// 更新処理
				CManager::GetInstance()->Update();

				// 描画処理
				CManager::GetInstance()->Draw();

				dwFrameCount++;
			}
		}
	}

	if (CManager::GetInstance() != NULL)
	{
		// 終了処理
		CManager::GetInstance()->Uninit();
	}

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 分解能を戻す
	timeEndPeriod(1);

	return (int)msg.wParam;
}

//=============================================================================
// ウインドウプロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_CREATE:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:				// [ESC]キーが押された

			//ゲーム中でなかったら
			if (CManager::GetInstance()->GetMode() != CManager::MODE::GAME01)
			{
				DestroyWindow(hWnd);	// ウィンドウを破棄するよう指示する
			}
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=======================================================================
//行列を使ったベクトルの変換
//=======================================================================
D3DXVECTOR3 VTransform(D3DXVECTOR3 InV, D3DXMATRIX InM)
{
	D3DXVECTOR3 tmp;
	tmp.x = InV.x * InM.m[0][0] + InV.y * InM.m[1][0] + InV.z * InM.m[2][0] + InM.m[3][0];
	tmp.y = InV.x * InM.m[0][1] + InV.y * InM.m[1][1] + InV.z * InM.m[2][1] + InM.m[3][1];
	tmp.z = InV.x * InM.m[0][2] + InV.y * InM.m[1][2] + InV.z * InM.m[2][2] + InM.m[3][2];

	return tmp;
}

//------------------------
// ∠P1P2P3の内積を算出
//------------------------
float checkDot(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{
	return (x1 - x2) * (x3 - x2) + (y1 - y2) * (y3 - y2) + (z1 - z2) * (z3 - z2);
}

//------------------------
// レイとカプセルの貫通点を算出
//------------------------
bool calcRayCapsule(float lx, float ly, float lz,
	float vx, float vy, float vz,
	float p1x, float p1y, float p1z,
	float p2x, float p2y, float p2z,
	float r,
	float &q1x, float &q1y, float &q1z,
	float &q2x, float &q2y, float &q2z)
{
	bool Q1inP1 = false;
	bool Q1inP2 = false;
	bool Q1inCld = false;

	// Q1の検査
	if (calcRaySphere(lx, ly, lz, vx, vy, vz, p1x, p1y, p1z, r, q1x, q1y, q1z, q2x, q2y, q2z) == true &&
		checkDot(p2x, p2y, p2z, p1x, p1y, p1z, q1x, q1y, q1z) <= 0.0f)
	{
		Q1inP1 = true; // Q1は球面P1上にある
	}
	else if (calcRaySphere(lx, ly, lz, vx, vy, vz, p2x, p2y, p2z, r, q1x, q1y, q1z, q2x, q2y, q2z) == true &&
		checkDot(p1x, p1y, p1z, p2x, p2y, p2z, q1x, q1y, q1z) <= 0.0f)
	{
		Q1inP2 = true; // Q1は球面P2上にある
	}
	else if (calcRayInfCilinder(lx, ly, lz, vx, vy, vz, p1x, p1y, p1z, p2x, p2y, p2z, r, q1x, q1y, q1z, q2x, q2y, q2z) == true &&
		checkDot(p1x, p1y, p1z, p2x, p2y, p2z, q1x, q1y, q1z) > 0.0f &&
		checkDot(p2x, p2y, p2z, p1x, p1y, p1z, q1x, q1y, q1z) > 0.0f)
	{
		Q1inCld = true; // Q1は円柱面にある
	}
	else
	{
		return false; // レイは衝突していない
	}

	// Q2の検査
	float tx, ty, tz; // ダミー
	if (Q1inP1 && checkDot(p2x, p2y, p2z, p1x, p1y, p1z, q2x, q2y, q2z) <= 0.0f)
	{
		// Q1、Q2共球P1上にある
		return true;
	}
	else if (Q1inP2 && checkDot(p1x, p1y, p1z, p2x, p2y, p2z, q2x, q2y, q2z) <= 0.0f)
	{
		// Q1、Q2共球P2上にある
		return true;
	}
	else if (Q1inCld &&
		checkDot(p1x, p1y, p1z, p2x, p2y, p2z, q2x, q2y, q2z) > 0.0f &&
		checkDot(p2x, p2y, p2z, p1x, p1y, p1z, q2x, q2y, q2z) > 0.0f)
	{
		// Q1、Q2共球円柱面にある
		return true;
	}
	else if (calcRaySphere(lx, ly, lz, vx, vy, vz, p1x, p1y, p1z, r, tx, ty, tz, q2x, q2y, q2z) == true &&
		checkDot(p2x, p2y, p2z, p1x, p1y, p1z, q2x, q2y, q2z) <= 0.0f)
	{
		// Q2は球P1上にある
		return true;
	}
	else if (calcRaySphere(lx, ly, lz, vx, vy, vz, p2x, p2y, p2z, r, tx, ty, tz, q2x, q2y, q2z) == true &&
		checkDot(p1x, p1y, p1z, p2x, p2y, p2z, q2x, q2y, q2z) <= 0.0f)
	{
		// Q2は球P2上にある
		return true;
	}

	// Q2が円柱上にある事が確定
	calcRayInfCilinder(lx, ly, lz, vx, vy, vz, p1x, p1y, p1z, p2x, p2y, p2z, r, tx, ty, tz, q2x, q2y, q2z);

	return true;
}

//------------------------
// レイと円柱の貫通点を算出
//------------------------
bool calcRayInfCilinder(float lx, float ly, float lz,
	float vx, float vy, float vz,
	float p1x, float p1y, float p1z,
	float p2x, float p2y, float p2z,
	float r,
	float &q1x, float &q1y, float &q1z,
	float &q2x, float &q2y, float &q2z)
{
	float px = p1x - lx;
	float py = p1y - ly;
	float pz = p1z - lz;
	p2x = p2x - lx;
	p2y = p2y - ly;
	p2z = p2z - lz;
	float sx = p2x - px;
	float sy = p2y - py;
	float sz = p2z - pz;

	// 各種内積値
	float Dvv = vx * vx + vy * vy + vz * vz;
	float Dsv = sx * vx + sy * vy + sz * vz;
	float Dpv = px * vx + py * vy + pz * vz;
	float Dss = sx * sx + sy * sy + sz * sz;
	float Dps = px * sx + py * sy + pz * sz;
	float Dpp = px * px + py * py + pz * pz;
	float rr = r * r;

	if (Dss == 0.0f)
		return false; // 円柱が定義されない

	float A = Dvv - Dsv * Dsv / Dss;
	float B = Dpv - Dps * Dsv / Dss;
	float C = Dpp - Dps * Dps / Dss - rr;

	if (A == 0.0f)
		return false;

	float s = B * B - A * C;
	if (s < 0.0f)
		return false; // レイが円柱と衝突していない
	s = sqrtf(s);

	float a1 = (B - s) / A;
	float a2 = (B + s) / A;

	q1x = lx + a1 * vx;
	q1y = ly + a1 * vy;
	q1z = lz + a1 * vz;
	q2x = lx + a2 * vx;
	q2y = ly + a2 * vy;
	q2z = lz + a2 * vz;

	return true;
}

//------------------------
// lx, ly, lz : レイの始点
//------------------------
bool calcRaySphere(float lx, float ly, float lz,
	float vx, float vy, float vz,
	float px, float py, float pz,
	float r,
	float &q1x, float &q1y, float &q1z,
	float &q2x, float &q2y, float &q2z)
{
	px = px - lx;
	py = py - ly;
	pz = pz - lz;

	float A = vx * vx + vy * vy + vz * vz;
	float B = vx * px + vy * py + vz * pz;
	float C = px * px + py * py + pz * pz - r * r;

	if (A == 0.0f)
	{
		return false; // レイの長さが0
	}

	float s = B * B - A * C;
	if (s < 0.0f)
	{
		return false; // 衝突していない
	}

	s = sqrtf(s);
	float a1 = (B - s) / A;
	float a2 = (B + s) / A;

	if (a1 < 0.0f || a2 < 0.0f)
	{
		return false; // レイの反対で衝突
	}

	q1x = lx + a1 * vx;
	q1y = ly + a1 * vy;
	q1z = lz + a1 * vz;
	q2x = lx + a2 * vx;
	q2y = ly + a2 * vy;
	q2z = lz + a2 * vz;

	return true;
}

#ifdef _DEBUG
//=============================================================================
// FPS取得処理
//=============================================================================
int GetFPS(void)
{
	return g_nCountFPS;
}
#endif