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
#include "model.h"
#include "model_single.h"
#include "model_collision.h"
#include "player.h"
#include <thread>

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CCommunicationData CEnemy::m_commu_data[MAX_PLAYER] = {};
int CEnemy::m_create_count = 0;
bool CEnemy::m_create_thread = false;

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CEnemy::CEnemy(CObject::PRIORITY Priority) : CObject(Priority)
{
	m_pos = { 0.0f, 0.0f, 0.0f };
	m_rot = { 0.0f, 0.0f, 0.0f };
	m_nLife = 0;
	m_pCollModel = nullptr;
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
	SetObjType(CObject::OBJTYPE::ENEMY);
	m_nLife = PLAYER_LIFE;

	m_my_index = m_create_count;
	m_create_count++;

	m_model = CXanimModel::Create("data/motion.x");
	m_model->ChangeAnimation("nutral", 60.0f / 4800.0f);
	m_pGunModel = CModelSingle::Create({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, "asult_gun.x", nullptr, false);
	//当たり判定ボックスの生成
	m_pCollModel = CModelCollision::Create({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, "player_coll.x", nullptr, true);

	if (m_create_thread == false)
	{
		std::thread th(Recv);

		th.detach();

		m_create_thread = true;
	}

	return S_OK;
}

//=============================================================================
// モデルの終了処理
//=============================================================================
void CEnemy::Uninit(void)
{
	Release();
	m_model->Uninit();
}

//=============================================================================
// モデルの更新処理
//=============================================================================
void CEnemy::Update(void)
{
	m_model->Update();
	
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

	//--------------------------------------
	//プレイヤー(原点)のマトリックスの設定
	//--------------------------------------
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	D3DXMatrixIdentity(&m_mtx_wld);	//マトリックス初期化

	//向きの設定
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y,
		m_rot.x,
		m_rot.z);

	D3DXMatrixMultiply(&m_mtx_wld,
		&m_mtx_wld,
		&mtxRot);
	//位置
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x,
		m_pos.y,
		m_pos.z);

	D3DXMatrixMultiply(&m_mtx_wld,
		&m_mtx_wld,
		&mtxTrans);
	//マトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD,
		&m_mtx_wld);

	m_model->Draw();

	//マトリックスを取得
	D3DXMATRIX *hand = nullptr;
	hand = m_model->GetMatrix("handL");

	//銃と親子関係をつける
	m_pGunModel->GetModel()->SetMtxParent(hand);
	m_pGunModel->GetModel()->SetObjParent(true);

	//親子関係をつける
	m_pCollModel->GetModel()->SetMtxParent(&m_mtx_wld);
	m_pCollModel->GetModel()->SetObjParent(true);
}

//=============================================================================
// モデルの生成
//=============================================================================
CEnemy *CEnemy::Create(void)
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

	while (size >= 0)
	{
		CTcpClient *pTcp = CManager::GetInstance()->GetCommunication();
		char recv[MAX_COMMU_DATA];

		for (int count_enemy = 0; count_enemy < MAX_PLAYER; count_enemy++)
		{
			CCommunicationData::COMMUNICATION_DATA *pData = m_commu_data[count_enemy].GetCmmuData();

			size = pTcp->Recv(&recv[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
			if (size <= 0)
			{
				break;
			}
			else
			{
				memcpy(pData, &recv[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
				m_commu_data[count_enemy].SetCmmuData(*pData);
			}
		}
	}
	for (int count_enemy = 0; count_enemy < MAX_PLAYER; count_enemy++)
	{
		CCommunicationData::COMMUNICATION_DATA *pData = m_commu_data[count_enemy].GetCmmuData();
		pData->bConnect = false;
	}
}

//=============================================================================
// 攻撃
//=============================================================================
void CEnemy::Attack(void)
{
	CCommunicationData::COMMUNICATION_DATA *pData = m_commu_data[m_my_index].GetCmmuData();

	for (int bullet_count = 0; bullet_count < MAX_BULLET; bullet_count++)
	{
		if (pData->Bullet[bullet_count].bUse == true)
		{
			pData->Bullet[bullet_count].bUse = false;
		}
	}
}

//=============================================================================
// 移動
//=============================================================================
void CEnemy::Move(void)
{
	CCommunicationData::COMMUNICATION_DATA *pData = m_commu_data[m_my_index].GetCmmuData(); 
	string now_motion;
	string commu_motion = pData->Player.aMotion[0];

	if (pData->bConnect == true)
	{
		m_pos = pData->Player.Pos;
		m_rot = pData->Player.Rot;

		now_motion = m_model->GetAnimation();
		if (now_motion != commu_motion && pData->bConnect == true)
		{
			m_model->ChangeAnimation(commu_motion, pData->Player.fMotionSpeed);
		}
	}
	else
	{
		m_pos.y = 100;
	}
}