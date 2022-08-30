//================================================
//���U���g����
//Author: ��������
//================================================
#include "result.h"
#include "manager.h"
#include "fade.h"
#include "object2D.h"
#include "communicationdata.h"
#include "manager.h"
#include "networkmanager.h"
#include "counter.h"
#include "play_data.h"
#include "letter.h"
#include "input_keyboard.h"

//================================================
//�X�V�����}�N��
//===============================================
#define DISPLAY_FADE (500)

//================================================
//�f�t�H���g�R���X�g���N�^
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

			// SJIS �� wstring
			int iBufferSize = MultiByteToWideChar(CP_ACP,
				0,
				name.c_str(),
				-1,
				(wchar_t*)NULL,
				0);

			// �o�b�t�@�̎擾
			wchar_t* cpUCS2 = new wchar_t[iBufferSize];

			// SJIS �� wstring
			MultiByteToWideChar(CP_ACP,
				0,
				name.c_str(),
				-1,
				cpUCS2,
				iBufferSize);

			// string�̐���
			wstring utextbuf(cpUCS2, cpUCS2 + iBufferSize - 1);

			// �o�b�t�@�̔j��
			delete[] cpUCS2;

			m_key_name.push_back(utextbuf);
		}
	}

	fclose(file);
}

//================================================
//�f�X�g���N�^
//================================================
CResult::~CResult()
{

}

//================================================
//����������
//================================================
HRESULT CResult::Init(void)
{
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
			m_name_font.push_back(CLetter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - ((15.0f + 10.0f) * buf_size) / 2) + (30.0f * count_name), 0.0f + (100.0f * 2.0f), 0.0f), D3DXVECTOR3(15.0f, 15.0f, 0.0f), 300, 500, conbrt_buf[0][count_name]));
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
				m_name_font.push_back(CLetter::Create(D3DXVECTOR3(((SCREEN_WIDTH / 2) - ((15.0f + 10.0f) * buf_size) / 2) + (30.0f * count_name), 0.0f + (100.0f * 2.0f), 0.0f), D3DXVECTOR3(15.0f, 15.0f, 0.0f), 300, 500, conbrt_buf[0][count_name]));
			}
		}
	}

	m_Ui.push_back(CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, (SCREEN_WIDTH / 2) - 150.0f, 0.0f), D3DXVECTOR3(549.0f * 0.8f, 486.0f * 0.8f, 0.0f), (int)CObject::PRIORITY::UI));
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
		string buf = pData[count_player]->Player.aName[0];
		vector<wstring> conbrt_buf;
		int buf_size = buf.size();
		switch (pData[count_player]->Player.nNumber)
		{
		case 1:
			difference = 5.0f;
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
	}

	return S_OK;
}

//================================================
//�I������
//================================================
void CResult::Uninit(void)
{	
	//�I�u�W�F�N�g�̔j��
	Release();

	//�ʐM�؂�
	CTcpClient *pClient = CManager::GetInstance()->GetNetWorkmanager()->GetCommunication();
	pClient->Uninit();

	//�R�~���f�[�^������
	CManager::GetInstance()->GetNetWorkmanager()->Reset();
}

//================================================
//�X�V����
//================================================
void CResult::Update(void)
{
	m_nCount++;
	if (m_nCount >= DISPLAY_FADE)
	{
		//�t�F�[�h�擾����
		CFade *pFade;
		pFade = CManager::GetInstance()->GetFade();

		if (pFade->GetFade() == CFade::FADE_NONE)
		{
			pFade->SetFade(CManager::MODE::TITLE);
		}
	}
}

//================================================
//�`�揈��
//================================================
void CResult::Draw(void)
{
	
}
