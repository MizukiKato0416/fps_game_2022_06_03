//================================================
//�Q�[��01����
//Author: ��������
//================================================
#define _CRT_SECURE_NO_WARNINGS

#include <thread>
#include "game01.h"
#include "manager.h"
#include "input_keyboard.h"
#include "player.h"
#include "fade.h"
#include "floor.h"
#include "mesh_field.h"
#include "meshsphere.h"
#include "model_single.h"
#include "tcp_client.h"
#include "enemy.h"
#include "object3D.h"
#include "networkmanager.h"
#include "score_ui.h"
#include "option.h"
#include "communicationdata.h"
#include "networkmanager.h"
#include "play_data.h"

//================================================
//�}�N����`
//================================================
#define GAME01_FIELD_SIZE				(50000.0f)									//�}�b�v�̍L��
#define GAME01_PLAYER_RESPAWN_POS_00	(D3DXVECTOR3(-300.0f, 150.0f, 2000.0f))		//���X�|�[���̏ꏊ
#define GAME01_PLAYER_RESPAWN_POS_01	(D3DXVECTOR3(2400.0f, 100.0f, 2800.0f))		//���X�|�[���̏ꏊ
#define GAME01_PLAYER_RESPAWN_POS_02	(D3DXVECTOR3(2700.0f, 170.0f, 600.0f))		//���X�|�[���̏ꏊ
#define GAME01_PLAYER_RESPAWN_POS_03	(D3DXVECTOR3(-2300.0f, 100.0f, -1500.0f))	//���X�|�[���̏ꏊ
#define GAME01_PLAYER_RESPAWN_POS_04	(D3DXVECTOR3(200.0f, 100.0f, -1000.0f))		//���X�|�[���̏ꏊ
#define GAME01_PLAYER_RESPAWN_POS_05	(D3DXVECTOR3(1900.0f, 200.0f, -1000.0f))	//���X�|�[���̏ꏊ
#define GAME01_PLAYER_RESPAWN_POS_06	(D3DXVECTOR3(2500.0f, 200.0f, -2500.0f))	//���X�|�[���̏ꏊ
#define GAME01_PLAYER_RESPAWN_POS_07	(D3DXVECTOR3(-1000.0f, 160.0f, 1700.0f))	//���X�|�[���̏ꏊ
#define GAME01_SCORE_UI_POS_00			(D3DXVECTOR3(0.0f, 666.0f, 0.0f))			//�X�R�AUI�̈ʒu
#define GAME01_SCORE_UI_POS_01			(D3DXVECTOR3(0.0f, 700.0f, 0.0f))			//�X�R�AUI�̈ʒu

//================================================
//�ÓI�����o�ϐ��錾
//================================================

//================================================
//�f�t�H���g�R���X�g���N�^
//================================================
CGame01::CGame01(CObject::PRIORITY Priority):CObject(Priority)
{
	m_pPlayer = nullptr;
	m_pMeshField = nullptr;
	m_bAllConnect = false;
	m_pEnemy.clear();
	m_respawnPos = PlayerRespawnPos::NONE;
	m_pMapBg = nullptr;
	m_pScorUiTop = nullptr;
	m_pScorUiUnder = nullptr;
	m_pOption = nullptr;
}

//================================================
//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
//================================================


//================================================
//�f�X�g���N�^
//================================================
CGame01::~CGame01()
{
	
}

//================================================
//����������
//================================================
HRESULT CGame01::Init(void)
{
	//�ϐ�������
	m_respawnPos = PlayerRespawnPos::NONE;
	m_pOption = nullptr;

	FirstContact();

	//���b�V���t�B�[���h����
	m_pMeshField = CMeshField::CreateLoadText("data/mesh_field.txt");
	m_pMeshField->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("floor_01.jpg"));

	//���b�V���X�t�B�A����
	CMeshSphere *pMeshSphere = CMeshSphere::Create(D3DXVECTOR3(0.0f, -100.0f, 0.0f), D3DXVECTOR3(GAME01_FIELD_SIZE, GAME01_FIELD_SIZE, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		                                           32, 8);
	pMeshSphere->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("sky.jpg"));

	//���̐���
	CObject3D *pObject3D = CObject3D::Create(D3DXVECTOR3(0.0f, -100.0f, 0.0f), D3DXVECTOR3(GAME01_FIELD_SIZE, 0.0f, GAME01_FIELD_SIZE), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pObject3D->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("mist.png"));

	//���f���̓ǂݍ��ݐݒu
	LoadModelTxt("data/model_set.txt");

	//�ݒ��ʂ𐶐�����
	m_pOption = COption::Create();

	return S_OK;
}

