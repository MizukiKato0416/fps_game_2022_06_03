//================================================
//リザルト処理
//Author: 加藤瑞葵
//================================================
#include "tutorial.h"
#include "manager.h"
#include "fade.h"
#include "object2D.h"
#include "sound.h"
#include "input_keyboard.h"
#include "input_mouse.h"

//================================================
//更新処理マクロ
//===============================================

//================================================
//デフォルトコンストラクタ
//================================================
CTutorial::CTutorial(CObject::PRIORITY Priority) :CObject(Priority)
{
	
}

//================================================
//デストラクタ
//================================================
CTutorial::~CTutorial()
{

}

//================================================
//初期化処理
//================================================
HRESULT CTutorial::Init(void)
{
	//音を鳴らす
	CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL::TITLE_BGM);

	//チュートリアル画像の生成
	CObject2D *pTutorialUi = CObject2D::Create({ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f }, { SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f },
		                                       (int)CObject::PRIORITY::UI);
	pTutorialUi->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("tutorial.jpg"));

	return S_OK;
}

//================================================
//終了処理
//================================================
void CTutorial::Uninit(void)
{	
	//音を止める
	CManager::GetInstance()->GetSound()->Stop(CSound::SOUND_LABEL::TITLE_BGM);

	//オブジェクトの破棄
	Release();
}

//================================================
//更新処理
//================================================
void CTutorial::Update(void)
{
	//キーボード取得処理
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//マウス取得処理
	CInputMouse *pInputMouse;
	pInputMouse = CManager::GetInstance()->GetInputMouse();

	//エンターキーを押したらまたは左クリックしたら
	if (pInputKeyboard->GetTrigger(DIK_RETURN) || pInputMouse->GetPress(CInputMouse::MOUSE_TYPE::MOUSE_TYPE_LEFT))
	{
		//フェード取得処理
		CFade *pFade;
		pFade = CManager::GetInstance()->GetFade();

		if (pFade->GetFade() == CFade::FADE_NONE)
		{
			//タイトルに遷移する
			pFade->SetFade(CManager::MODE::TITLE);

			//音を鳴らす
			CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL::DECIDE_SE);
		}
	}
}

//================================================
//描画処理
//================================================
void CTutorial::Draw(void)
{
	
}
