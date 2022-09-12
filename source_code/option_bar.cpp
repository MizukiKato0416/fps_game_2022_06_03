//=============================================================================
//
// 設定画面のバーの処理 [option_bar.cpp]
// Author :加藤瑞葵
//
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include "option_bar.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include "input_mouse.h"
#include "counter.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define OPTION_BAR_COUNETR_POS		(25.0f)									//数値UI位置調節値
#define OPTION_BAR_COUNETR_SIZE		(D3DXVECTOR3(20.0f, 30.0f, 0.0f))		//数値UIサイズ
#define OPTION_BAR_COUNETR_NUM		(3)										//数値UI桁数

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// コンストラクタ
//=============================================================================
COptionBar::COptionBar(CObject::PRIORITY Priority) : CObject(Priority)
{
	m_pos = { 0.0f, 0.0f, 0.0f };
	m_barSize = { 0.0f, 0.0f, 0.0f };
	m_circleSize = { 0.0f, 0.0f, 0.0f };
	m_fMin = 0.0f;
	m_fMax = 0.0f;
	m_fDefault = 0.0f;
	m_fNum = 0.0f;
	m_pCircle = nullptr;
	m_pBar = nullptr;
	m_bClick = false;
	m_pCounter = nullptr;
	m_bDefault = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
COptionBar::~COptionBar()
{
	
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT COptionBar::Init(void)
{
	//変数初期化
	m_bClick = false;
	m_fNum = m_fDefault;
	m_bDefault = false;

	SetObjType(CObject::OBJTYPE::UI);

	//バーの生成
	m_pBar = CObject2D::Create(m_pos, m_barSize, (int)CObject::PRIORITY::OPTION);

	//数値の生成
	m_pCounter = CCounter::Create(D3DXVECTOR3(m_pos.x + m_barSize.x / 2.0f + OPTION_BAR_COUNETR_POS, m_pos.y , 0.0f),
		                          OPTION_BAR_COUNETR_SIZE,
		                          OPTION_BAR_COUNETR_NUM, "Number.png");
	//数値の設定
	m_pCounter->SetCounterNum(int(m_fDefault));
	m_pCounter->SetPriority((int)CObject::PRIORITY::OPTION);


	//円の生成
	m_pCircle = CObject2D::Create(m_pos, m_circleSize, (int)CObject::PRIORITY::OPTION);
	m_pCircle->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("circle.png"));

	//円の位置保存用
	D3DXVECTOR3 circlePos = m_pos;

	//最大値と最小値の差分を求める
	float fDiffer = m_fMax - m_fMin;
	//デフォルトの円の位置がどこにあるかを求めるためにバーに対してどこにあるのか比率を求める
	float fDefaultRatio = (m_fDefault - m_fMin) * 100.0f / fDiffer;
	//出した比率で場所を求める
	circlePos.x = (m_pos.x - m_barSize.x / 2.0f) + (m_barSize.x * (fDefaultRatio / 100.0f));
	//円の位置を設定
	m_pCircle->SetPos(circlePos, m_pCircle->GetSize());

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void COptionBar::Uninit(void)
{
	//円の削除
	if (m_pCircle != nullptr)
	{
		m_pCircle->Uninit();
		m_pCircle = nullptr;
	}

	//バーの削除
	if (m_pBar != nullptr)
	{
		m_pBar->Uninit();
		m_pBar = nullptr;
	}

	//数値の削除
	if (m_pCounter != nullptr)
	{
		m_pCounter->Uninit();
		m_pCounter = nullptr;
	}

	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void COptionBar::Update(void)
{
	//マウス取得処理
	CInputMouse *pInputMouse;
	pInputMouse = CManager::GetInstance()->GetInputMouse();

	//クリックしたら
	if (pInputMouse->GetPress(CInputMouse::MOUSE_TYPE::MOUSE_TYPE_LEFT) == true)
	{
		//マウスカーソルの位置取得
		POINT mouse_pos;
		GetCursorPos(&mouse_pos);
		
		HWND hWind = CManager::GetWindowHandle();

		//RECT lprc = {};
		//if (GetClientRect(hWind, &lprc))
		//{
		//	SetWindowPos(
		//	// ウィンドウハンドル
		//	hWind,
		//	// 配置順序のハンドル(NULLでよし)
		//	NULL,
		//	// 表示座標X
		//	CW_USEDEFAULT,
		//	// 表示座標Y
		//	CW_USEDEFAULT,
		//	// リサイズ横幅		
		//	lprc.right,
		//	// リサイズ縦幅
		//	lprc.bottom,
		//	// SWP_NOMOVE => 位置変更なし
		//	SWP_NOMOVE);
		//}

		ScreenToClient(hWind, &mouse_pos);
		D3DXVECTOR2 mousePos = D3DXVECTOR2((float)mouse_pos.x, (float)mouse_pos.y);

		//円の位置とサイズ取得
		D3DXVECTOR3 circlePos = m_pCircle->GetPos();
		D3DXVECTOR3 circleSize = m_pCircle->GetSize();

		//クリックしたら
		if (pInputMouse->GetTrigger(CInputMouse::MOUSE_TYPE::MOUSE_TYPE_LEFT) == true)
		{
			//円の中をクリックしていたら
			if (mousePos.x >= circlePos.x - circleSize.x / 2.0f && mousePos.x <= circlePos.x + circleSize.x / 2.0f &&
				mousePos.y >= circlePos.y - circleSize.y / 2.0f && mousePos.y <= circlePos.y + circleSize.y / 2.0f)
			{
				//クリックしていない状態なら
				if (!m_bClick)
				{
					//クリックした状態にする
					m_bClick = true;
				}
			}
		}

		//クリックしている状態なら
		if (m_bClick)
		{
			//円のX位置をマウスのX位置にする
			circlePos.x = mousePos.x;

			//バーの位置、サイズを取得
			D3DXVECTOR3 barPos = m_pBar->GetPos();
			D3DXVECTOR3 barSize = m_pBar->GetSize();

			//円の位置がバーの左端よりも小さくなったら
			if (circlePos.x < barPos.x - barSize.x / 2.0f)
			{
				//円の位置がバーの左端にする
				circlePos.x = barPos.x - barSize.x / 2.0f;
			}
			else if (circlePos.x > barPos.x + barSize.x / 2.0f)
			{//円の位置がバーの右端よりも小さくなったら
				//円の位置がバーの右端にする
				circlePos.x = barPos.x + barSize.x / 2.0f;
			}
			
			//バーの大きさに対する現在の円の位置の比率を求める
			float fRatio = (circlePos.x - (barPos.x - barSize.x / 2.0f)) /
				           ((barPos.x + barSize.x / 2.0f) - (barPos.x - barSize.x / 2.0f));

			//比率から現在の値を求める
			m_fNum = (m_fMax - m_fMin) * fRatio + m_fMin;

			//円の位置を設定
			m_pCircle->SetPos(circlePos, m_pCircle->GetSize());
		}
	}

	//離したら
	if (pInputMouse->GetRelease(CInputMouse::MOUSE_TYPE::MOUSE_TYPE_LEFT) == true)
	{
		//クリックしている状態なら
		if (m_bClick)
		{
			//クリックしていない状態にする
			m_bClick = false;
		}
	}

	//数値UIが生成されていたら
	if (m_pCounter != nullptr)
	{
		//数値の設定
		m_pCounter->SetCounterNum(int(m_fNum));
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void COptionBar::Draw(void)
{
	
}

//=============================================================================
// 生成
//=============================================================================
COptionBar *COptionBar::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &barSize, const D3DXVECTOR3 &circleSize,
		                       const float &fMin, const float &fMax, const float &fDefault)
{
	//インスタンスの生成
	COptionBar *pOptionBar = nullptr;
	if (pOptionBar == nullptr)
	{
		pOptionBar = new COptionBar();
		if (pOptionBar != nullptr)
		{
			pOptionBar->m_pos = pos;
			pOptionBar->m_barSize = barSize;
			pOptionBar->m_circleSize = circleSize;
			pOptionBar->m_fMin = fMin;
			pOptionBar->m_fMax = fMax;
			pOptionBar->m_fDefault = fDefault;
			pOptionBar->Init();
		}
	}
	return pOptionBar;
}

//=============================================================================
//現在の値設定処理
//=============================================================================
void COptionBar::SetNum(const float & fNum)
{
	m_fNum = fNum;

	//最大値と最小値の差分を求める
	float fDiffer = m_fMax - m_fMin;
	//デフォルトの円の位置がどこにあるかを求めるためにバーに対してどこにあるのか比率を求める
	float fDefaultRatio = (m_fNum - m_fMin) * 100.0f / fDiffer;
	//円の位置取得
	D3DXVECTOR3 circlePos = m_pCircle->GetPos();
	//出した比率で場所を求める
	circlePos.x = (m_pos.x - m_barSize.x / 2.0f) + (m_barSize.x * (fDefaultRatio / 100.0f));
	//円の位置を設定
	m_pCircle->SetPos(circlePos, m_pCircle->GetSize());
}
