//=============================================================================
//
// 設定画面の処理 [option.cpp]
// Author :加藤瑞葵
//
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include "option.h"
#include "option_bar.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include "player.h"
#include "input_mouse.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define OPTION_MOUSE_SENSI_MIN			(30.0f)												//マウスの感度をデフォルトから何パーセントまで下げれるようにするか
#define OPTION_MOUSE_SENSI_MAX			(170.0f)											//マウスの感度をデフォルトから何パーセントまで上げれるようにするか
#define OPTION_MOUSE_SENSI_DEFAULT		(100.0f)											//マウスの感度の最初の設定をデフォルトの何パーセントにするか
#define OPTION_BAR_SIZE					(D3DXVECTOR3(600.0f, 5.0f, 0.0f))					//バーのサイズ
#define OPTION_BAR_POS_01				(D3DXVECTOR3(710.0f, 285.0f, 0.0f))					//バーの位置
#define OPTION_BAR_POS_02				(D3DXVECTOR3(710.0f, 355.0f, 0.0f))					//バーの位置
#define OPTION_CIRCLE_SIZE				(D3DXVECTOR3(30.0f, 30.0f, 0.0f ))					//円のサイズ
#define OPTION_SELECT_COLOR_ALPHA		(0.5f)												//選択で薄くなる時のα値
#define OPTION_DEFAULT_UI_POS			(D3DXVECTOR3(300.0f, 650.0f, 0.0f))					//デフォルトに戻すUIの位置
#define OPTION_DEFAULT_UI_SIZE			(D3DXVECTOR3(414.0f, 76.0f, 0.0f))					//デフォルトに戻すUIのサイズ
#define OPTION_TUTORIAL_UI_POS			(D3DXVECTOR3(SCREEN_WIDTH - 300.0f, 650.0f, 0.0f))	//操作説明を表示するUIの位置
#define OPTION_CLOSE_TUTORIAL_UI_POS	(D3DXVECTOR3(SCREEN_WIDTH - 100.0f, 50.0f, 0.0f))	//操作説明消すUIの位置
#define OPTION_CLOSE_TUTORIAL_UI_SIZE	(D3DXVECTOR3(53.0f, 51.0f, 0.0f))					//操作説明消すUIのサイズ

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// コンストラクタ
//=============================================================================
COption::COption(CObject::PRIORITY Priority) : CObject(Priority)
{
	memset(m_apOptionBar, NULL, sizeof(m_apOptionBar[OPTION_BAR_NUM]));
	memset(m_aNum, 0.0f, sizeof(m_aNum[OPTION_BAR_NUM]));
	m_pOptionFrame = nullptr;
	m_bOpen = false;
	m_pDefaultUi = nullptr;
	m_pTutorialUi = nullptr;
	m_pTutorial = nullptr;
	m_pCloseTutorialUi = nullptr;
}

