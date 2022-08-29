//=============================================================================
//
// タイトル処理
// Author : 加藤瑞葵
//
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include "title.h"
#include "manager.h"
#include "fade.h"
#include "ui.h"
#include "object2D.h"
#include "input_mouse.h"
#include "fileload.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TITLE_BULLET_HOLE_UI_SIZE		(100.0f)		//弾痕UIのサイズ


//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CTitle::CTitle(CObject::PRIORITY Priority):CObject(Priority)
{
	
}

//=============================================================================
// デフォルトデストラクタ
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTitle::Init(void)
{
	vector<string> txt_data;	// テキストファイルの保存バッファ
	vector<CFileLoad::TITLE_ALLOCATION_DATA> title;	// ステージ情報

	txt_data = CFileLoad::LoadTxt("data/titledata.txt");
	title = CFileLoad::CreateTitleAllocation(txt_data);
	int stage_element = title.size();
	for (int count_stage = 0; count_stage < stage_element; count_stage++)
	{
		m_ui.push_back(CUi::Create(title[count_stage].pos, title[count_stage].size, title[count_stage].stop, title[count_stage].blinking_type, title[count_stage].move_type, title[count_stage].blinking_speed, title[count_stage].move_speed, title[count_stage].pas));
	}

	//右下のプレイヤーの生成
	CObject2D *pPlayer = CObject2D::Create({ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f }, { SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f },
		                                  (int)CObject::PRIORITY::UI);
	pPlayer->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("title_player.png"));

	m_pointor = CObject2D::Create({ 0.0f, 0.0f, 0.0f }, { 83.0f * 0.9f, 74.0f * 0.9f, 0.0f }, (int)CObject::PRIORITY::UI);
	m_pointor->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("pointer.png"));

	//ShowCursor(FALSE);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	//オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTitle::Update(void)
{
	POINT mouse_pos;
	GetCursorPos(&mouse_pos);
	ScreenToClient(CManager::GetWindowHandle(), &mouse_pos);
	CInputMouse *mouse = CManager::GetInstance()->GetInputMouse();
	D3DXVECTOR3 pointor_size = m_pointor->GetSize();
	D3DXVECTOR3 pointor_pos = { (float)mouse_pos.x, (float)mouse_pos.y, 0.0f };
	int ui_size = m_ui.size();

	m_pointor->SetPos(pointor_pos, pointor_size);

	for (int count_ui = 0; count_ui < ui_size; count_ui++)
	{
		if (m_ui[count_ui]->GetTexName() == "signboard.png")
		{
			D3DXCOLOR col = m_ui[count_ui]->GetCol();
			if ((pointor_pos.x + pointor_size.x) >= (m_ui[count_ui]->GetPos().x - (m_ui[count_ui]->GetSize().x / 2)) &&
				(pointor_pos.x - pointor_size.x) <= (m_ui[count_ui]->GetPos().x + (m_ui[count_ui]->GetSize().x / 2)) &&
				(pointor_pos.y + pointor_size.y) >= (m_ui[count_ui]->GetPos().y - (m_ui[count_ui]->GetSize().y / 2)) &&
				(pointor_pos.y - pointor_size.y) <= (m_ui[count_ui]->GetPos().y + (m_ui[count_ui]->GetSize().y / 2)))
			{
				col.a = 0.5f;
				if (mouse->GetPress(mouse->MOUSE_TYPE_LEFT) == true)
				{
					CFade *fade = CManager::GetInstance()->GetFade();

					if (fade->GetFade() == CFade::FADE_NONE)
					{
						fade->SetFade(CManager::MODE::GAME01);

						//弾痕のUIを出す
						CObject2D *pObject2D = CObject2D::Create({ (float)mouse_pos.x, (float)mouse_pos.y, 0.0f },
						                                         { TITLE_BULLET_HOLE_UI_SIZE, TITLE_BULLET_HOLE_UI_SIZE, 0.0f },
							                                     (int)CObject::PRIORITY::UI);
						pObject2D->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("bullet_hole_ui.png"));
					}
				}
			}
			else
			{
				col.a = 1.0f;
			}
			m_ui[count_ui]->SetCol(col);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTitle::Draw(void)
{
	
}