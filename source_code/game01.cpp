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
#include "meshfield.h"

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
	//プレイヤーの生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI / 2.0f, 0.0f));


	CMeshField *pMeshField = CMeshField::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1000.0f, 0.0f, 1000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		                                        100, 100);
	pMeshField->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("road000.jpg"));

	

	D3DXVECTOR3 pos = pMeshField->GetVtxPos(0);
	pos.y += 400;
	pMeshField->SetVtxPos(0, pos);

	pos = pMeshField->GetVtxPos(2);
	pos.y += 100;
	pMeshField->SetVtxPos(2, pos);

	pos = pMeshField->GetVtxPos(6);
	pos.y += -300;
	pMeshField->SetVtxPos(6, pos);

	pos = pMeshField->GetVtxPos(8);
	pos.y += 400;
	pMeshField->SetVtxPos(8, pos);

	return S_OK;
}

//================================================
//終了処理
//================================================
void CGame01::Uninit(void)
{
	//オブジェクトの破棄
	Release();
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
}

//================================================
//描画処理
//================================================
void CGame01::Draw(void)
{

}