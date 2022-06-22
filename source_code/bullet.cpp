//================================================
//弾処理
//Author: 加藤瑞葵
//================================================
#include "bullet.h"
#include "manager.h"
#include "fade.h"
#include "model.h"
#include "model_single.h"
#include "model_collision.h"
#include "camera.h"
#include "PresetSetEffect.h"
#include "Object3D.h"
#include "renderer.h"
#include "player.h"
#include "gunmodel.h"
#include "mesh_field.h"
#include "xanimmodel.h"
#include "hierarchydata.h"
#include "enemy.h"

//================================================
//マクロ定義
//================================================
#define BULLET_MOVE_SPEED		(350.0f)		//軌道の速さ
#define BULLET_SIZE_X			(300.0f)		//軌道のサイズ
#define BULLET_SIZE_Y			(3.0f)			//軌道のサイズ
#define BULLET_MAX_END_POS		(10000.0f)		//軌道の終点最大値
#define BULLET_DAMAGE			(20)			//ダメージ

//================================================
//静的メンバ変数宣言
//================================================

//================================================
//デフォルトコンストラクタ
//================================================
CBullet::CBullet(CObject::PRIORITY Priority) :CObject(Priority)
{
	m_bigenPos = { 0.0f, 0.0f, 0.0f };
	m_endPos = {0.0f, 0.0f, 0.0f};
	m_rayVec = { 0.0f, 0.0f, 0.0f };
	memset(m_apOrbit, NULL, sizeof(m_apOrbit[BULLET_MAX_ORBIT]));
	m_fDiffer = 0.0f;
	m_nPlayer = 0;
	m_nDamage = 0;
}

//================================================
//オーバーロードされたコンストラクタ
//================================================


//================================================
//デストラクタ
//================================================
CBullet::~CBullet()
{

}

