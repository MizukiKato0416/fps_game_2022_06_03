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
#include "gunmodel.h"
#include "player.h"
#include "PresetSetEffect.h"
#include "ballistic.h"
#include "player.h"
#include "networkmanager.h"
#include "shadow.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
int CEnemy::m_all_count = 0;	// 敵のカウンター

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CEnemy::CEnemy(CObject::PRIORITY Priority) : CObject(Priority)
{
	m_pos = { 0.0f, 0.0f, 0.0f };
	m_recvPos = { 0.0f, 0.0f, 0.0f };
	m_posOld = { 0.0f, 0.0f, 0.0f };
	m_rot = { 0.0f, 0.0f, 0.0f };
	m_recvRot = { 0.0f, 0.0f, 0.0f };
	m_rotOld = { 0.0f, 0.0f, 0.0f };
	m_size = { 0.0f, 0.0f, 0.0f };
	m_nLife = 0;
	m_my_number = m_all_count;
	m_all_count++;
	m_pShadow = nullptr;
}

//=============================================================================
// デフォルトデストラクタ
//=============================================================================
CEnemy::~CEnemy()
{
	m_all_count = 0;
}

//=============================================================================
// モデルの初期化処理
//=============================================================================
HRESULT CEnemy::Init(void)
{
	m_posOld = m_pos;
	m_recvPos = m_pos;

	m_rotOld = m_rot;
	m_recvRot = m_rot;

	SetObjType(CObject::OBJTYPE::ENEMY);
	m_nLife = PLAYER_LIFE;

	m_model = CXanimModel::Create("data/motion.x");
	m_model->ChangeAnimation("neutral", 60.0f / 4800.0f);
	//銃モデルの生成
	m_pGunModel = CGunModel::Create({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.6f, 12.0f }, "asult_gun_inv.x");
	m_pGunModel->SetMtxParent(m_pGunModel->GetModel()->GetModel()->GetMtxPoint());

	//サイズを取得
	D3DXVECTOR3 modelSize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	modelSize = m_model->GetSize();

	//サイズのXとZを比べて大きいほうをXとZそれぞれに割り当てる
	if (modelSize.x >= modelSize.z)
	{
		m_size = D3DXVECTOR3(modelSize.x + PLAYER_SIZE, modelSize.y, modelSize.x + PLAYER_SIZE);
	}
	else
	{
		m_size = D3DXVECTOR3(modelSize.z + PLAYER_SIZE, modelSize.y, modelSize.z + PLAYER_SIZE);
	}

	//サイズの設定
	SetSize(m_size);

	//影の設定
	m_pShadow = CShadow::Create(D3DXVECTOR3(m_pos.x, 0.0f, m_pos.z), D3DXVECTOR3(m_size.x, 0.0f, m_size.z), this);

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

	SetPos(m_pos);

	//影の当たり判定
	m_pShadow->Collision(m_pos, m_size.x * 20.0f);

	CPlayer::Collision(this, m_size.x / 2.0f);
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

	m_model->SetMatrix(&m_mtx_wld);
	m_model->Draw();

	//マトリックスを取得
	D3DXMATRIX *hand = nullptr;
	hand = m_model->GetMatrix("handR");
	m_pGunModel->SetMtxParent(m_pGunModel->GetModel()->GetModel()->GetMtxPoint());
	//銃と親子関係をつける
	m_pGunModel->GetModel()->GetModel()->SetMtxParent(hand);
	m_pGunModel->GetModel()->GetModel()->SetObjParent(true);
	m_pGunModel->GetModel()->GetModel()->SetRot({ 0.0f, D3DX_PI / 2.0f, 0.0f });
	m_pGunModel->GetModel()->GetModel()->SetPos({ 0.0f, 0.0f, 0.0f });
	m_pGunModel->GetModel()->SetCulliMode(false);
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
// 攻撃
//=============================================================================
void CEnemy::Attack(void)
{
	vector<CCommunicationData> data = CManager::GetInstance()->GetNetWorkmanager()->GetEnemyData();

	//敵が撃ってきたら
	if (data[m_my_number].GetCmmuData()->Bullet.bUse == true)
	{
		//銃口のマトリックス
		D3DXMATRIX mtx = m_pGunModel->GetMuzzleMtx();
		D3DXVECTOR3 gunPos = { mtx._41, mtx._42, mtx._43 };

		//弾の軌道エフェクトを生成
		CBallistic::Create(gunPos, data[m_my_number].GetCmmuData()->Ballistic.Size, data[m_my_number].GetCmmuData()->Ballistic.Rot, data[m_my_number].GetCmmuData()->Ballistic.EndPos,
			data[m_my_number].GetCmmuData()->Ballistic.fSpeed, "bullet_00.png", "bullet_01.png");

		//マズルフラッシュエフェクトの生成
		CPresetEffect::SetEffect3D(0, gunPos, {}, {});
		CPresetEffect::SetEffect3D(1, gunPos, {}, {});
	}
}

//=============================================================================
// 移動
//=============================================================================
void CEnemy::Move(void)
{
	vector<CCommunicationData> data = CManager::GetInstance()->GetNetWorkmanager()->GetEnemyData();
	string now_motion;
	string commu_motion = data[m_my_number].GetCmmuData()->Player.aMotion[0];

	//死んでいなかったら
	if (!data[m_my_number].GetCmmuData()->Player.bDeath)
	{
		if (data[m_my_number].GetCmmuData()->bConnect == true)
		{
			m_posOld = m_pos;
			m_rotOld = m_rot;
			m_recvPos = data[m_my_number].GetCmmuData()->Player.Pos;
			m_recvRot = data[m_my_number].GetCmmuData()->Player.Rot;

			now_motion = m_model->GetAnimation();
			m_model->ChangeSpeed(data[m_my_number].GetCmmuData()->Player.fMotionSpeed);
			if (now_motion != commu_motion && data[m_my_number].GetCmmuData()->bConnect == true)
			{
				m_model->ChangeAnimation(commu_motion, data[m_my_number].GetCmmuData()->Player.fMotionSpeed);
			}
		}
		else
		{
			m_pos = { 0.0f, 100.0f, 0.0f };
		}

		//受け取った位置から元の位置までのヴェクトルを算出
		D3DXVECTOR3 posVec = m_recvPos - m_posOld;
		D3DXVECTOR3 rotVec = { 0.0f, 0.0f, 0.0f };

		//現在の向きごとにそれぞれ向きを変える量を計算
		if (m_rotOld.y < 0.0f && -m_rotOld.y + m_recvRot.y > D3DX_PI)
		{
			rotVec.y = (-D3DX_PI - m_rotOld.y) + -(D3DX_PI - m_recvRot.y);
		}
		else if (m_rotOld.y >= D3DX_PI / 2.0f && m_rotOld.y - m_recvRot.y > D3DX_PI)
		{
			rotVec.y = (D3DX_PI - m_rotOld.y) - (-D3DX_PI - m_recvRot.y);
		}
		else
		{
			rotVec.y = (m_recvRot.y - m_rotOld.y);
		}

		//ベクトルを既定の数で割る
		posVec /= (float)SEND_COUNTER;
		rotVec /= (float)SEND_COUNTER;
		//現在位置からベクトル分位置を移動
		m_pos += posVec;
		m_rot += rotVec;

		//πより大きくなったら-2πする
		if (m_rot.y > D3DX_PI)
		{
			m_rot.y -= D3DX_PI * 2.0f;
		}
		else if (m_rot.y < -D3DX_PI)
		{	//-πより小さくなったら+2πする
			m_rot.y += D3DX_PI * 2.0f;
		}
	}
}