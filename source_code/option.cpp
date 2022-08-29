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

//=============================================================================
// マクロ定義
//=============================================================================
#define OPTION_MOUSE_SENSI_MIN		(30.0f)								//マウスの感度をデフォルトから何パーセントまで下げれるようにするか
#define OPTION_MOUSE_SENSI_MAX		(170.0f)							//マウスの感度をデフォルトから何パーセントまで上げれるようにするか
#define OPTION_MOUSE_SENSI_DEFAULT	(100.0f)							//マウスの感度の最初の設定をデフォルトの何パーセントにするか
#define OPTION_BAR_SIZE				(D3DXVECTOR3(600.0f, 5.0f, 0.0f))	//バーのサイズ
#define OPTION_CIRCLE_SIZE			(D3DXVECTOR3(30.0f, 30.0f, 0.0f ))	//円のサイズ

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
	m_aNum[0] = OPTION_MOUSE_SENSI_DEFAULT;
	m_aNum[1] = 100.0f;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void COption::Uninit(void)
{
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
		//マウス感度設定処理
		MouseSensi();

		//視野角設定処理
		ViewAngle();
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
			                               (int)CObject::PRIORITY::UI);
		m_pOptionFrame->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("game_option.png"));

		//バーの生成
		m_apOptionBar[0] = COptionBar::Create({ 710.0f, 285.0f, 0.0f }, OPTION_BAR_SIZE, OPTION_CIRCLE_SIZE,
			                                  OPTION_MOUSE_SENSI_MIN, OPTION_MOUSE_SENSI_MAX, m_aNum[0]);
		//値の保存
		m_aNum[0] = m_apOptionBar[0]->GetNum();

		m_apOptionBar[1] = COptionBar::Create({ 710.0f, 355.0f, 0.0f }, OPTION_BAR_SIZE, OPTION_CIRCLE_SIZE,
			                                  50.0f, 150.0f, m_aNum[1]);
		//値の保存
		m_aNum[1] = m_apOptionBar[1]->GetNum();
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
				D3DXVECTOR2 cameraSpeed = { PLAYER_CAMERA_V__MOUSE_SPEED_XZ, PLAYER_CAMERA_V__MOUSE_SPEED_Y };

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
//視野角設定処理
//=============================================================================
void COption::ViewAngle(void)
{

}
