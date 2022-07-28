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
#include "ballistic.h"
#include "renderer.h"
#include "player.h"
#include "gun_player.h"
#include "mesh_field.h"
#include "xanimmodel.h"
#include "hierarchydata.h"
#include "enemy.h"
#include "networkmanager.h"

//================================================
//マクロ定義
//================================================
#define BULLET_MOVE_SPEED			(450.0f)						//軌道の速さ
#define BULLET_SIZE_X				(200.0f)						//軌道のサイズ
#define BULLET_SIZE_Y				(2.5f)							//軌道のサイズ
#define BULLET_MAX_END_POS			(10000.0f)						//軌道の終点最大値
#define BULLET_DAMAGE				(20)							//ダメージ
#define BULLET_PLAYER_COL_SIZE_Y	(90.0f)							//当たり判定サイズ調整値
#define BULLET_PLAYER_COL_RADIUS	(28.0f)							//当たり判定半径
#define BULLET_RAND					(rand() % 280 + -140)			//拡散量
#define BULLET_RAND_JUMP			(rand() % 360 + -180)			//ジャンプしたときの拡散量

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
	//オブジェクトの種類を設定
	SetObjType(CObject::OBJTYPE::BULLET);

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
	bool bHitModel = false;
	m_nDamage = BULLET_DAMAGE;

	//ADSしているかどうか保存用変数
	bool bAds = false;
	//ジャンプしているかどうか保存用変数
	bool bJump = false;

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
			CPlayer *pPlayerObj = nullptr;
			pPlayerObj = (CPlayer*)object[nCnt];

			//ADSしているかどうか取得
			bAds = pPlayerObj->GetAds();
			//ジャンプ取得
			bJump = pPlayerObj->GetJump();
		}
	}



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

			//腰うちだったら
			if (!bAds)
			{
				////ジャンプしていたら
				//if (bJump)
				//{
				//	//拡散させる量設定
				//	m_rand.x = float(BULLET_RAND_JUMP);
				//	m_rand.y = float(BULLET_RAND_JUMP);
				//	m_rand.z = float(BULLET_RAND_JUMP);
				//}
				//else
				//{//ジャンプしていなかったら
				//	//拡散させる量設定
				//	m_rand.x = float(BULLET_RAND);
				//	m_rand.y = float(BULLET_RAND);
				//	m_rand.z = float(BULLET_RAND);
				//}

				//拡散させる量設定
				m_rand.x = float(BULLET_RAND);
				m_rand.y = float(BULLET_RAND);
				m_rand.z = float(BULLET_RAND);

				//腰うちの場合はランダムに拡散させる
				posCameraR.x += cosf(rotCamera.y) * m_rand.x;
				posCameraR.z += sinf(rotCamera.y) * m_rand.z;
				posCameraR.y += m_rand.y;
			}

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
	object.clear();

	//先頭のポインタを代入
	object = CObject::GetObject(static_cast<int>(CObject::PRIORITY::MODEL));
	nProprty_Size = object.size();

	for (int nCnt = 0; nCnt < nProprty_Size; nCnt++)
	{
		//オブジェクトの種類がモデルだったら
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
							//モデルに当たっている状態にする
							bHitModel = true;
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
	D3DXMATRIX mtx = pPlayerObj->GetGunPlayer()->GetMuzzleMtx();
	D3DXVECTOR3 gunPos = { mtx._41, mtx._42, mtx._43 };

	//モデルと当たっているとき
	if (bHitModel)
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

	float fMeshDiffer = 0.0f;
	D3DXVECTOR3 meshHitPos = { 0.0f, 0.0f, 0.0f };

	//メッシュフィールドの当たり判定
	bool bCollMesh = CMeshField::Collision(meshHitPos, fMeshDiffer, posCameraV, m_endPos);
	//メッシュフィールドに当たったら
	if (bCollMesh)
	{
		//当たったメッシュフィールドまでの距離がモデルの距離より近い時
		if (fMeshDiffer < m_fDiffer)
		{
			//終点を設定
			m_endPos = meshHitPos;
			bHitModel = false;
			//距離を保存
			m_fDiffer = fMeshDiffer;
		}
		else
		{
			bCollMesh = false;
		}
	}

	//初期化
	object.clear();
	bool bHitEnemy = false;
	float fSaveDiffer = m_fDiffer;

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

			//マトリックス取得
			D3DXMATRIX enemyMtx = pEnemy->GetMtx();

			//当たった位置保存用変数
			D3DXVECTOR3 hitPos = { 0.0f, 0.0f, 0.0f };
			D3DXVECTOR3 endPos = { 0.0f, 0.0f, 0.0f };

			// レイとカプセルの当たり判定
			if (calcRayCapsule(posCameraV.x, posCameraV.y, posCameraV.z,
				m_rayVec.x, m_rayVec.y, m_rayVec.z,
				enemyMtx._41, enemyMtx._42, enemyMtx._43,
				enemyMtx._41, enemyMtx._42 + BULLET_PLAYER_COL_SIZE_Y, enemyMtx._43,
				BULLET_PLAYER_COL_RADIUS,
				hitPos.x, hitPos.y, hitPos.z,
				endPos.x, endPos.y, endPos.z))
			{
				D3DXVECTOR3 hitVec = hitPos - posCameraV;
				// ベクトルを正規化
				D3DXVec3Normalize(&hitVec, &hitVec);


				if ((m_rayVec.x > 0.0f && hitVec.x < 0.0f || m_rayVec.x < 0.0f && hitVec.x > 0.0f) &&
					(m_rayVec.y > 0.0f && hitVec.y < 0.0f || m_rayVec.y < 0.0f && hitVec.y > 0.0f) &&
					(m_rayVec.z > 0.0f && hitVec.z < 0.0f || m_rayVec.z < 0.0f && hitVec.z > 0.0f))
				{
					//当たっていない
				}
				else
				{//当たってる
					// 当たった場所までの距離を算出
					D3DXVECTOR3 differVec = hitPos - posCameraV;
					fDiffer = D3DXVec3Length(&differVec);

					if (fSaveDiffer > fDiffer)
					{
						// 距離を保存
						fSaveDiffer = fDiffer;
						// 当たった
						bHitEnemy = true;
					}
				}
			}
		}
	}

	//当たったオブジェクトによってエフェクトを出す
	if (!bCollMesh && bHitModel && !bHitEnemy)
	{//モデルに当たったら
		//当たった位置にエフェクトを出す
		CPresetEffect::SetEffect3D(2, m_endPos, {}, {});
		CPresetEffect::SetEffect3D(3, m_endPos, {}, {});
		//弾痕　　最後の引数に回転入れてください(Y軸部分のみ適応)
		CPresetEffect::SetEffect3D(4, m_endPos, {}, D3DXVECTOR3(0.0f, D3DX_PI / 2.0f, D3DX_PI));
		CPresetEffect::SetEffect3D(4, m_endPos, {}, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	else if (bCollMesh && !bHitModel && !bHitEnemy)
	{//メッシュフィールドに当たったら
		//当たった位置にエフェクトを出す
		CPresetEffect::SetEffect3D(2, m_endPos, {}, {});
		CPresetEffect::SetEffect3D(3, m_endPos, {}, {});
	}

	//弾の軌道エフェクトを生成
	CBallistic::Create(gunPos, { BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f }, rotCamera, m_endPos, BULLET_MOVE_SPEED, "bullet_00.png", "bullet_01.png");

	//通信データに情報を突っ込む
	CCommunicationData::COMMUNICATION_DATA *pData = CManager::GetInstance()->GetNetWorkmanager()->GetPlayerData()->GetCmmuData();

	if(!bCollMesh && bHitModel)
	{//モデルに当たったら
		//モデルに当たっている状態にする
		pData->Bullet.type = CCommunicationData::HIT_TYPE::MODEL;
	}
	else if (bCollMesh && !bHitModel)
	{//メッシュフィールドに当たったら
		//メッシュフィールドに当たっている状態にする
		pData->Bullet.type = CCommunicationData::HIT_TYPE::MESHFIELD;
	}
	else if (!bCollMesh && !bHitModel)
	{//何にもあたっていなかったら
		//何も当たっていない状態にする
		pData->Bullet.type = CCommunicationData::HIT_TYPE::NONE;
	}

	// 情報を設定
	pData->Bullet.fDiffer = m_fDiffer;
	pData->Bullet.hitPos = m_endPos;
	pData->Bullet.nDamage = m_nDamage;
	pData->Bullet.bUse = true;
	pData->Ballistic.BigenPos = gunPos;
	pData->Ballistic.Size = { BULLET_SIZE_X, BULLET_SIZE_Y, 0.0f };
	pData->Ballistic.Rot = rotCamera;
	pData->Ballistic.EndPos = m_endPos;
	pData->Ballistic.fSpeed = BULLET_MOVE_SPEED;
	pData->Player.CamV = posCameraV;
	pData->Player.CamR = posCameraR;

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
	Uninit();
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

