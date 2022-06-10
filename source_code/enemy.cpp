//=============================================================================
//
// モデルテスト処理 [modeltester.cpp]
// Author : 羽鳥太一
//
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "tcp_client.h"
#include "xanimmodel.h"
#include <thread>

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CCommunicationData CEnemy::m_commu_data[MAX_PLAYER];

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CEnemy::CEnemy(CObject::PRIORITY Priority) : CObject(Priority)
{
	for (int count_enemy = 0; count_enemy < MAX_PLAYER; count_enemy++)
	{
		m_pos[count_enemy] = { 0.0f, 0.0f, 0.0f };
		m_rot[count_enemy] = { 0.0f, 0.0f, 0.0f };
	}
}

//=============================================================================
// デフォルトデストラクタ
//=============================================================================
CEnemy::~CEnemy()
{
	
}

//=============================================================================
// モデルの初期化処理
//=============================================================================
HRESULT CEnemy::Init(void)
{
	for (int count_enemy = 0; count_enemy < MAX_PLAYER; count_enemy++)
	{
		m_model[count_enemy] = CXanimModel::Create("data/motion.x");
		m_model[count_enemy]->ChangeAnimation(1, 60.0f / 4800.0f);
	}

	std::thread th(Recv);

	th.detach();

	return S_OK;
}

//=============================================================================
// モデルの終了処理
//=============================================================================
void CEnemy::Uninit(void)
{
	Release();
	for (int count_enemy = 0; count_enemy < MAX_PLAYER; count_enemy++)
	{
		m_model[count_enemy]->Uninit();
	}
}

//=============================================================================
// モデルの更新処理
//=============================================================================
void CEnemy::Update(void)
{
	for (int count_enemy = 0; count_enemy < MAX_PLAYER; count_enemy++)
	{
		m_model[count_enemy]->Update();
	}
	
	Move();
	Attack();
}

//=============================================================================
// モデルの描画処理
//=============================================================================
void CEnemy::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice; // デバイスのポインタ
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスを取得する

	for (int count_enemy = 0; count_enemy < MAX_PLAYER; count_enemy++)
	{
		//--------------------------------------
		//プレイヤー(原点)のマトリックスの設定
		//--------------------------------------
		D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

		D3DXMatrixIdentity(&m_mtx_wld[count_enemy]);	//マトリックス初期化

										//向きの設定
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			m_rot[count_enemy].y,
			m_rot[count_enemy].x,
			m_rot[count_enemy].z);

		D3DXMatrixMultiply(&m_mtx_wld[count_enemy],
			&m_mtx_wld[count_enemy],
			&mtxRot);
		//位置
		D3DXMatrixTranslation(&mtxTrans,
			m_pos[count_enemy].x,
			m_pos[count_enemy].y,
			m_pos[count_enemy].z);

		D3DXMatrixMultiply(&m_mtx_wld[count_enemy],
			&m_mtx_wld[count_enemy],
			&mtxTrans);
		//マトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD,
			&m_mtx_wld[count_enemy]);
		m_model[count_enemy]->Draw();
	}
}

//=============================================================================
// モデルの生成
//=============================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CEnemy *pEnemy = NULL;
	if (pEnemy == NULL)
	{
		pEnemy = new CEnemy;
	}

	if (pEnemy != NULL)
	{
		pEnemy->Init();
	}
	return pEnemy;
}

//=============================================================================
// レシーブスレッド
//=============================================================================
void CEnemy::Recv(void)
{
	int size = 1;

	while (size > 0)
	{
		CTcpClient *pTcp = CManager::GetInstance()->GetCommunication();
		char recv[MAX_COMMU_DATA];

		for (int count_enemy = 0; count_enemy < MAX_PLAYER; count_enemy++)
		{
			CCommunicationData::COMMUNICATION_DATA *pData = m_commu_data[count_enemy].GetCmmuData();

			size = pTcp->Recv(&recv[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			memcpy(pData, &recv[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
		}
	}
}

//=============================================================================
// 攻撃
//=============================================================================
void CEnemy::Attack(void)
{
	
}

//=============================================================================
// 移動
//=============================================================================
void CEnemy::Move(void)
{
	for (int count_enemy = 0; count_enemy < MAX_PLAYER; count_enemy++)
	{
		CCommunicationData::COMMUNICATION_DATA *pData = m_commu_data[count_enemy].GetCmmuData();

		m_pos[count_enemy] = pData->Player.Pos;
		m_rot[count_enemy] = pData->Player.Rot;
	}
}