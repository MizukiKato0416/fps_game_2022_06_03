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
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 1000.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI / 2.0f, 0.0f));

	//メッシュフィールド生成
	CMeshField *pMeshField = CMeshField::CreateLoadText("data/mesh_field.txt");
	pMeshField->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("grass001.png"));
	//CFloor *pFloor = CFloor::Create(D3DXVECTOR3(0.0f, 60.0f, 0.0f), D3DXVECTOR3(500.0f, 000.0f, 500.0f), D3DXVECTOR3(0.2f, 0.0f, 0.0f));
	//pFloor->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("grass001.png"));

	//メッシュスフィア生成
	CMeshSphere *pMeshSphere = CMeshSphere::Create(D3DXVECTOR3(0.0f, -100.0f, 0.0f), D3DXVECTOR3(5000.0f, 5000.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		                                           32, 8);
	pMeshSphere->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("sky.jpg"));

	CObject3D *pObject3D = CObject3D::Create(D3DXVECTOR3(0.0f, -100.0f, 0.0f), D3DXVECTOR3(5000.0f, 0.0f, 5000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pObject3D->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("mist.png"));

	CModelSingle::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), "01_body.x", NULL, true);
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
#endif // !_DEBUG
}

//================================================
//描画処理
//================================================
void CGame01::Draw(void)
{

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