//================================================
//初期化処理
//================================================
HRESULT CBullet::Init(void)
{
	//カメラの位置保存用
	D3DXVECTOR3 posCameraV = { 0.0f, 0.0f, 0.0f };
	D3DXVECTOR3 posCameraR = { 0.0f, 0.0f, 0.0f };
	//カメラの向き保存
	D3DXVECTOR3 rotCamera = { 0.0f, 0.0f, 0.0f };
	//レイを飛ばす方向
	D3DXVECTOR3 rayVecHit = { 0.0f, 0.0f, 0.0f };
	//レイの当たり判定結果保存用
	BOOL bHit = false;
	//当たった場所までの距離保存用
	float fDiffer = 0.0f;
	//一番プレイヤーに近いモデルの当たった距離
	m_fDiffer = 100000.0f;
	//当たったオブジェクトのポインタ
	CObject *pHitObject = nullptr;
	//カメラのローカル座標
	D3DXVECTOR3 hitPosV = { 0.0f, 0.0f, 0.0f };
	//モデルにあたったかどうか
	bool bHitAny = false;
	//敵にあたったかどうか
	bool bHitEnemy = false;

	//cameraのポインタ配列1番目のアドレス取得
	CCamera **pCameraAddress = CManager::GetInstance()->GetCamera();

	for (int nCntCamera = 0; nCntCamera < MAX_MAIN_CAMERA; nCntCamera++, pCameraAddress++)
	{
		//cameraの取得
		CCamera *pCamera = &**pCameraAddress;
		if (pCamera != nullptr)
		{
			//カメラの位置取得
			posCameraV = pCamera->GetPosV();
			posCameraR = pCamera->GetPosR();
			//カメラの向き取得
			rotCamera = pCamera->GetRotV();

			//レイを飛ばす方向を算出
			m_rayVec = posCameraR - posCameraV;
			//ベクトルを正規化
			D3DXVec3Normalize(&m_rayVec, &m_rayVec);
			//始点を設定
			m_bigenPos = posCameraV;
			//終点を設定
			m_endPos = posCameraV + m_rayVec * BULLET_MAX_END_POS;
		}
	}

	//オブジェクト情報を入れるポインタ
	vector<CObject*> object;

	//先頭のポインタを代入
	object = CObject::GetObject(static_cast<int>(CObject::PRIORITY::MODEL));
	int nProprty_Size = object.size();

	for (int nCnt = 0; nCnt < nProprty_Size; nCnt++)
	{
		//オブジェクトの種類がモデルだったら
		if (object[nCnt]->GetObjType() == CObject::OBJTYPE::MODEL || object[nCnt]->GetObjType() == CObject::OBJTYPE::MODEL_COLLISION)
		{
			if (object[nCnt]->GetObjType() == CObject::OBJTYPE::MODEL)
			{
				//modelSingleにキャスト
				CModelSingle *pModel = (CModelSingle*)object[nCnt];

				if (pModel->GetColl() == true)
				{
					D3DXMATRIX modelInvMtx;
					D3DXMATRIX modelMtx = pModel->GetModel()->GetMtx();
					D3DXMatrixIdentity(&modelInvMtx);
					D3DXMatrixInverse(&modelInvMtx, NULL, &modelMtx);

					D3DXVECTOR3 posV = posCameraV;

					D3DXVECTOR3 endPos = posV + m_rayVec;
					D3DXVec3TransformCoord(&posV, &posV, &modelInvMtx);
					D3DXVec3TransformCoord(&endPos, &endPos, &modelInvMtx);

					D3DXVECTOR3 vec = endPos - posV;

					//レイとメッシュの当たり判定
					if (D3DXIntersect(pModel->GetModel()->GetMesh(), &posV, &vec, &bHit, NULL, NULL, NULL, &fDiffer, NULL, NULL) == D3D_OK)
					{
						//当たったとき
						if (bHit)
						{
							if (m_fDiffer > fDiffer)
							{
								//距離を保存
								m_fDiffer = fDiffer;
								//当たったオブジェクトを保存
								pHitObject = object[nCnt];
								//レイの方向を保存
								rayVecHit = vec;
								//カメラのローカル座標を保存
								hitPosV = posV;
								bHitAny = true;
							}
						}
					}
				}
			}
		}
	}

	//プレイヤーのポインタ
	CPlayer *pPlayerObj = nullptr;

	//オブジェクト情報を入れるポインタ
	object.clear();

	//先頭のポインタを代入
	object = CObject::GetObject(static_cast<int>(CObject::PRIORITY::PLAYER));
	nProprty_Size = object.size();

	for (int nCnt = 0; nCnt < nProprty_Size; nCnt++)
	{
		//オブジェクトの種類がプレイヤーだったら
		if (object[nCnt]->GetObjType() == CObject::OBJTYPE::PLAYER)
		{
			//プレイヤーにキャスト
			pPlayerObj = nullptr;
			pPlayerObj = (CPlayer*)object[nCnt];
		}
	}
	//銃口のマトリックス
	D3DXMATRIX mtx = pPlayerObj->GetGunModel()->GetMuzzleMtx();
	D3DXVECTOR3 gunPos = { mtx._41, mtx._42, mtx._43 };


	object.clear();

	//先頭のポインタを代入
	object = CObject::GetObject(static_cast<int>(CObject::PRIORITY::ENEMY));
	nProprty_Size = object.size();

	for (int nCnt = 0; nCnt < nProprty_Size; nCnt++)
	{
		//オブジェクトの種類が敵だったら
		if (object[nCnt]->GetObjType() == CObject::OBJTYPE::ENEMY)
		{
			//敵にキャスト
			CEnemy *pEnemy = (CEnemy*)object[nCnt];

			//モデル取得
			CModelCollision *pModel = pEnemy->GetModelCollision();

			if (pModel->GetColl() == true)
			{
				D3DXMATRIX modelInvMtx;
				D3DXMATRIX modelMtx = pModel->GetModel()->GetMtx();
				D3DXMatrixIdentity(&modelInvMtx);
				D3DXMatrixInverse(&modelInvMtx, NULL, &modelMtx);

				D3DXVECTOR3 posV = posCameraV;

				D3DXVECTOR3 endPos = posV + m_rayVec;
				D3DXVec3TransformCoord(&posV, &posV, &modelInvMtx);
				D3DXVec3TransformCoord(&endPos, &endPos, &modelInvMtx);

				D3DXVECTOR3 vec = endPos - posV;

				//レイとメッシュの当たり判定
				if (D3DXIntersect(pModel->GetModel()->GetMesh(), &posV, &vec, &bHit, NULL, NULL, NULL, &fDiffer, NULL, NULL) == D3D_OK)
				{
					//当たったとき
					if (bHit)
					{
						if (m_fDiffer > fDiffer)
						{
							//距離を保存
							m_fDiffer = fDiffer;
							//当たったオブジェクトを保存
							pHitObject = pModel;
							//レイの方向を保存
							rayVecHit = vec;
							//カメラのローカル座標を保存
							hitPosV = posV;
							bHitAny = false;
							bHitEnemy = true;

							//敵の番号保存
							m_nPlayer = pEnemy->GetPlayerNumber();
						}
					}
				}
			}
		}
	}





	//モデルと当たっていて敵に当たっていないとき
	if (bHitAny)
	{
		//modelSingleにキャスト
		CModelSingle *pHitModel = (CModelSingle*)pHitObject;

		D3DXVec3Normalize(&rayVecHit, &rayVecHit);
		//レイのベクトルを算出した距離の分伸ばす
		rayVecHit *= m_fDiffer;

		//カメラの位置から伸ばしたベクトルを足して当たった位置を算出
		D3DXVECTOR3 HitPos = hitPosV + rayVecHit;
		D3DXMATRIX hitModelMtx = pHitModel->GetModel()->GetMtx();
		D3DXVec3TransformCoord(&HitPos, &HitPos, &hitModelMtx);

		//終点を設定
		m_endPos = HitPos;
	}

	//敵に当たったら
	if (bHitEnemy)
	{
		//modelSingleにキャスト
		CModelCollision *pHitModel = (CModelCollision*)pHitObject;

		D3DXVec3Normalize(&rayVecHit, &rayVecHit);
		//レイのベクトルを算出した距離の分伸ばす
		rayVecHit *= m_fDiffer;

		//カメラの位置から伸ばしたベクトルを足して当たった位置を算出
		D3DXVECTOR3 HitPos = hitPosV + rayVecHit;
		D3DXMATRIX hitModelMtx = pHitModel->GetModel()->GetMtx();
		D3DXVec3TransformCoord(&HitPos, &HitPos, &hitModelMtx);

		//ダメージを設定
		m_nDamage = BULLET_DAMAGE;

		//終点を設定
		m_endPos = HitPos;
	}

	float fMeshDiffer = 0.0f;
	D3DXVECTOR3 meshHitPos = { 0.0f, 0.0f, 0.0f };
	bool bCollMesh = CMeshField::Collision(meshHitPos, fMeshDiffer, posCameraV, m_endPos);
	//メッシュフィールドに当たったら
	if (bCollMesh)
	{
		//当たったメッシュフィールドまでの距離がモデルの距離より遠いとき
		if (fMeshDiffer > m_fDiffer)
		{
			//モデルの当たった位置にエフェクトを出す
			CPresetEffect::SetEffect3D(2, m_endPos, {}, {});
			CPresetEffect::SetEffect3D(3, m_endPos, {}, {});
			//弾痕　　最後の引数に回転入れてください(Y軸部分のみ適応)
			CPresetEffect::SetEffect3D(4, m_endPos, {}, D3DXVECTOR3(0.0f, D3DX_PI / 2.0f, D3DX_PI));
			CPresetEffect::SetEffect3D(4, m_endPos, {}, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
		else
		{
			//終点を設定
			m_endPos = meshHitPos;
			//メッシュフィールドの当たった位置にエフェクトを出す
			CPresetEffect::SetEffect3D(2, meshHitPos, {}, {});
			CPresetEffect::SetEffect3D(3, meshHitPos, {}, {});
		}
	}
	else if(!bCollMesh && bHitAny)
	{//モデルに当たったら
		//モデルの当たった位置にエフェクトを出す
		CPresetEffect::SetEffect3D(2, m_endPos, {}, {});
		CPresetEffect::SetEffect3D(3, m_endPos, {}, {});
		//弾痕　　最後の引数に回転入れてください(Y軸部分のみ適応)
		CPresetEffect::SetEffect3D(4, m_endPos, {}, D3DXVECTOR3(0.0f, D3DX_PI / 2.0f, D3DX_PI));
		CPresetEffect::SetEffect3D(4, m_endPos, {}, D3DXVECTOR3(0.0f, 0.0f , 0.0f));
	}
	else if (!bCollMesh && bHitEnemy)
	{//敵に当たったら
		//モデルの当たった位置にエフェクトを出す
		CPresetEffect::SetEffect3D(2, m_endPos, {}, {});
		CPresetEffect::SetEffect3D(3, m_endPos, {}, {});
	}


	//弾の軌道エフェクトを生成
	m_apOrbit[0] = CObject3D::Create(gunPos, { BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f }, { 0.0f, rotCamera.y + D3DX_PI / 2.0f, rotCamera.x + D3DX_PI / 2.0f });
	m_apOrbit[0]->BindTexture(CManager::GetTexture()->GetTexture("bullet_00.png"));
	m_apOrbit[0]->SetOriginType(CObject3D::ORIGIN_TYPE::LEFT);
	m_apOrbit[0]->SetPos(gunPos, { BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f });

	m_apOrbit[1] = CObject3D::Create(gunPos, { BULLET_SIZE_Y, BULLET_SIZE_X, 0.0f }, { rotCamera.x, rotCamera.y, 0.0f });
	m_apOrbit[1]->BindTexture(CManager::GetTexture()->GetTexture("bullet_01.png"));
	m_apOrbit[1]->SetOriginType(CObject3D::ORIGIN_TYPE::LOWER);
	m_apOrbit[1]->SetPos(gunPos, { BULLET_SIZE_Y, BULLET_SIZE_X, 0.0f });

	//カリングをオフにする
	m_apOrbit[0]->SetCulling(false);
	m_apOrbit[1]->SetCulling(false);
	//ライティングオフにする
	m_apOrbit[0]->SetLighting(false);
	m_apOrbit[1]->SetLighting(false);
	//ライティングオフにする
	m_apOrbit[0]->SetAlphaTest(true);
	m_apOrbit[1]->SetAlphaTest(true);

	return S_OK;
}

//================================================
//終了処理
//================================================
void CBullet::Uninit(void)
{	
	//オブジェクトの破棄
	Release();
}

//================================================
//更新処理
//================================================
void CBullet::Update(void)
{
	//プレイヤーのポインタ
	CPlayer *pPlayerObj = nullptr;

	//オブジェクト情報を入れるポインタ
	vector<CObject*> object;

	//先頭のポインタを代入
	object = CObject::GetObject(static_cast<int>(CObject::PRIORITY::PLAYER));
	int nProprty_Size = object.size();

	for (int nCnt = 0; nCnt < nProprty_Size; nCnt++)
	{
		//オブジェクトの種類がプレイヤーだったら
		if (object[nCnt]->GetObjType() == CObject::OBJTYPE::PLAYER)
		{
			//プレイヤーにキャスト
			pPlayerObj = nullptr;
			pPlayerObj = (CPlayer*)object[nCnt];
		}
	}


	for (int nCntOrbit = 0; nCntOrbit < BULLET_MAX_ORBIT; nCntOrbit++)
	{
		if (m_apOrbit[nCntOrbit] != nullptr)
		{
			//位置とサイズを取得
			D3DXVECTOR3 pos = m_apOrbit[nCntOrbit]->GetPos();

			//移動した位置から終点までのベクトルを算出
			D3DXVECTOR3 differVec = m_endPos - pos;
			float fDiffer = D3DXVec3Length(&differVec);

			if (fDiffer < BULLET_SIZE_X)
			{
				//消す
				m_apOrbit[nCntOrbit]->Uninit();
				m_apOrbit[nCntOrbit] = nullptr;
				break;
			}

			D3DXVECTOR3 size = m_apOrbit[nCntOrbit]->GetSize();

			//銃口のマトリックス
			D3DXMATRIX mtx = pPlayerObj->GetGunModel()->GetMuzzleMtx();
			D3DXVECTOR3 gunPos = { mtx._41, mtx._42, mtx._43 };
			//飛ばす方向を設定
			D3DXVECTOR3 vec = m_endPos - gunPos;
			//正規化
			D3DXVec3Normalize(&vec, &vec);
			//移動量を設定
			D3DXVECTOR3 move = vec * BULLET_MOVE_SPEED;
			pos += move;

			//移動した位置から終点までのベクトルを算出
			differVec = m_endPos - pos;
			fDiffer = D3DXVec3Length(&differVec);

			if (fDiffer < BULLET_SIZE_X)
			{
				//消す
				m_apOrbit[nCntOrbit]->Uninit();
				m_apOrbit[nCntOrbit] = nullptr;
			}
			else
			{
				//位置を設定
				m_apOrbit[nCntOrbit]->SetPos(pos, size);
			}
		}
	}
}

//================================================
//描画処理
//================================================
void CBullet::Draw(void)
{
	
}

//================================================
//生成処理
//================================================
CBullet *CBullet::Create(void)
{
	//インスタンスの生成
	CBullet *pBullet = nullptr;
	if (pBullet == nullptr)
	{
		pBullet = new CBullet;
		if (pBullet != nullptr)
		{
			pBullet->Init();
		}
	}
	return pBullet;
}