//================================================
//�I������
//================================================
void CGame01::Uninit(void)
{
	//�I�u�W�F�N�g�̔j��
	Release();
}

//================================================
//�X�V����
//================================================
void CGame01::Update(void)
{
	//POINT pos;

	//pos.x = SCREEN_WIDTH / 2;
	//pos.y = SCREEN_HEIGHT / 2;

	//SetCursorPos(pos.x, pos.y);

	if (CManager::GetInstance()->GetNetWorkmanager()->GetAllConnect() == true)
	{
		m_bAllConnect = true;
	}

	if (m_bAllConnect == false)
	{
		if (m_now_loding != nullptr)
		{
			m_count_pattern++;
			if (m_count_pattern >= 25)
			{
				m_now_loding->SetTex(m_pattern_tex, 4);
				m_pattern_tex++;
				if (m_pattern_tex >= 4)
				{
					m_pattern_tex = 0;
				}
				m_count_pattern = 0;
			}
		}
	}
	else if (m_bAllConnect == true)
	{
		//�}�b�vBG����������Ă�����
		if (m_pMapBg != nullptr)
		{
			//����
			m_pMapBg->Uninit();
			m_pMapBg = nullptr;
		}

		if (m_now_loding != nullptr)
		{
			m_now_loding->Uninit();
			m_now_loding = nullptr;

			//���X�|�[��
			RespawnPlayer();

			//�X�R�AUI�̐���
			m_pScorUiTop = CScoreUi::Create(GAME01_SCORE_UI_POS_00, { 1.0f, 1.0f, 1.0f });
			m_pScorUiUnder = CScoreUi::Create(GAME01_SCORE_UI_POS_01, { 1.0f, 1.0f, 1.0f });

			//��ʂ̃X�R�A��\��
			m_pScorUiTop->SetRank(1);
		}

		//�v���C���[�̃f�[�^�擾
		CCommunicationData::COMMUNICATION_DATA *PlayerDataBuf = CManager::GetInstance()->GetNetWorkmanager()->GetPlayerData()->GetCmmuData();

		//��ʂ�������������
		if (m_pScorUiUnder->GetRankData(0).nKill == PlayerDataBuf->Player.nKill)
		{
			//���UI��1�ʂ�\������悤�ɂ���
			m_pScorUiTop->SetRank(1);

			//����UI�������̏��ʂ�\������ݒ�ɂȂ��Ă���Ȃ�
			if (m_pScorUiUnder->GetPlayerNum())
			{
				//�w�肵�����ʂ�\���ł����Ԃɂ���
				m_pScorUiUnder->SetPlayerNum(false);
			}
			
			//����UI��2�ʂ�\������悤�ɂ���
			m_pScorUiUnder->SetRank(2);
		}
		else
		{//��ʂ���������Ȃ�������
			//���UI��1�ʂ�\������悤�ɂ���
			m_pScorUiTop->SetRank(1);
			//����UI�������̏��ʂ�\������ݒ�ɂȂ��Ă��Ȃ��Ȃ�
			if (!m_pScorUiUnder->GetPlayerNum())
			{
				//�����̏��ʂ����\�������Ԃɂ���
				m_pScorUiUnder->SetPlayerNum(true);
			}
		}
	}

	//�L�[�{�[�h�擾����
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

#ifdef _DEBUG
	//Enter�L�[����������
	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		//�t�F�[�h�擾����
		CFade *pFade;
		pFade = CManager::GetInstance()->GetFade();

		if (pFade->GetFade() == CFade::FADE_NONE)
		{
			pFade->SetFade(CManager::MODE::RESULT);
		}
	}

