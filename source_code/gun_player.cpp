//=============================================================================
//
// プレイヤーが持っている銃処理 [modeltester.cpp]
// Author : 加藤瑞葵
//
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include "gun_player.h"
#include "manager.h"
#include "renderer.h"
#include "model_single.h"
#include "model.h"
#include "communicationdata.h"
#include "input_mouse.h"
#include "player.h"
#include "manager.h"
#include "PresetSetEffect.h"
#include "networkmanager.h"
#include "xanimmodel.h"


//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CGunPlayer::CGunPlayer(CObject::PRIORITY Priority) : CObject(Priority)
{

}

//=============================================================================
// デフォルトデストラクタ
//=============================================================================
CGunPlayer::~CGunPlayer()
{

}

//=============================================================================
// モデルの初期化処理
//=============================================================================
HRESULT CGunPlayer::Init(void)
{
	m_model = CModelSingle::Create(m_pos, {0.0f, 2.0f, 0.0f}, m_type, NULL, false);

	return S_OK;
}

//=============================================================================
// モデルの終了処理
//=============================================================================
void CGunPlayer::Uninit(void)
{
	Release();
}

//=============================================================================
// モデルの更新処理
//=============================================================================
void CGunPlayer::Update(void)
{
	
}


//=============================================================================
// モデルの描画処理
//=============================================================================
void CGunPlayer::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice; // デバイスのポインタ
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	// デバイスを取得する

	//--------------------------------------
	//プレイヤー(原点)のマトリックスの設定
	//--------------------------------------
	D3DXMATRIX mtx_rot, mtx_trans, mtx_parent;	//計算用マトリックス

	D3DXMatrixIdentity(&m_mtx_muzzle_world);	//マトリックス初期化

	D3DXVECTOR3 rot = m_model->GetModel()->GetRot();

	//向きの設定
	D3DXMatrixRotationYawPitchRoll(	&mtx_rot,
									rot.y,
									rot.x,
									rot.z);

	D3DXMatrixMultiply(	&m_mtx_muzzle_world,
						&m_mtx_muzzle_world,
						&mtx_rot);
	//位置
	D3DXMatrixTranslation(	&mtx_trans,
							m_muzzle_pos.x,
							m_muzzle_pos.y,
							m_muzzle_pos.z);

	D3DXMatrixMultiply(	&m_mtx_muzzle_world,
						&m_mtx_muzzle_world,
						&mtx_trans);
	//マトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD,
						  &m_mtx_muzzle_world);

	m_model->GetModel()->SetMtx();
	*m_mtxParent = m_model->GetModel()->GetMtx();
	mtx_parent = *m_mtxParent;

	D3DXMatrixMultiply(&m_mtx_muzzle_world,
						&m_mtx_muzzle_world,
						&mtx_parent);

	//マトリックスの設定
	pDevice->SetTransform(	D3DTS_WORLD,
							&m_mtx_muzzle_world);
}

//=============================================================================
// モデルの生成
//=============================================================================
CGunPlayer *CGunPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 muzzle_pos, string type)
{
	CGunPlayer *gun = NULL;
	if (gun == NULL)
	{
		gun = new CGunPlayer;
	}

	gun->m_pos = pos;
	gun->m_rot = rot;
	gun->m_muzzle_pos = muzzle_pos;
	gun->m_type = type;

	if (gun != NULL)
	{
		gun->Init();
	}
	return gun;
}

