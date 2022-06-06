//================================================
//ゲーム01処理
//Author: 加藤瑞葵
//================================================
#include "game01.h"
#include "manager.h"
#include "input_keyboard.h"
#include "player.h"
#include "fade.h"
#include "floor.h"
#include "mesh_field.h"
#include "xanimmodel.h"

//================================================
//マクロ定義
//================================================

//================================================
//静的メンバ変数宣言
//================================================

//================================================
//デフォルトコンストラクタ
//================================================
CGame01::CGame01(CObject::PRIORITY Priority):CObject(Priority)
{
	m_AnimSpeed = 1.0f;
	m_pPlayer = nullptr;
}

//================================================
//オーバーロードされたコンストラクタ
//================================================


//================================================
//デストラクタ
//================================================
CGame01::~CGame01()
{
	
}

//================================================
//初期化処理
//================================================
HRESULT CGame01::Init(void)
{
	pXanim = CXanimModel::Create("data/test.x");
	pXanim->ChangeAnimation(0, (120.0f * m_AnimSpeed) / 4800);

	//プレイヤーの生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI / 2.0f, 0.0f));

	//メッシュフィールド生成
	CMeshField *pMeshField = CMeshField::CreateLoadText("data/mesh_field.txt");
	pMeshField->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("grass001.png"));

	return S_OK;
}

//================================================
//終了処理
//================================================
void CGame01::Uninit(void)
{
	//オブジェクトの破棄
	Release();
	pXanim->Uninit();
}

//================================================
//更新処理
//================================================
void CGame01::Update(void)
{
#ifdef _DEBUG
	//キーボード取得処理
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//Enterキーを押したら
	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		//フェード取得処理
		CFade *pFade;
		pFade = CManager::GetInstance()->GetFade();

		if (pFade->GetFade() == CFade::FADE_NONE)
		{
			pFade->SetFade(CManager::MODE::RESULT);
		}
	}
#endif // !_DEBUG
	if (pInputKeyboard->GetPress(DIK_LEFT) == true)
	{
		m_AnimSpeed -= 0.01f;
	}
	else if (pInputKeyboard->GetPress(DIK_RIGHT) == true)
	{
		m_AnimSpeed += 0.01f;
	}

	pXanim->Update();
	if (pInputKeyboard->GetPress(DIK_1) == true)
	{
		pXanim->ChangeAnimation(0, (70.0f * m_AnimSpeed) / 4800);
	}
	else if (pInputKeyboard->GetPress(DIK_2) == true)
	{
		pXanim->ChangeAnimation(1, (120.0f * m_AnimSpeed) / 4800);
	}

}

//================================================
//描画処理
//================================================
void CGame01::Draw(void)
{
	pXanim->Draw();
}