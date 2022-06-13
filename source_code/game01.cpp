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
#include "meshsphere.h"
#include "model_single.h"
#include "tcp_client.h"
#include "enemy.h"
#include "PresetSetEffect.h"

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
	CTcpClient *pClient = CManager::GetInstance()->GetCommunication();
	pClient->Connect();

	//プレイヤーの生成
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 1000.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI / 2.0f, 0.0f));
	CEnemy *enemy;
	//enemy = CEnemy::Create(D3DXVECTOR3(0.0f, 1000.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//メッシュフィールド生成
	m_pMeshField = CMeshField::CreateLoadText("data/mesh_field.txt");
	m_pMeshField->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("grass001.png"));

	//メッシュスフィア生成
	CMeshSphere *pMeshSphere = CMeshSphere::Create(D3DXVECTOR3(0.0f, -100.0f, 0.0f), D3DXVECTOR3(GAME01_FIELD_SIZE, GAME01_FIELD_SIZE, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		                                           32, 8);
	pMeshSphere->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("sky.jpg"));

	CObject3D *pObject3D = CObject3D::Create(D3DXVECTOR3(0.0f, -100.0f, 0.0f), D3DXVECTOR3(GAME01_FIELD_SIZE, 0.0f, GAME01_FIELD_SIZE), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pObject3D->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("mist.png"));

	CModelSingle::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), "01_body.x", NULL, true);
	CModelSingle::Create(D3DXVECTOR3(50.0f, 40.0f, 50.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), "fan.x", NULL, true);
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
	if (pInputKeyboard->GetTrigger(DIK_DELETE) == true)
	{
		CPresetEffect::SetEffect3D(0, D3DXVECTOR3(0.0f, 100.0f, 0.0f), {});
		CPresetEffect::SetEffect3D(1, D3DXVECTOR3(0.0f, 100.0f, 0.0f), {});
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