//=============================================================================
//射撃処理
//=============================================================================
void CGunPlayer::Shot(void)
{
	////マウス取得処理
	//CInputMouse *pInputMouse;
	//pInputMouse = CManager::GetInstance()->GetInputMouse();

	//// 通信データ取得処理
	//CCommunicationData::COMMUNICATION_DATA *pData = CManager::GetInstance()->GetNetWorkmanager()->GetPlayerData()->GetCmmuData();

	////オブジェクト情報を入れるポインタ
	//vector<CObject*> object;

	////先頭のポインタを代入
	//object = CObject::GetObject(static_cast<int>(CObject::PRIORITY::PLAYER));
	//int nProprty_Size = object.size();

	//for (int nCnt = 0; nCnt < nProprty_Size; nCnt++)
	//{
	//	//オブジェクトの種類がプレイヤーだったら
	//	if (object[nCnt]->GetObjType() == CObject::OBJTYPE::PLAYER)
	//	{
	//		//プレイヤーにキャスト
	//		CPlayer *pPlayerObj = nullptr;
	//		pPlayerObj = (CPlayer*)object[nCnt];

	//		if (pInputMouse->GetPress(CInputMouse::MOUSE_TYPE::MOUSE_TYPE_LEFT) == true)
	//		{
	//			//移動量取得
	//			D3DXVECTOR3 move = pPlayerObj->GetMove();
	//			//モデル取得
	//			CXanimModel *pAnimodel = pPlayerObj->GetAnimModel();

	//			if (move.x != 0.0f || move.z != 0.0f)
	//			{
	//				//撃つアニメーションでなかったら
	//				if (pAnimodel->GetAnimation() != "fireneutral")
	//				{
	//					//撃つモーションにする
	//					pPlayerObj->SetAnimSpeed((20.0f * 3.0f) / 4800.0f);
	//					pAnimodel->ChangeAnimation("fireneutral", pPlayerObj->GetAnimSpeed());
	//					memset(pData->Player.aMotion[0], NULL, sizeof(pData->Player.aMotion[0]));
	//					memcpy(pData->Player.aMotion[0], pAnimodel->GetAnimation().c_str(), pAnimodel->GetAnimation().size());
	//				}
	//			}
	//			else
	//			{
	//				//撃つアニメーションでなかったら
	//				if (pAnimodel->GetAnimation() != "fireneutral")
	//				{
	//					//撃つモーションにする
	//					pPlayerObj->SetAnimSpeed((20.0f * 3.0f) / 4800.0f);
	//					pAnimodel->ChangeAnimation("fireneutral", pPlayerObj->GetAnimSpeed());
	//					memset(pData->Player.aMotion[0], NULL, sizeof(pData->Player.aMotion[0]));
	//					memcpy(pData->Player.aMotion[0], pAnimodel->GetAnimation().c_str(), pAnimodel->GetAnimation().size());
	//				}
	//			}

	//			//カウンターを減算
	//			m_nCounter--;

	//			//撃っている状態なら
	//			if (m_bShot == true)
	//			{
	//				D3DXVECTOR3 pos = { 0.0f, 0.0f,0.0f };
	//				//ADSしていなかったら
	//				if (!m_bAds)
	//				{
	//					m_pGunPlayer->GetModel()->GetModel()->SetMtx();
	//					//オフセット位置設定
	//					pos = { m_pGunPlayer->GetMuzzleMtx()._41, m_pGunPlayer->GetMuzzleMtx()._42, m_pGunPlayer->GetMuzzleMtx()._43 };
	//				}
	//				else
	//				{//ADSしたら
	//					m_pGunPlayerAds->GetModel()->GetModel()->SetMtx();
	//					//オフセット位置設定
	//					pos = { m_pGunPlayerAds->GetMuzzleMtx()._41, m_pGunPlayerAds->GetMuzzleMtx()._42, m_pGunPlayerAds->GetMuzzleMtx()._43 };
	//				}


	//				//ADS状態なら
	//				if (m_bAds)
	//				{
	//					//マズルフラッシュエフェクトの生成
	//					CPresetEffect::SetEffect3D(7, pos, {}, {});
	//					CPresetEffect::SetEffect3D(8, pos, {}, {});
	//				}
	//				else
	//				{//ADS状態でないなら
	//				 //マズルフラッシュエフェクトの生成
	//					CPresetEffect::SetEffect3D(0, pos, {}, {});
	//					CPresetEffect::SetEffect3D(1, pos, {}, {});
	//				}

	//				CBullet *pBullet;	// 弾のポインタ
	//									//弾の生成
	//				pBullet = CBullet::Create();

	//				//cameraのポインタ配列1番目のアドレス取得
	//				CCamera **pCameraAddress = CManager::GetInstance()->GetCamera();

	//				for (int nCntCamera = 0; nCntCamera < MAX_MAIN_CAMERA; nCntCamera++, pCameraAddress++)
	//				{
	//					//cameraの取得
	//					CCamera *pCamera = &**pCameraAddress;
	//					if (pCamera != nullptr)
	//					{
	//						//反動を設定
	//						D3DXVECTOR3 rotV = pCamera->GetRotV();
	//						rotV.x -= 0.02f;
	//						rotV.y -= ((rand() % 30 + -15) / 1000.0f);
	//						pCamera->SetRotV(rotV);
	//					}
	//				}

	//				//撃っていない状態にする
	//				m_bShot = false;
	//			}

	//			//0より小さくなったら
	//			if (m_nCounter < 0)
	//			{
	//				//既定の値にする
	//				m_nCounter = PLAYER_SHOT_COUNTER;

	//				//撃ってる状態にする
	//				m_bShot = true;
	//			}
	//		}
	//		else
	//		{
	//			//撃っていない状態にする
	//			m_bShot = false;
	//			//弾を使ってない
	//			pData->Bullet.bUse = false;
	//			pData->Bullet.fDiffer = 0.0f;
	//			pData->Bullet.hitPos = { 0.0f, 0.0f, 0.0f };
	//			pData->Bullet.nDamage = 0;
	//			pData->Bullet.type = CCommunicationData::HIT_TYPE::NONE;
	//			pData->Ballistic.BigenPos = { 0.0f, 0.0f, 0.0f };
	//			pData->Ballistic.EndPos = { 0.0f, 0.0f, 0.0f };
	//			pData->Ballistic.fSpeed = 0.0f;
	//			pData->Ballistic.Rot = { 0.0f, 0.0f, 0.0f };
	//			pData->Ballistic.Size = { 0.0f, 0.0f, 0.0f };

	//			//撃つアニメーションだったら
	//			if (m_pAnimModel->GetAnimation() == "fireneutral")
	//			{
	//				//ニュートラルモーションにする
	//				m_fAnimSpeed = (20.0f * 3.0f) / 4800.0f;
	//				m_pAnimModel->ChangeAnimation("neutral", m_fAnimSpeed);
	//				memset(pData->Player.aMotion[0], NULL, sizeof(pData->Player.aMotion[0]));
	//				memcpy(pData->Player.aMotion[0], m_pAnimModel->GetAnimation().c_str(), m_pAnimModel->GetAnimation().size());
	//			}

	//			//既定の値より小さかったら
	//			if (m_nCounter > 0 && m_nCounter <= PLAYER_SHOT_COUNTER)
	//			{
	//				//カウンターを加算
	//				m_nCounter++;

	//				//既定の値より大きくなったら
	//				if (m_nCounter > PLAYER_SHOT_COUNTER)
	//				{
	//					//0にする
	//					m_nCounter = 0;
	//				}
	//			}
	//		}
	//	}
	//}
}