#endif // !_DEBUG

	//ESC�L�[����������
	if (pInputKeyboard->GetTrigger(DIK_ESCAPE) == true)
	{
		//�ݒ��ʂ𐶐����Ă���Ȃ�
		if (m_pOption != nullptr)
		{
			//�ݒ��ʂ��J���Ă��Ȃ��Ȃ�
			if (!m_pOption->GetOpen())
			{
				//�ݒ��ʂ��J��
				m_pOption->Open();
			}
			else
			{
				//�ݒ��ʂ����
				m_pOption->Close();
			}
		}
	}
}

//================================================
//�`�揈��
//================================================
void CGame01::Draw(void)
{

}

//================================================
//�}�b�v�̈ړ�����
//================================================
bool CGame01::MapLimit(CObject* pObj)
{
	bool bHit = false;

	D3DXVECTOR3 objPos = pObj->GetPos();
	D3DXVECTOR3 size = m_pMeshField->GetSize();
	D3DXVECTOR3 pos = m_pMeshField->GetPos();

	if (objPos.x >= pos.x + size.x / 2.0f)
	{
		objPos.x = pos.x + size.x / 2.0f;
		pObj->SetPos(objPos);
		bHit = true;
	}
	else if (objPos.x <= pos.x - size.x / 2.0f)
	{
		objPos.x = pos.x - size.x / 2.0f;
		pObj->SetPos(objPos);
		bHit = true;
	}

	objPos = pObj->GetPos();

	if (objPos.z >= pos.z + size.z / 2.0f)
	{
		objPos.z = pos.z + size.z / 2.0f;
		pObj->SetPos(objPos);
		bHit = true;
	}
	else if (objPos.z <= pos.z - size.z / 2.0f)
	{
		objPos.z = pos.z - size.z / 2.0f;
		pObj->SetPos(objPos);
		bHit = true;
	}

	return bHit;
}

