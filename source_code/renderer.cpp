//================================================
//描画処理
//Author:加藤瑞葵
//================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "renderer.h"
#include "object.h"
#include "manager.h"
#include "camera.h"

//================================================
//静的メンバ変数宣言
//================================================

//================================================
//デフォルトコンストラクタ
//================================================
CRenderer::CRenderer()
{
	
}

//================================================
//オーバーロードされたコンストラクタ
//================================================


//================================================
//デストラクタ
//================================================
CRenderer::~CRenderer()
{

}

//================================================
//初期化処理
//================================================
HRESULT CRenderer::Init(const HWND &hWnd, const bool &bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;									// バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;						// カラーモードの指定
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;									// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// デバイスの生成
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}
	
	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// カリングを行う
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用

	//透明度の設定をできるようにしている
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

#ifdef _DEBUG
	// デバッグ情報表示用フォントの生成
	D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
#endif //!_DEBUG

	return S_OK;
}

//================================================
//終了処理
//================================================
void CRenderer::Uninit(void)
{
#ifdef _DEBUG
	// デバッグ情報表示用フォントの破棄
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
#endif //!_DEBUG

	// デバイスの破棄
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//================================================
//更新処理
//================================================
void CRenderer::Update(void)
{
	// ポリゴンの更新処理
	CObject::UpdateAll();
}

//================================================
//描画処理
//================================================
void CRenderer::Draw(void)
{
	// Direct3Dによる描画の開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		D3DVIEWPORT9 viewportDef;
		//現在のビューポートを取得
		m_pD3DDevice->GetViewport(&viewportDef);

		//カメラのポインタ配列1番目のアドレス取得
		CCamera** pCameraAddress = CManager::GetInstance()->GetCamera();

		//使うcameraの数
		int nNumCamera = MAX_CAMERA;

		for (int nCntCamera = 0; nCntCamera < nNumCamera; nCntCamera++, pCameraAddress++)
		{
			//cameraの取得
			CCamera* pCamera = &**pCameraAddress;
			if (pCamera != nullptr)
			{
				//ビューポートの設定
				m_pD3DDevice->SetViewport(pCamera->GetViewPort());

				//デバックバッファ&Zバッファのクリア
				m_pD3DDevice->Clear(0,
									NULL,
									(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
									D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

				pCamera->Set();

				// ポリゴンの描画処理
				CObject::DrawAll();
#ifdef _DEBUG
				// FPS表示
				DrawFPS();
				DrawFrame();
#endif //!_DEBUG
			}
		}
		//ビューポートを元に戻す
		m_pD3DDevice->SetViewport(&viewportDef);
		// Direct3Dによる描画の終了
		m_pD3DDevice->EndScene();
	}
	//バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}


#ifdef _DEBUG
//=============================================================================
// FPS表示
//=============================================================================
void CRenderer::DrawFPS(void)
{
	int nCountFPS = 0;
	nCountFPS = GetFPS();
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[256];

	wsprintf(str, "FPS:%d\n", nCountFPS);

	// テキスト描画
	m_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}

//=============================================================================
// 経過フレーム数表示
//=============================================================================
void CRenderer::DrawFrame(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char strtage[1000];
	CCamera** pCameraAddress = CManager::GetInstance()->GetCamera();
	//pCameraAddress++;
	CCamera* pCamera = &**pCameraAddress;
	D3DXVECTOR3 posV = pCamera->GetPosV();

	sprintf(&strtage[0], "視点X:%f\nY:%f\nZ:%f\n", posV.x, posV.y, posV.z);

	// テキスト描画
	m_pFont->DrawText(NULL, strtage, -1, &rect, DT_RIGHT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}
#endif //!_DEBUG