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
#include "play_data.h"
#include "letter.h"
#include "communicationdata.h"
#include "sound.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TITLE_BULLET_HOLE_UI_SIZE		(100.0f)										//弾痕UIのサイズ
#define NONAME_SIZE (6)
#define TITLE_BGM_PLAY_COUNT			(180)											//タイトルBGMを鳴らし始めるまでのカウント
#define TITLE_LOGO_INIT_POS				(D3DXVECTOR3(300.0f, -200.0f, 0.0f))			//タイトルロゴの初期位置
#define TITLE_LOGO_SIZE					(D3DXVECTOR3(550.0f, 300.0f, 0.0f))				//タイトルロゴのサイズ
#define TITLE_LOGO_LAST_POS_Y			(160.0f)										//タイトルロゴの最終位置Y
#define TITLE_LOGO_MOVE					(2.0f)											//タイトルロゴの移動量

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CTitle::CTitle(CObject::PRIORITY Priority):CObject(Priority)
{
	m_nCounter = 0;
	m_pTitleLogo = nullptr;

	FILE *file;

	file = fopen("data/keyconfig.txt", "r");

	if (file != NULL)
	{
		for (int nCntKey = 1; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			char name_buf[1][64];
			string name;

			fscanf(file, "%s", name_buf[0]);
			name = name_buf[0];
			m_letter_single.push_back(name);

			// SJIS → wstring
			int iBufferSize = MultiByteToWideChar(CP_ACP,
				0,
				name.c_str(),
				-1,
				(wchar_t*)NULL,
				0);

			// バッファの取得
			wchar_t* cpUCS2 = new wchar_t[iBufferSize];

			// SJIS → wstring
			MultiByteToWideChar(CP_ACP,
				0,
				name.c_str(),
				-1,
				cpUCS2,
				iBufferSize);

			// stringの生成
			wstring utextbuf(cpUCS2, cpUCS2 + iBufferSize - 1);

			// バッファの破棄
			delete[] cpUCS2;

			m_key_name.push_back(utextbuf);
		}
	}

	fclose(file);
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
	//変数初期化
	m_nCounter = 0;

	//音を鳴らす
	CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL::TITLE_WIND_SE);

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
	m_name_box = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH - 300.0f, 0.0f + 95.0f, 0.0f), D3DXVECTOR3(480.0f, 50.0f, 0.0f), static_cast<int>(CObject::PRIORITY::UI));
	m_name_box->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("password_wordbox.png"));

	for (int count_name = 0; count_name < NONAME_SIZE; count_name++)
	{
		switch (count_name)
		{
		case 0:
			m_name_font.push_back(CLetter::Create(D3DXVECTOR3((((SCREEN_WIDTH - 300.0f) - (480.0f / 2.0f)) + 32.5f) + (30.0f * count_name), (((0.0f + 95.f) - (50.0f / 2.0f)) + 25.0f), 0.0f), D3DXVECTOR3(15.0f, 15.0f, 0.0f), 300, 500, m_key_name[DIK_N][0]));
			m_name.push_back(m_letter_single[DIK_N][0]);
			break;
		case 1:
			m_name_font.push_back(CLetter::Create(D3DXVECTOR3((((SCREEN_WIDTH - 300.0f) - (480.0f / 2.0f)) + 32.5f) + (30.0f * count_name), (((0.0f + 95.f) - (50.0f / 2.0f)) + 25.0f), 0.0f), D3DXVECTOR3(15.0f, 15.0f, 0.0f), 300, 500, m_key_name[DIK_O][0]));
			m_name.push_back(m_letter_single[DIK_O][0]);
			break;
		case 2:
			m_name_font.push_back(CLetter::Create(D3DXVECTOR3((((SCREEN_WIDTH - 300.0f) - (480.0f / 2.0f)) + 32.5f) + (30.0f * count_name), (((0.0f + 95.f) - (50.0f / 2.0f)) + 25.0f), 0.0f), D3DXVECTOR3(15.0f, 15.0f, 0.0f), 300, 500, m_key_name[DIK_N][0]));
			m_name.push_back(m_letter_single[DIK_N][0]);
			break;
		case 3:
			m_name_font.push_back(CLetter::Create(D3DXVECTOR3((((SCREEN_WIDTH - 300.0f) - (480.0f / 2.0f)) + 32.5f) + (30.0f * count_name), (((0.0f + 95.f) - (50.0f / 2.0f)) + 25.0f), 0.0f), D3DXVECTOR3(15.0f, 15.0f, 0.0f), 300, 500, m_key_name[DIK_A][0]));
			m_name.push_back(m_letter_single[DIK_A][0]);
			break;
		case 4:
			m_name_font.push_back(CLetter::Create(D3DXVECTOR3((((SCREEN_WIDTH - 300.0f) - (480.0f / 2.0f)) + 32.5f) + (30.0f * count_name), (((0.0f + 95.f) - (50.0f / 2.0f)) + 25.0f), 0.0f), D3DXVECTOR3(15.0f, 15.0f, 0.0f), 300, 500, m_key_name[DIK_M][0]));
			m_name.push_back(m_letter_single[DIK_M][0]);
			break;
		case 5:
			m_name_font.push_back(CLetter::Create(D3DXVECTOR3((((SCREEN_WIDTH - 300.0f) - (480.0f / 2.0f)) + 32.5f) + (30.0f * count_name), (((0.0f + 95.f) - (50.0f / 2.0f)) + 25.0f), 0.0f), D3DXVECTOR3(15.0f, 15.0f, 0.0f), 300, 500, m_key_name[DIK_E][0]));
			m_name.push_back(m_letter_single[DIK_E][0]);
			break;
		default:
			break;
		}
	}
	CManager::GetInstance()->GetPlayData()->SetName(m_name);

	//タイトルロゴの生成
	m_pTitleLogo = CObject2D::Create(TITLE_LOGO_INIT_POS, TITLE_LOGO_SIZE, (int)CObject::PRIORITY::UI);
	m_pTitleLogo->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("title_logo.png"));

	ShowCursor(FALSE);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	//音を止める
	CManager::GetInstance()->GetSound()->Stop(CSound::SOUND_LABEL::TITLE_BGM);
	CManager::GetInstance()->GetSound()->Stop(CSound::SOUND_LABEL::TITLE_WIND_SE);

	//オブジェクトの破棄
	Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void CTitle::Update(void)
{
	//既定の値以下なら
	if(m_nCounter <= TITLE_BGM_PLAY_COUNT)
	{
		//カウンターを加算
		m_nCounter++;

		//既定の値になったら
		if (m_nCounter == TITLE_BGM_PLAY_COUNT)
		{
			//音を鳴らす
			//CManager::GetInstance()->GetSound()->UpdateVoice(CSound::SOUND_LABEL::TITLE_BGM);
			CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL::TITLE_BGM);
		}
	}

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
		if (m_ui[count_ui]->GetTexName() == "signboard.png" || m_ui[count_ui]->GetTexName() == "signboard_tutorial.png")
		{
			D3DXCOLOR col = m_ui[count_ui]->GetCol();
			if ((pointor_pos.x /*+ pointor_size.x*/) >= (m_ui[count_ui]->GetPos().x - (m_ui[count_ui]->GetSize().x / 2)) &&
				(pointor_pos.x /*- pointor_size.x*/) <= (m_ui[count_ui]->GetPos().x + (m_ui[count_ui]->GetSize().x / 2)) &&
				(pointor_pos.y /*+ pointor_size.y*/) >= (m_ui[count_ui]->GetPos().y - (m_ui[count_ui]->GetSize().y / 2)) &&
				(pointor_pos.y /*- pointor_size.y*/) <= (m_ui[count_ui]->GetPos().y + (m_ui[count_ui]->GetSize().y / 2)))
			{
				col.a = 0.5f;
				if (mouse->GetTrigger(mouse->MOUSE_TYPE_LEFT) == true)
				{
					CFade *fade = CManager::GetInstance()->GetFade();

					if (fade->GetFade() == CFade::FADE_NONE)
					{
						if (m_ui[count_ui]->GetTexName() == "signboard.png")
						{
							fade->SetFade(CManager::MODE::GAME01);
						}
						else if (m_ui[count_ui]->GetTexName() == "signboard_tutorial.png")
						{
							fade->SetFade(CManager::MODE::TUTORIAL);
						}
						//弾痕のUIを出す
						CObject2D *pObject2D = CObject2D::Create({ (float)mouse_pos.x, (float)mouse_pos.y, 0.0f },
						                                         { TITLE_BULLET_HOLE_UI_SIZE, TITLE_BULLET_HOLE_UI_SIZE, 0.0f },
							                                     (int)CObject::PRIORITY::UI);
						pObject2D->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("bullet_hole_ui.png"));

						//音を鳴らす
						CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL::START_SHOT_SE);
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
	PasWord();

	//ロゴの処理
	Logo();
}

//=============================================================================
// 描画処理
//=============================================================================
void CTitle::Draw(void)
{
	
}

//=============================================================================
// 名前入力
//=============================================================================
void CTitle::PasWord(void)
{
	CInputKeyboard *key;
	CInputMouse *mouse;
	POINT point;
	HWND hwnd;
	string text_buf;
	pair<int, bool> key_update;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 size;
	key = CManager::GetInstance()->GetInputKeyboard();
	hwnd = CManager::GetInstance()->GetWindowHandle();
	mouse = CManager::GetInstance()->GetInputMouse();
	GetCursorPos(&point);
	ScreenToClient(hwnd, &point);

	pos = m_name_box->GetPos();
	size = m_name_box->GetSize();

	if (pos.x - size.x / 2.0f <= point.x &&
		pos.x + size.x / 2.0f >= point.x &&
		pos.y - size.y / 2.0f <= point.y &&
		pos.y + size.y / 2.0f >= point.y)
	{
		if (mouse->GetTrigger(CInputMouse::MOUSE_TYPE_LEFT) == true)
		{
			m_name_drop = true;
			if (m_name == "NONAME")
			{
				CManager::GetInstance()->GetPlayData()->SetName("");
				int font_size = m_name_font.size();
				for (int count_font = 0; count_font < font_size; count_font++)
				{
					m_name_font[count_font]->Uninit();
					m_name_font.erase(m_name_font.begin());
					font_size = m_name_font.size();
					count_font--;
					m_count_letter = 0;
					m_letter_limitl = 0;
				}
				int pas_size = m_name.size();
				for (int count_pas = 0; count_pas < pas_size; count_pas++)
				{
					m_name.pop_back();
				}
			}
		}
	}
	else
	{
		if (mouse->GetTrigger(CInputMouse::MOUSE_TYPE_LEFT) == true)
		{
			CManager::GetInstance()->GetPlayData()->SetName(m_name);
			m_name_drop = false;
		}
	}

	if (m_name_drop == true)
	{
		m_name_box->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

		key_update = key->GetAllKeyUpdate();

		if (key_update.second == true)
		{
			if (key_update.first != DIK_RETURN && key_update.first != DIK_BACK)
			{
				if (m_letter_limitl < NAME_NAX)
				{
					int name_size = m_key_name[key_update.first].size();
					for (int count_name = 0; count_name < name_size; count_name++)
					{
						m_name_font.push_back(CLetter::Create(D3DXVECTOR3((((SCREEN_WIDTH - 300.0f) - (480.0f / 2.0f)) + 32.5f) + (30.0f * m_count_letter), (((0.0f + 95.f) - (50.0f / 2.0f)) + 25.0f), 0.0f), D3DXVECTOR3(15.0f, 15.0f, 0.0f), 300, 500, m_key_name[key_update.first][count_name]));
						m_name.push_back(m_letter_single[key_update.first][count_name]);
						m_count_letter++;
						m_letter_limitl++;
					}
				}
			}
			else if (key_update.first == DIK_BACK)
			{
				CManager::GetInstance()->GetPlayData()->SetName("");
				int font_size = m_name_font.size();
				for (int count_font = 0; count_font < font_size; count_font++)
				{
					m_name_font[count_font]->Uninit();
					m_name_font.erase(m_name_font.begin());
					font_size = m_name_font.size();
					count_font--;
					m_count_letter = 0;
					m_letter_limitl = 0;
				}
				int pas_size = m_name.size();
				for (int count_pas = 0; count_pas < pas_size; count_pas++)
				{
					m_name.pop_back();
				}
			}
			else if (key_update.first == DIK_RETURN)
			{
				CManager::GetInstance()->GetPlayData()->SetName(m_name);
				m_name_drop = false;
			}
		}
	}
	else
	{
		m_name_box->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

//=============================================================================
//ロゴの処理
//=============================================================================
void CTitle::Logo(void)
{
	//タイトルロゴが生成されていたら
	if (m_pTitleLogo != nullptr)
	{
		//位置を取得
		D3DXVECTOR3 pos = m_pTitleLogo->GetPos();

		//既定の値より小さいとき
		if (pos.y < TITLE_LOGO_LAST_POS_Y)
		{
			//既定の値分動かす
			pos.y += TITLE_LOGO_MOVE;

			//既定の値より大きくなったら
			if (pos.y > TITLE_LOGO_LAST_POS_Y)
			{
				//既定の値にする
				pos.y = TITLE_LOGO_LAST_POS_Y;
			}

			//位置設定
			m_pTitleLogo->SetPos(pos, m_pTitleLogo->GetSize());
		}
	}
}
