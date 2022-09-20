//================================================
//リザルト処理
//Author: 加藤瑞葵
//================================================
#include "result.h"
#include "manager.h"
#include "fade.h"
#include "object2D.h"
#include "communicationdata.h"
#include "networkmanager.h"
#include "counter.h"
#include "play_data.h"
#include "letter.h"
#include "input_keyboard.h"
#include "input_mouse.h"
#include "sound.h"
#include "kill_rate_ui.h"

//================================================
//更新処理マクロ
//===============================================
#define DISPLAY_FADE (600)
#define RESULT_SCORE_UI_FRAME_SIZE		 (D3DXVECTOR3(570.0f, 498.0f, 0.0f))		//スコアUIを載せるフレームのUIのサイズ
#define RESULT_SCORE_UI_FRAME_POS		 (250)										//スコアUIを載せるフレームのUIの位置調整値
#define RESULT_KILL_DEATH_UI_FRAME_SIZE	 (D3DXVECTOR3(131.0f, 47.0f, 0.0f))			//K/D UIのサイズ
#define RESULT_KILL_DEATH_UI_FRAME_POS	 (D3DXVECTOR3(820.0f, 180.0f, 0.0f))		//K/D UIの位置

//================================================
//デフォルトコンストラクタ
//================================================
CResult::CResult(CObject::PRIORITY Priority) :CObject(Priority)
{
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

//================================================
//デストラクタ
//================================================
CResult::~CResult()
{

}

//================================================
//初期化処理
//================================================
HRESULT CResult::Init(void)
{
	//音を鳴らす
	CManager::GetInstance()->GetSound()->Play(CSound::SOUND_LABEL::RESULT_BGM);

	//背景を生成
	CObject2D *pBg = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f),
									   D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),
									   (int)CObject::PRIORITY::UI);
	pBg->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("result.png"));

	//フレームUIを生成
	CObject2D *pScoreFrame = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f),
											   RESULT_SCORE_UI_FRAME_SIZE,
											   (int)CObject::PRIORITY::UI);
	pScoreFrame->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("result_score_frame.png"));

	//K/D UIを生成
	CObject2D *pKillDeatfUi = CObject2D::Create(RESULT_KILL_DEATH_UI_FRAME_POS,
											    RESULT_KILL_DEATH_UI_FRAME_SIZE,
											    (int)CObject::PRIORITY::UI);
	pKillDeatfUi->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("kill_death.png"));

	//キルレUIをプレイヤーの数分生成 
	vector<CKillRateUi*> pKillRateUi;
	pKillRateUi.clear();

	//敵のデータ取得
	vector<CCommunicationData> data = CManager::GetInstance()->GetNetWorkmanager()->GetEnemyData();
	int nDataSize = data.size();

	//プレイヤーの数分分回す
	for (int nCntPlayer = 0; nCntPlayer < nDataSize + 1; nCntPlayer++)
	{
		//キルレUIの生成 
		pKillRateUi.push_back(CKillRateUi::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f - KILL_RATE_UI_FRAME_DEFAULT_SIZE_X / 2.0f,
	                                                          RESULT_SCORE_UI_FRAME_POS + (KILL_RATE_UI_FRAME_DEFAULT_SIZE_Y * nCntPlayer),
	                                                          0.0f),
															  { 1.0f, 1.0f, 1.0f }));
		//表示するランキングの設定
		//上から順にキル数の多い順番にする
		pKillRateUi[nCntPlayer]->SetRank(nCntPlayer + 1);
	}

	CCommunicationData::COMMUNICATION_DATA *pPlayerData = CManager::GetInstance()->GetNetWorkmanager()->GetPlayerData()->GetCmmuData();
	vector<CCommunicationData> EnemyData = CManager::GetInstance()->GetNetWorkmanager()->GetEnemyData();
	int max_enemy = EnemyData.size();

	if (pPlayerData->Player.bWin == true)
	{
		string buf = CManager::GetInstance()->GetPlayData()->GetName();
		buf = buf + "WIN!";
		vector<wstring> conbrt_buf;
		int buf_size = buf.size();
		conbrt_buf = CLetter::Conbrt(buf);

		buf_size = conbrt_buf[0].size();
		for (int count_name = 0; count_name < buf_size; count_name++)
		{
			m_name_font.push_back(CLetter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - ((15.0f + 10.0f) * buf_size) / 2) + (30.0f * count_name), 0.0f + (80.0f * 1.0f), 0.0f), D3DXVECTOR3(15.0f, 15.0f, 0.0f), 300, 500, conbrt_buf[0][count_name]));
		}
	}

	for (int count_enemy = 0; count_enemy < max_enemy; count_enemy++)
	{
		CCommunicationData::COMMUNICATION_DATA *pEnemyData = EnemyData[count_enemy].GetCmmuData();
		if (pEnemyData->Player.bWin == true)
		{
			string buf = pEnemyData->Player.aName[0];
			buf = buf + "WIN!";
			vector<wstring> conbrt_buf;
			int buf_size = buf.size();
			conbrt_buf = CLetter::Conbrt(buf);

			buf_size = conbrt_buf[0].size();
			for (int count_name = 0; count_name < buf_size; count_name++)
			{
				m_name_font.push_back(CLetter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - ((15.0f + 10.0f) * buf_size) / 2) + (30.0f * count_name), 0.0f + (80.0f * 1.0f), 0.0f), D3DXVECTOR3(15.0f, 15.0f, 0.0f), 300, 500, conbrt_buf[0][count_name]));
			}
		}
	}

	/*m_Ui.push_back(CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, (SCREEN_WIDTH / 2) - 150.0f, 0.0f), D3DXVECTOR3(549.0f * 0.8f, 486.0f * 0.8f, 0.0f), (int)CObject::PRIORITY::UI));
	m_Ui[0]->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("p_scoreboard.png"));

	CCommunicationData::COMMUNICATION_DATA *pData[MAX_PLAYER + 1];
	float difference = 0.0f;
	float difference_name = 0.0f;
	pData[0] = CManager::GetInstance()->GetNetWorkmanager()->GetPlayerData()->GetCmmuData();
	for (int count_enemy = 0; count_enemy < max_enemy; count_enemy++)
	{
		pData[count_enemy + 1] = EnemyData[count_enemy].GetCmmuData();
	}

	for (int count_player = 0, countor = 0; count_player < MAX_PLAYER + 1; count_player++, countor += 2)
	{
		difference_name += 75.0f;
		string buf;
		vector<wstring> conbrt_buf;
		int buf_size;
		switch (pData[count_player]->Player.nNumber)
		{
		case 1:
			difference = 5.0f;
			buf = pData[count_player]->Player.aName[0];
			m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (200.0f), 0.0f + (83.5f * difference), 0.0f), D3DXVECTOR3(25.0f, 32.5f, 0.0f), 2, "kill_num.png"));
			m_Countor[countor]->SetCounterNum(pData[count_player]->Player.nKill);
			m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (200.0f + 65.0f), 0.0f + (83.5f * difference), 0.0f), D3DXVECTOR3(25.0f, 32.5f, 0.0f), 2, "death_num.png"));
			m_Countor[countor + 1]->SetCounterNum(pData[count_player]->Player.nDeath);
			conbrt_buf = CLetter::Conbrt(buf);

			buf_size = conbrt_buf[0].size();
			for (int count_name = 0; count_name < buf_size; count_name++)
			{
				m_name_font.push_back(CLetter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - (((15.0f + 15.0f) * buf_size) / 2) * 1.7f) + (30.0f * count_name), ((SCREEN_HEIGHT / 2) - (15.0f * 2.0f)) + (difference_name + count_player), 0.0f), D3DXVECTOR3(15.0f, 15.0f, 0.0f), 300, 500, conbrt_buf[0][count_name]));
			}
			conbrt_buf.clear();
			break;
		case 2:
			difference = 6.0f;
			buf = pData[count_player]->Player.aName[0];
			m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (200.0f), 0.0f + (83.5f * difference), 0.0f), D3DXVECTOR3(25.0f, 32.5f, 0.0f), 2, "kill_num.png"));
			m_Countor[countor]->SetCounterNum(pData[count_player]->Player.nKill);
			m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (200.0f + 65.0f), 0.0f + (83.5f * difference), 0.0f), D3DXVECTOR3(25.0f, 32.5f, 0.0f), 2, "death_num.png"));
			m_Countor[countor + 1]->SetCounterNum(pData[count_player]->Player.nDeath);
			conbrt_buf = CLetter::Conbrt(buf);

			buf_size = conbrt_buf[0].size();
			for (int count_name = 0; count_name < buf_size; count_name++)
			{
				m_name_font.push_back(CLetter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - (((15.0f + 15.0f) * buf_size) / 2) * 1.7f) + (30.0f * count_name), ((SCREEN_HEIGHT / 2) - (15.0f * 2.0f)) + (difference_name + count_player), 0.0f), D3DXVECTOR3(15.0f, 15.0f, 0.0f), 300, 500, conbrt_buf[0][count_name]));
			}
			conbrt_buf.clear();
			break;
		case 3:
			difference = 7.0f;
			buf = pData[count_player]->Player.aName[0];
			m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (200.0f), 0.0f + (83.5f * difference), 0.0f), D3DXVECTOR3(25.0f, 32.5f, 0.0f), 2, "kill_num.png"));
			m_Countor[countor]->SetCounterNum(pData[count_player]->Player.nKill);
			m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (200.0f + 65.0f), 0.0f + (83.5f * difference), 0.0f), D3DXVECTOR3(25.0f, 32.5f, 0.0f), 2, "death_num.png"));
			m_Countor[countor + 1]->SetCounterNum(pData[count_player]->Player.nDeath);
			conbrt_buf = CLetter::Conbrt(buf);

			buf_size = conbrt_buf[0].size();
			for (int count_name = 0; count_name < buf_size; count_name++)
			{
				m_name_font.push_back(CLetter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - (((15.0f + 15.0f) * buf_size) / 2) * 1.7f) + (30.0f * count_name), ((SCREEN_HEIGHT / 2) - (15.0f * 2.0f)) + (difference_name + count_player), 0.0f), D3DXVECTOR3(15.0f, 15.0f, 0.0f), 300, 500, conbrt_buf[0][count_name]));
			}
			conbrt_buf.clear();
			break;
		case 4:
			difference = 8.0f;
			buf = pData[count_player]->Player.aName[0];
			m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (200.0f), 0.0f + (83.5f * difference), 0.0f), D3DXVECTOR3(25.0f, 32.5f, 0.0f), 2, "kill_num.png"));
			m_Countor[countor]->SetCounterNum(pData[count_player]->Player.nKill);
			m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (200.0f + 65.0f), 0.0f + (83.5f * difference), 0.0f), D3DXVECTOR3(25.0f, 32.5f, 0.0f), 2, "death_num.png"));
			m_Countor[countor + 1]->SetCounterNum(pData[count_player]->Player.nDeath);
			conbrt_buf = CLetter::Conbrt(buf);

			buf_size = conbrt_buf[0].size();
			for (int count_name = 0; count_name < buf_size; count_name++)
			{
				m_name_font.push_back(CLetter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - (((15.0f + 15.0f) * buf_size) / 2) * 1.7f) + (30.0f * count_name), ((SCREEN_HEIGHT / 2) - (15.0f * 2.0f)) + (difference_name + count_player), 0.0f), D3DXVECTOR3(15.0f, 15.0f, 0.0f), 300, 500, conbrt_buf[0][count_name]));
			}
			conbrt_buf.clear();
			break;
		default:
			buf = pData[count_player]->Player.aName[0];
			m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (200.0f), 0.0f + (83.5f * difference), 0.0f), D3DXVECTOR3(25.0f, 32.5f, 0.0f), 2, "kill_num.png"));
			m_Countor[countor]->SetCounterNum(pData[count_player]->Player.nKill);
			m_Countor.push_back(CCounter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - 100.0f) + (200.0f + 65.0f), 0.0f + (83.5f * difference), 0.0f), D3DXVECTOR3(25.0f, 32.5f, 0.0f), 2, "death_num.png"));
			m_Countor[countor + 1]->SetCounterNum(pData[count_player]->Player.nDeath);
			conbrt_buf = CLetter::Conbrt(buf);

			buf_size = conbrt_buf[0].size();
			for (int count_name = 0; count_name < buf_size; count_name++)
			{
				m_name_font.push_back(CLetter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - (((15.0f + 15.0f) * buf_size) / 2) * 1.7f) + (30.0f * count_name), ((SCREEN_HEIGHT / 2) - (15.0f * 2.0f)) + (difference_name + count_player), 0.0f), D3DXVECTOR3(15.0f, 15.0f, 0.0f), 300, 500, conbrt_buf[0][count_name]));
			}
			conbrt_buf.clear();
			break;
		}
	}*/

	return S_OK;
}

//================================================
//終了処理
//================================================
void CResult::Uninit(void)
{	
	//音を止める
	CManager::GetInstance()->GetSound()->Stop(CSound::SOUND_LABEL::RESULT_BGM);

	//オブジェクトの破棄
	Release();

	//通信切る
	CTcpClient *pClient = CManager::GetInstance()->GetNetWorkmanager()->GetCommunication();
	pClient->Uninit();

	//コミュデータ初期化
	CManager::GetInstance()->GetNetWorkmanager()->Reset();
}

//================================================
//更新処理
//================================================
void CResult::Update(void)
{
	//キーボード取得処理
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//マウス取得処理
	CInputMouse *pInputMouse;
	pInputMouse = CManager::GetInstance()->GetInputMouse();

	m_nCount++;
	if (m_nCount >= DISPLAY_FADE || pInputKeyboard->GetTrigger(DIK_RETURN) || pInputMouse->GetPress(CInputMouse::MOUSE_TYPE::MOUSE_TYPE_LEFT))
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
void CResult::Draw(void)
{
	
}