//=============================================================================
// デストラクタ
//=============================================================================
COption::~COption()
{
	
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT COption::Init(void)
{
	//変数初期化
	m_bOpen = false;
	memset(m_apOptionBar, NULL, sizeof(m_apOptionBar[OPTION_BAR_NUM]));
	m_pOptionFrame = nullptr;
	m_pDefaultUi = nullptr;
	m_aNum[0] = OPTION_MOUSE_SENSI_DEFAULT;
	m_aNum[1] = OPTION_MOUSE_SENSI_DEFAULT;
	m_pTutorialUi = nullptr;
	m_pCloseTutorialUi = nullptr;
	m_pTutorial = nullptr;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void COption::Uninit(void)
{
	//設定を閉じたときの処理
	Close();

	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void COption::Update(void)
{
	//開いている状態だったら
	if (m_bOpen)
	{
		//チュートリアル画像が生成されていなかったら
		if (m_pTutorial == nullptr && m_pCloseTutorialUi == nullptr)
		{
			//マウス感度設定処理
			MouseSensi();

			//ADS感度設定処理
			AdsSensi();

			//デフォルトに戻す処理
			Default();

			//チュートリアルに行く処理
			Tutorial();
		}
		else
		{//生成されていたら
			//チュートリアルを消す処理
			TutorialCancel();
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void COption::Draw(void)
{
	
}

//=============================================================================
// 生成
//=============================================================================
COption *COption::Create(void)
{
	//インスタンスの生成
	COption *pOptionBar = nullptr;
	if (pOptionBar == nullptr)
	{
		pOptionBar = new COption();
		if (pOptionBar != nullptr)
		{
			pOptionBar->Init();
		}
	}
	return pOptionBar;
}

//=============================================================================
//設定を開いた時の処理
//=============================================================================
void COption::Open(void)
{
	//開いている状態ではなかったら
	if (!m_bOpen)
	{
		//開いている状態にする
		m_bOpen = true;

		//フレームの生成
		m_pOptionFrame = CObject2D::Create({ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f }, { SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f },
			                               (int)CObject::PRIORITY::OPTION);
		m_pOptionFrame->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("game_option.png"));

		//バーの生成
		m_apOptionBar[0] = COptionBar::Create(OPTION_BAR_POS_01, OPTION_BAR_SIZE, OPTION_CIRCLE_SIZE,
			                                  OPTION_MOUSE_SENSI_MIN, OPTION_MOUSE_SENSI_MAX, OPTION_MOUSE_SENSI_DEFAULT);
		//初期値の設定
		m_apOptionBar[0]->SetNum(m_aNum[0]);
		//値の保存
		m_aNum[0] = m_apOptionBar[0]->GetNum();

		//バーの生成
		m_apOptionBar[1] = COptionBar::Create(OPTION_BAR_POS_02, OPTION_BAR_SIZE, OPTION_CIRCLE_SIZE,
			                                  OPTION_MOUSE_SENSI_MIN, OPTION_MOUSE_SENSI_MAX, OPTION_MOUSE_SENSI_DEFAULT);
		//初期値の設定
		m_apOptionBar[1]->SetNum(m_aNum[1]);
		//値の保存
		m_aNum[1] = m_apOptionBar[1]->GetNum();

		//デフォルトに戻すUIの生成
		m_pDefaultUi = CObject2D::Create(OPTION_DEFAULT_UI_POS, OPTION_DEFAULT_UI_SIZE,
			                             (int)CObject::PRIORITY::OPTION);
		m_pDefaultUi->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("default_ui.png"));

		//チュートリアルに行くUIの生成
		m_pTutorialUi = CObject2D::Create(OPTION_TUTORIAL_UI_POS, OPTION_DEFAULT_UI_SIZE,
			                              (int)CObject::PRIORITY::OPTION);
		m_pTutorialUi->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("tutorial_ui.png"));
	}
}

//=============================================================================
//設定を閉じた時の処理
//=============================================================================
void COption::Close(void)
{
	//開いている状態だったら
	if (m_bOpen)
	{
		//閉じている状態にする
		m_bOpen = false;

		//設定のバーの削除
		for (int nCntBar = 0; nCntBar < OPTION_BAR_NUM; nCntBar++)
		{
			if (m_apOptionBar[nCntBar] != nullptr)
			{
				m_apOptionBar[nCntBar]->Uninit();
				m_apOptionBar[nCntBar] = nullptr;
			}
		}

		//設定画面のフレームの削除
		if (m_pOptionFrame != nullptr)
		{
			m_pOptionFrame->Uninit();
			m_pOptionFrame = nullptr;
		}

		//デフォルトに戻すUIの削除
		if (m_pDefaultUi != nullptr)
		{
			m_pDefaultUi->Uninit();
			m_pDefaultUi = nullptr;
		}

		//チュートリアルの削除
		if (m_pTutorial != nullptr)
		{
			m_pTutorial->Uninit();
			m_pTutorial = nullptr;
		}

		//チュートリアルを写すUIの削除
		if (m_pTutorialUi != nullptr)
		{
			m_pTutorialUi->Uninit();
			m_pTutorialUi = nullptr;
		}

		//チュートリアルを閉じるUIの削除
		if (m_pCloseTutorialUi != nullptr)
		{
			m_pCloseTutorialUi->Uninit();
			m_pCloseTutorialUi = nullptr;
		}
	}
}

//=============================================================================
//マウス感度設定処理
//=============================================================================
void COption::MouseSensi(void)
{
	if (m_apOptionBar[0] != nullptr)
	{
		//オブジェクト情報を入れるポインタ
		vector<CObject*> object;

		//先頭のポインタを代入
		object = CObject::GetObject(static_cast<int>(CObject::PRIORITY::PLAYER));
		int nProprty_Size = object.size();

		for (int nCnt = 0; nCnt < nProprty_Size; nCnt++)
		{
			//オブジェクトの種類がプレイヤーだったら
			if (object[nCnt]->GetObjType() == CObject::OBJTYPE::PLAYER)
			{
				//プレイヤーにキャスト
				CPlayer *pPlayerObj = nullptr;
				pPlayerObj = (CPlayer*)object[nCnt];

				//現在の設定の値取得
				float fNum = m_apOptionBar[0]->GetNum();
				D3DXVECTOR2 cameraSpeed = { PLAYER_CAMERA_V_MOUSE_SPEED_XZ, PLAYER_CAMERA_V_MOUSE_SPEED_Y };

				//値をスピードの反映
				cameraSpeed *= fNum / 100.0f;

				//設定
				pPlayerObj->SetCameraSpeed(cameraSpeed);

				//値の保存
				m_aNum[0] = m_apOptionBar[0]->GetNum();
			}
		}
	}
}

//=============================================================================
//ADS感度設定処理
//=============================================================================
void COption::AdsSensi(void)
{
	if (m_apOptionBar[1] != nullptr)
	{
		//オブジェクト情報を入れるポインタ
		vector<CObject*> object;

		//先頭のポインタを代入
		object = CObject::GetObject(static_cast<int>(CObject::PRIORITY::PLAYER));
		int nProprty_Size = object.size();

		for (int nCnt = 0; nCnt < nProprty_Size; nCnt++)
		{
			//オブジェクトの種類がプレイヤーだったら
			if (object[nCnt]->GetObjType() == CObject::OBJTYPE::PLAYER)
			{
				//プレイヤーにキャスト
				CPlayer *pPlayerObj = nullptr;
				pPlayerObj = (CPlayer*)object[nCnt];

				//現在の設定の値取得
				float fNum = m_apOptionBar[1]->GetNum();
				D3DXVECTOR2 adsCameraSpeed = { PLAYER_ADS_CAMERA_V_MOUSE_SPEED_XZ, PLAYER_ADS_CAMERA_V_MOUSE_SPEED_Y };

				//値をスピードの反映
				adsCameraSpeed *= fNum / 100.0f;

				//設定
				pPlayerObj->SetAdsCameraSpeed(adsCameraSpeed);

				//値の保存
				m_aNum[1] = m_apOptionBar[1]->GetNum();
			}
		}
	}
}

//=============================================================================
//デフォルトに戻す処理
//=============================================================================
void COption::Default(void)
{
	//マウス取得処理
	CInputMouse *pInputMouse;
	pInputMouse = CManager::GetInstance()->GetInputMouse();

	//マウスカーソルの位置取得
	POINT mouse_pos;
	GetCursorPos(&mouse_pos);
	HWND hWind = CManager::GetWindowHandle();
	ScreenToClient(hWind, &mouse_pos);
	D3DXVECTOR2 mousePos = D3DXVECTOR2((float)mouse_pos.x, (float)mouse_pos.y);

	//デフォルトUIの位置とサイズ取得
	D3DXVECTOR3 uiPos = m_pDefaultUi->GetPos();
	D3DXVECTOR3 uiSize = m_pDefaultUi->GetSize();

	//UIの中をにあったら
	if (mousePos.x >= uiPos.x - uiSize.x / 2.0f && mousePos.x <= uiPos.x + uiSize.x / 2.0f &&
		mousePos.y >= uiPos.y - uiSize.y / 2.0f && mousePos.y <= uiPos.y + uiSize.y / 2.0f)
	{
		//色取得
		D3DXCOLOR uiCol = m_pDefaultUi->GetCol();
		//薄くする
		uiCol.a = OPTION_SELECT_COLOR_ALPHA;
		//色設定
		m_pDefaultUi->SetCol(uiCol);

		//クリックしたら
		if (pInputMouse->GetTrigger(CInputMouse::MOUSE_TYPE::MOUSE_TYPE_LEFT) == true)
		{
			//設定の値を全てデフォルトの状態にする
			for (int nCntBar = 0; nCntBar < OPTION_BAR_NUM; nCntBar++)
			{
				if (m_apOptionBar[nCntBar] != nullptr)
				{
					//値をデフォルトに設定する
					m_apOptionBar[nCntBar]->SetNum(m_apOptionBar[nCntBar]->GetDefaultNum());
					//値の保存
					m_aNum[nCntBar] = m_apOptionBar[nCntBar]->GetNum();
					//デフォルトに戻す状態に設定する
					m_apOptionBar[nCntBar]->SetDefault(true);
				}
			}
		}
	}
	else
	{
		//色取得
		D3DXCOLOR uiCol = m_pDefaultUi->GetCol();
		//色を元に戻す
		uiCol.a = 1.0f;
		//色設定
		m_pDefaultUi->SetCol(uiCol);
	}
}

//=============================================================================
//チュートリアルを写す処理
//=============================================================================
void COption::Tutorial(void)
{
	//マウス取得処理
	CInputMouse *pInputMouse;
	pInputMouse = CManager::GetInstance()->GetInputMouse();

	//マウスカーソルの位置取得
	POINT mouse_pos;
	GetCursorPos(&mouse_pos);
	HWND hWind = CManager::GetWindowHandle();
	ScreenToClient(hWind, &mouse_pos);
	D3DXVECTOR2 mousePos = D3DXVECTOR2((float)mouse_pos.x, (float)mouse_pos.y);

	//デフォルトUIの位置とサイズ取得
	D3DXVECTOR3 uiPos = m_pTutorialUi->GetPos();
	D3DXVECTOR3 uiSize = m_pTutorialUi->GetSize();

	//UIの中をにあったら
	if (mousePos.x >= uiPos.x - uiSize.x / 2.0f && mousePos.x <= uiPos.x + uiSize.x / 2.0f &&
		mousePos.y >= uiPos.y - uiSize.y / 2.0f && mousePos.y <= uiPos.y + uiSize.y / 2.0f)
	{
		//色取得
		D3DXCOLOR uiCol = m_pTutorialUi->GetCol();
		//薄くする
		uiCol.a = OPTION_SELECT_COLOR_ALPHA;
		//色設定
		m_pTutorialUi->SetCol(uiCol);

		//クリックしたら
		if (pInputMouse->GetTrigger(CInputMouse::MOUSE_TYPE::MOUSE_TYPE_LEFT) == true)
		{
			//チュートリアル画像が生成されてないなら
			if (m_pTutorial == nullptr)
			{
				//チュートリアル画像を生成する
				m_pTutorial = CObject2D::Create({ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f }, { SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f },
					                             (int)CObject::PRIORITY::OPTION);
				m_pTutorial->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("tutorial.jpg"));
			}

			//チュートリアルキャンセルUIが生成されてないなら
			if (m_pCloseTutorialUi == nullptr)
			{
				//チュートリアルキャンセルUIを生成する
				m_pCloseTutorialUi = CObject2D::Create(OPTION_CLOSE_TUTORIAL_UI_POS, OPTION_CLOSE_TUTORIAL_UI_SIZE,
					                                   (int)CObject::PRIORITY::OPTION);
				m_pCloseTutorialUi->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("cancel_ui.png"));
			}
		}
	}
	else
	{
		//色取得
		D3DXCOLOR uiCol = m_pTutorialUi->GetCol();
		//色を元に戻す
		uiCol.a = 1.0f;
		//色設定
		m_pTutorialUi->SetCol(uiCol);
	}
}

//=============================================================================
//チュートリアルを消すUIの処理
//=============================================================================
void COption::TutorialCancel(void)
{
	//マウス取得処理
	CInputMouse *pInputMouse;
	pInputMouse = CManager::GetInstance()->GetInputMouse();

	//マウスカーソルの位置取得
	POINT mouse_pos;
	GetCursorPos(&mouse_pos);
	HWND hWind = CManager::GetWindowHandle();
	ScreenToClient(hWind, &mouse_pos);
	D3DXVECTOR2 mousePos = D3DXVECTOR2((float)mouse_pos.x, (float)mouse_pos.y);

	//デフォルトUIの位置とサイズ取得
	D3DXVECTOR3 uiPos = m_pCloseTutorialUi->GetPos();
	D3DXVECTOR3 uiSize = m_pCloseTutorialUi->GetSize();

	//UIの中をにあったら
	if (mousePos.x >= uiPos.x - uiSize.x / 2.0f && mousePos.x <= uiPos.x + uiSize.x / 2.0f &&
		mousePos.y >= uiPos.y - uiSize.y / 2.0f && mousePos.y <= uiPos.y + uiSize.y / 2.0f)
	{
		//色取得
		D3DXCOLOR uiCol = m_pCloseTutorialUi->GetCol();
		//薄くする
		uiCol.a = OPTION_SELECT_COLOR_ALPHA;
		//色設定
		m_pCloseTutorialUi->SetCol(uiCol);

		//クリックしたら
		if (pInputMouse->GetTrigger(CInputMouse::MOUSE_TYPE::MOUSE_TYPE_LEFT) == true)
		{
			//チュートリアルの削除
			if (m_pTutorial != nullptr)
			{
				m_pTutorial->Uninit();
				m_pTutorial = nullptr;
			}

			//チュートリアルを閉じるUIの削除
			if (m_pCloseTutorialUi != nullptr)
			{
				m_pCloseTutorialUi->Uninit();
				m_pCloseTutorialUi = nullptr;
			}
		}
	}
	else
	{
		//色取得
		D3DXCOLOR uiCol = m_pCloseTutorialUi->GetCol();
		//色を元に戻す
		uiCol.a = 1.0f;
		//色設定
		m_pCloseTutorialUi->SetCol(uiCol);
	}
}
