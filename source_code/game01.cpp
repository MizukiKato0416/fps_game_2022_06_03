//================================================
//ゲーム01処理
//Author: 加藤瑞葵
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

//================================================
//マクロ定義
//================================================
#define GAME01_FIELD_SIZE		(50000.0f)			//マップの広さ

//================================================
//静的メンバ変数宣言
//================================================

//================================================
//デフォルトコンストラクタ
//================================================
CGame01::CGame01(CObject::PRIORITY Priority):CObject(Priority)
{
	m_pPlayer = nullptr;
	m_pMeshField = nullptr;
	m_bAllConnect = false;
	m_pEnemy.clear();
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
	FirstContact();

	//メッシュフィールド生成
	m_pMeshField = CMeshField::CreateLoadText("data/mesh_field.txt");
	m_pMeshField->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("grass001.png"));

	//メッシュスフィア生成
	CMeshSphere *pMeshSphere = CMeshSphere::Create(D3DXVECTOR3(0.0f, -100.0f, 0.0f), D3DXVECTOR3(GAME01_FIELD_SIZE, GAME01_FIELD_SIZE, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		                                           32, 8);
	pMeshSphere->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("sky.jpg"));

	CObject3D *pObject3D = CObject3D::Create(D3DXVECTOR3(0.0f, -100.0f, 0.0f), D3DXVECTOR3(GAME01_FIELD_SIZE, 0.0f, GAME01_FIELD_SIZE), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pObject3D->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("mist.png"));

	LoadModelTxt("data/model_set.txt");

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
		if (m_now_loding != nullptr)
		{
			m_now_loding->Uninit();
			m_now_loding = nullptr;
		}
	}

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

//================================================
//マップの移動制限
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
//初期情報の取得
//================================================
void CGame01::FirstContact(void)
{
	CTcpClient *pClient = CManager::GetInstance()->GetNetWorkmanager()->GetCommunication();
	m_now_loding = CObject2D::Create({ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f }, { 800.0f, 250.0f, 0.0f }, (int)CObject::PRIORITY::UI);
	m_now_loding->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("matching.png"));
	m_now_loding->SetTex(m_pattern_tex, 4);
	m_now_loding->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

	pClient->Init();
	pClient->Connect();

	CCommunicationData::COMMUNICATION_DATA *PlayerDataBuf = CManager::GetInstance()->GetNetWorkmanager()->GetPlayerData()->GetCmmuData();
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
}

//================================================
//敵のモデル取得処理
//================================================
vector<CEnemy*> CGame01::GetEnemy(void)
{
	return m_pEnemy;
}

//================================================
//モデルロード処理
//================================================
void CGame01::LoadModelTxt(const string &Pas)
{
	//ファイルの読み込み
	FILE *pFile;
	pFile = fopen(Pas.c_str(), "r");
	if (pFile != NULL)
	{
		char cStr[128];
		while (fgets(cStr, 128, pFile) != nullptr)
		{
			if (strncmp("SET_MODEL\n", cStr, 11) == 0)
			{
				char cBuff[1][128];
				string sPas;
				D3DXVECTOR3 pos;
				D3DXVECTOR3 rot;
				int nColl;
				fscanf(pFile, "%*s%*s%s", cBuff);
				fscanf(pFile, "%*s%*s%f%f%f", &pos.x, &pos.y, &pos.z);
				fscanf(pFile, "%*s%*s%f%f%f", &rot.x, &rot.y, &rot.z);
				fscanf(pFile, "%*s%*s%d", &nColl);

				sPas = cBuff[0];
				CModelSingle::Create(pos, rot, sPas, NULL, nColl);
			}
		}
	}
	else
	{
		printf("ファイルが開けませんでした\n");
	}
	fclose(pFile);
}