//================================================
//�������̎擾
//================================================
void CGame01::FirstContact(void)
{
	CTcpClient *pClient = CManager::GetInstance()->GetNetWorkmanager()->GetCommunication();
	string buf = CManager::GetInstance()->GetPlayData()->GetName();
	CCommunicationData::COMMUNICATION_DATA *PlayerDataBuf = CManager::GetInstance()->GetNetWorkmanager()->GetPlayerData()->GetCmmuData();

	pClient->Init();
	pClient->Connect();

	memset(PlayerDataBuf->Player.aName[0], NULL, sizeof(PlayerDataBuf->Player.aName[0]));
	memcpy(PlayerDataBuf->Player.aName[0], buf.c_str(), buf.size());

	if (pClient->GetConnect() == true)
	{
		CManager::GetInstance()->GetNetWorkmanager()->CreateThread();

		bool bLoop = true;
		while (bLoop)
		{
			switch (PlayerDataBuf->Player.nNumber)
			{
			case 1:
				m_pPlayer = CPlayer::Create(D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				bLoop = false;
				break;
			case 2:
				m_pPlayer = CPlayer::Create(D3DXVECTOR3(-1000.0f, 1000.0f, 1000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				bLoop = false;
				break;
			case 3:
				m_pPlayer = CPlayer::Create(D3DXVECTOR3(1000.0f, 1000.0f, -1000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				bLoop = false;
				break;
			case 4:
				m_pPlayer = CPlayer::Create(D3DXVECTOR3(-1000.0f, 1000.0f, -1000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				bLoop = false;
				break;
			default:
				m_pPlayer = CPlayer::Create(D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				bLoop = false;
				break;
			}
		}
	}
	else
	{
		m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 1000.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	for (int count_enemy = 0; count_enemy < MAX_PLAYER; count_enemy++)
	{
		m_pEnemy.push_back(CEnemy::Create());
	}

	//�}�b�vBG�̐���
	m_pMapBg = CObject2D::Create({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f }, { SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f }, (int)CObject::PRIORITY::UI);
	m_pMapBg->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("map_000.jpg"));
	m_pMapBg->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	m_now_loding = CObject2D::Create({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f }, { 800.0f, 250.0f, 0.0f }, (int)CObject::PRIORITY::UI);
	m_now_loding->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("matching.png"));
	m_now_loding->SetTex(m_pattern_tex, 4);
	m_now_loding->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
}

//================================================
//�G�̃��f���擾����
//================================================
vector<CEnemy*> CGame01::GetEnemy(void)
{
	return m_pEnemy;
}

//================================================
//�v���C���[���X�|�[���֐�
//================================================
void CGame01::RespawnPlayer(void)
{
	//���X�|�[���̏ꏊ�ۑ��p�ϐ�
	D3DXVECTOR3 respawnPos = { 0.0f, 0.0f, 0.0f };

	//�G�̃f�[�^�擾
	vector<CCommunicationData> data = CManager::GetInstance()->GetNetWorkmanager()->GetEnemyData();

	int nCntTrue = 0;
	while (1)
	{
		//���X�|�[���ʒu�𐔎��Ŏw��
		m_respawnPos = static_cast<PlayerRespawnPos>(rand() % (static_cast<int>(PlayerRespawnPos::MAX) - 1) + 1);

		//�ݒ肵�����l�ɂ���ă��X�|�[���ꏊ��ς���
		switch (m_respawnPos)
		{
		case CGame01::PlayerRespawnPos::POS_00:
			respawnPos = GAME01_PLAYER_RESPAWN_POS_00;
			break;
		case CGame01::PlayerRespawnPos::POS_01:
			respawnPos = GAME01_PLAYER_RESPAWN_POS_01;
			break;
		case CGame01::PlayerRespawnPos::POS_02:
			respawnPos = GAME01_PLAYER_RESPAWN_POS_02;
			break;
		case CGame01::PlayerRespawnPos::POS_03:
			respawnPos = GAME01_PLAYER_RESPAWN_POS_03;
			break;
		case CGame01::PlayerRespawnPos::POS_04:
			respawnPos = GAME01_PLAYER_RESPAWN_POS_04;
			break;
		case CGame01::PlayerRespawnPos::POS_05:
			respawnPos = GAME01_PLAYER_RESPAWN_POS_05;
			break;
		case CGame01::PlayerRespawnPos::POS_06:
			respawnPos = GAME01_PLAYER_RESPAWN_POS_06;
			break;
		case CGame01::PlayerRespawnPos::POS_07:
			respawnPos = GAME01_PLAYER_RESPAWN_POS_07;
			break;
		default:
			break;
		}

		int nEnemyNum = data.size();

		for (int nCntEnemy = 0; nCntEnemy < nEnemyNum; nCntEnemy++)
		{
			//�G���ڑ�����Ă�����
			if (data[nCntEnemy].GetCmmuData()->bConnect)
			{
				//�G�̈ʒu�Ƃ��Ԃ��Ă��Ȃ�������
				if (m_respawnPos != static_cast<PlayerRespawnPos>(data[nCntEnemy].GetCmmuData()->Player.nRespawnPos))
				{
					//�f�[�^�擾
					CCommunicationData::COMMUNICATION_DATA *PlayerData = CManager::GetInstance()->GetNetWorkmanager()->GetPlayerData()->GetCmmuData();
					PlayerData->Player.nRespawnPos = static_cast<int>(m_respawnPos);
					nCntTrue++;
				}
			}
		}

		if (nCntTrue == nEnemyNum)
		{
			break;
		}
	}

	//�v���C���[����������Ă�����
	if (m_pPlayer != nullptr)
	{
		//�v���C���[�̈ʒu��ݒ�
		m_pPlayer->SetPos(respawnPos);
	}
}

//================================================
//���f�����[�h����
//================================================
void CGame01::LoadModelTxt(const string &Pas)
{
	vector<string> txt_data;	// �e�L�X�g�t�@�C���̕ۑ��o�b�t�@
	vector<CFileLoad::STAGE_ALLOCATION_DATA> stage;	// �X�e�[�W���

	txt_data = CFileLoad::LoadTxt(Pas);
	stage = CFileLoad::CreateStageAllocation(txt_data);
	int stage_element = stage.size();
	for (int count_stage = 0; count_stage < stage_element; count_stage++)
	{
		CModelSingle::Create(stage[count_stage].pos, stage[count_stage].rot, stage[count_stage].pas, NULL, stage[count_stage].coll);
	}
}