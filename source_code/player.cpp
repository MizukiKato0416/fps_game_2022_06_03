//================================================
//プレイヤー処理
//Author:加藤瑞葵
//================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "manager.h"
#include "renderer.h"
#include "input_keyboard.h"
#include "input_pad_d.h"
#include "input_mouse.h"
#include "player.h"
#include "x_load.h"
#include "model_single.h"
#include "texture.h"
#include "camera.h"
#include "shadow.h"
#include "floor.h"
#include "mesh_field.h"
#include "xanimmodel.h"
#include "model.h"
#include "tcp_client.h"
#include "communicationdata.h"
#include "game01.h"
#include "PresetSetEffect.h"
#include "bullet.h"
#include "gunmodel.h"
#include "object2D.h"
#include "enemy.h"
#include "networkmanager.h"
#include "model_collision.h"

//================================================
//マクロ定義
//================================================
#define PLAYER_JUMP							(16.0f)									//ジャンプ力
#define PLAYER_GRAVITY						(1.2f)									//重力の大きさ
#define PLAYER_WALK_SPEED					(3.5f)									//歩き移動の移動量
#define PLAYER_RUN_SPEED					(6.0f)									//走り移動の移動量
#define PLAYER_ADS_WALK_SPEED				(2.0f)									//ADS中の移動速度
#define PLAYER_SHOT_COUNTER					(5)										//次の弾が出るまでのカウンター
#define PLAYER_ADS_GUN_OFFSET				(D3DXVECTOR3(0.0f, -3.3f, 5.5f))		//ADSしたときの銃のオフセット
#define PLAYER_ADS_CAMERA_ADD_RADIUS		(10.0f)									//ADSしたときの画角加算量
#define PLAYER_ADS_CAMERA_RADIUS			(65.0f)									//ADSしたときの画角
#define CAMERA_V_SPEED_Y			(0.03f)											//カメラの横移動スピード
#define CAMERA_V__MOUSE_SPEED_Y		(0.002f)										//カメラの横移動スピード（マウスの時）
#define CAMERA_V_SPEED_XZ			(0.01f)											//カメラの縦移動スピード
#define CAMERA_V__MOUSE_SPEED_XZ	(-0.0005f)										//カメラの横移動スピード（マウスの時）

//================================================
//デフォルトコンストラクタ
//================================================
CPlayer::CPlayer(CObject::PRIORITY Priority):CObject(Priority)
{
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_offsetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pGunModel = nullptr;
	m_fObjectiveRot = 0.0f;
	m_fNumRot = 0.0f;
	m_bRotate = false;
	m_bJump = false;
	m_fMoveSpeed = 0.0f;
	m_nCounter = 0;
	m_bAds = false;
	m_pCloss = nullptr;
	m_nLife = 0;
	m_pCollModel = nullptr;
	m_bShot = false;
	m_pShadow = nullptr;
}

//================================================
//オーバーロードされたコンストラクタ
//================================================


//================================================
//デストラクタ
//================================================
CPlayer::~CPlayer()
{

}

//================================================
//初期化処理
//================================================
HRESULT CPlayer::Init(void)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_offsetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fObjectiveRot = 0.0f;
	m_fNumRot = 0.0f;
	m_bRotate = false;
	m_bJump = false;
	m_fMoveSpeed = 0.0f;
	m_nCounter = 0;
	m_bAds = false;
	m_nLife = PLAYER_LIFE;
	m_bShot = false;

	//銃モデルの生成
	m_pGunModel = CGunModel::Create({0.0f, 0.0f, 0.0f}, { 0.0f, 0.0f, 0.0f}, { 0.0f, 1.6f, 12.0f }, "asult_gun_inv.x");
	m_pGunModel->SetMtxParent(m_pGunModel->GetModel()->GetModel()->GetMtxPoint());

	//位置の設定
	SetPos(m_pos);
	SetPosOld(m_posOld);

	//オブジェクトの種類の設定
	SetObjType(CObject::OBJTYPE::PLAYER);

	// アニメーション付きXファイルの生成
	m_pAnimModel = CXanimModel::Create("data/armmotion.x");
	//ニュートラルモーションにする
	m_pAnimModel->ChangeAnimation("neutral", 60.0f / 4800.0f);

	//当たり判定ボックスの生成
	//m_pCollModel = CModelCollision::Create({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, "player_coll.x", nullptr, true);

	//サイズを取得
	D3DXVECTOR3 modelSize = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	modelSize = m_pAnimModel->GetSize();

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

	m_pCloss = CObject2D::Create({ SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f }, {SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f}, (int)CObject::PRIORITY::UI);
	m_pCloss->BindTexture(CManager::GetInstance()->GetTexture()->GetTexture("closs.png"));

	return S_OK;
}

//================================================
//終了処理
//================================================
void CPlayer::Uninit(void)
{
	//マトリックスを取得
	D3DXMATRIX *handR = nullptr;
	handR = m_pAnimModel->GetMatrix("handR");

	//newしたので消す
	delete handR;
	handR = nullptr;

	m_pAnimModel->Uninit();
	Release();
}

//================================================
//更新処理
//================================================
void CPlayer::Update(void)
{
	char Send[MAX_COMMU_DATA];

	CSound *sound;
	sound = CManager::GetInstance()->GetSound();
	CCommunicationData::COMMUNICATION_DATA *pData = CManager::GetInstance()->GetNetWorkmanager()->GetPlayerData()->GetCmmuData();

	//位置取得
	D3DXVECTOR3 pos = GetPos();

	m_pos = pos;
	m_posOld = pos;

	//1フレーム前の位置設定
	SetPosOld(m_posOld);

	//腰の処理
	Chest();

	//回転の慣性
	Rotate();

	//移動処理
	Move();

	//重力
	m_move.y -= PLAYER_GRAVITY;

	m_pos += m_move;		//移動量反映

	//位置反映
	SetPos(m_pos);

	//床との当たり判定
	if (CFloor::Collision(this, m_size.x) == true)
	{
		//重力を0にする
		m_move.y = 0.0f;

		//ジャンプをしていない状態にする
		m_bJump = false;

		//ジャンプ処理
		Jump();

		//位置取得
		pos = GetPos();
		m_pos = pos;
	}

	//メッシュフィールドとの当たり判定
	if (CMeshField::Collision(this, m_size.x * 20.0f) == true)
	{
		//重力を0にする
		m_move.y = 0.0f;

		//ジャンプをしていない状態にする
		m_bJump = false;

		//ジャンプ処理
		Jump();

		//位置取得
		pos = GetPos();
		m_pos = pos;
	}

	//モデルとの当たり判定
	int nHit = CModelSingle::Collision(this, m_size.x / 2.0f, 150.0f);
	//上からあたったとき
	if (nHit == 1)
	{
		//重力を0にする
		m_move.y = 0.0f;

		//ジャンプをしていない状態にする
		m_bJump = false;

		//ジャンプ処理
		Jump();
	}
	else if (nHit == 2)
	{//下からあたったとき
		//重力を0にする
		m_move.y = 0.0f;
	}

	//位置取得
	pos = GetPos();
	m_pos = pos;

	//影の当たり判定
	//m_pShadow->Collision(m_pos, m_size.x * 20.0f);

	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;			//計算用マトリックス

	D3DXMatrixIdentity(&m_mtxWorld);		//プレイヤーのワールドマトリックスの初期化
	
	D3DXVECTOR3 rot;

	//親子関係がつけられていたら
	if (m_bObjParent == true)
	{
		pos = m_offsetPos;
		rot = m_rot;
	}
	else
	{//つけられていなかったら
		pos = m_pos;
		rot = m_rot;
	}

	//プレイヤーの向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//プレイヤーの位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_bObjParent == true)
	{
		D3DXMATRIX mtxParent = *m_mtxWorldParent;

		//算出したパーツのワールドマトリックスと親のワールドマトリックスを掛け合わせる
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);
	}

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);


	D3DXMATRIX *cameraMtx = nullptr;
	//cameraのポインタ配列1番目のアドレス取得
	CCamera **pCameraAddress = CManager::GetInstance()->GetCamera();
	//カメラの位置保存用
	D3DXVECTOR3 posCameraV = { 0.0f, 0.0f, 0.0f };
	D3DXVECTOR3 posCameraR = { 0.0f, 0.0f, 0.0f };

	for (int nCntCamera = 0; nCntCamera < MAX_MAIN_CAMERA; nCntCamera++, pCameraAddress++)
	{
		//cameraの取得
		CCamera *pCamera = &**pCameraAddress;
		if (pCamera != nullptr)
		{
			//カメラのマトリックス取得
			cameraMtx = pCamera->GetMtxPoint();
			//カメラのマトリックスと親子関係をつける
			m_pAnimModel->SetMatrix(cameraMtx);
			//カメラの位置取得
			posCameraV = pCamera->GetPosV();
			posCameraR = pCamera->GetPosR();
		}
	}

	//腕の描画
	m_pAnimModel->Draw();

	if (!m_bAds)
	{
		//マトリックスを取得
		D3DXMATRIX *handR = nullptr;
		handR = m_pAnimModel->GetMatrix("handR");
		m_pGunModel->SetMtxParent(m_pGunModel->GetModel()->GetModel()->GetMtxPoint());
		//銃と親子関係をつける
		m_pGunModel->GetModel()->GetModel()->SetMtxParent(handR);
		m_pGunModel->GetModel()->GetModel()->SetObjParent(true);
		m_pGunModel->GetModel()->GetModel()->SetRot({ 0.0f, D3DX_PI / 2.0f, 0.0f });
		m_pGunModel->GetModel()->GetModel()->SetPos({ 0.0f, 0.0f, 0.0f });
		m_pGunModel->GetModel()->SetCulliMode(false);
		m_pGunModel->Draw();
	}
	else
	{
		//銃と親子関係をつける
		m_pGunModel->GetModel()->GetModel()->SetMtxParent(cameraMtx);
		m_pGunModel->GetModel()->GetModel()->SetObjParent(true);
		m_pGunModel->GetModel()->GetModel()->SetRot({ 0.0f, 0.0f, 0.0f });
		m_pGunModel->GetModel()->GetModel()->SetPos(PLAYER_ADS_GUN_OFFSET);
		m_pGunModel->GetModel()->SetCulliMode(true);
		m_pGunModel->Draw();
	}

	//射撃処理
	Shot();

	m_pAnimModel->Update();

	//ADS処理
	ADS();

	//当たったかどうか
	HitBullet();

	pData->Player.Pos = m_pos;
	pData->Player.Rot = m_rot;
	pData->Player.fMotionSpeed = m_fAnimSpeed;
	pData->Player.CamV = posCameraV;
	pData->Player.CamR = posCameraR;
	pData->Player.ModelMatrix = m_mtxWorld;

	memcpy(&Send[0], pData, sizeof(CCommunicationData::COMMUNICATION_DATA));
	CManager::GetInstance()->GetNetWorkmanager()->Send(&Send[0], sizeof(CCommunicationData::COMMUNICATION_DATA));
	pData->Player.nFrameCount++;
	pData->Bullet.bUse = false;
	if (pData->Player.nFrameCount <= SEND_COUNTER - 1)
	{
		pData->Player.nFrameCount = 0;
	}
}

//================================================
//描画処理
//================================================
void CPlayer::Draw(void)
{
	//親子関係をつける
	//m_pCollModel->GetModel()->SetMtxParent(&m_mtxWorld);
	//m_pCollModel->GetModel()->SetObjParent(true);
}

//================================================
//生成処理
//================================================
CPlayer *CPlayer::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot)
{
	//インスタンスの生成
	CPlayer *pPlayer = nullptr;
	if (pPlayer == nullptr)
	{
		pPlayer = new CPlayer;
		if (pPlayer != nullptr)
		{
			pPlayer->m_pos = pos;
			pPlayer->m_posOld = pos;
			pPlayer->m_rot = rot;
			pPlayer->Init();
		}
	}
	return pPlayer;
}

//================================================
//モデルの動き処理
//================================================
void CPlayer::Move(void)
{
	//パッドD取得処理
	CInputPadD *pInputPadD = CManager::GetInstance()->GetInputPadD();
	DIJOYSTATE2 JoyStick = pInputPadD->GetGamepad();

	// 通信データ取得処理
	CCommunicationData::COMMUNICATION_DATA *pData = CManager::GetInstance()->GetNetWorkmanager()->GetPlayerData()->GetCmmuData();

	//キーボード取得処理
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//パッドX取得処理
	CInputPadX *pInputPadX;
	pInputPadX = CManager::GetInstance()->GetInputPadX();

	D3DXVECTOR3 rotCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//cameraのポインタ配列1番目のアドレス取得
	CCamera **pCameraAddress = CManager::GetInstance()->GetCamera();

	for (int nCntCamera = 0; nCntCamera < MAX_MAIN_CAMERA; nCntCamera++, pCameraAddress++)
	{
		//cameraの取得
		CCamera *pCamera = &**pCameraAddress;
		if (pCamera != nullptr)
		{
			//cameraの向き取得
			rotCamera = pCamera->GetRotV();
		}
	}

	//移動量設定用変数
	m_fMoveSpeed = PLAYER_WALK_SPEED;

	//スティックの傾きがあったらまたはWASDを押したら
	if ((float)JoyStick.lX != 0.0f || (float)JoyStick.lY != 0.0f || 
		pInputKeyboard->GetPress(DIK_W) == true || pInputKeyboard->GetPress(DIK_A) == true ||
		pInputKeyboard->GetPress(DIK_S) == true || pInputKeyboard->GetPress(DIK_D) == true)
	{
		//歩きモーションでなかったら
		if (m_pAnimModel->GetAnimation() != "walk")
		{
			//歩きモーションにする
			m_fAnimSpeed = (20.0f * 3.0f) / 4800.0f;
			m_pAnimModel->ChangeAnimation("walk", m_fAnimSpeed);
			memcpy(pData->Player.aMotion[0], m_pAnimModel->GetAnimation().c_str(), m_pAnimModel->GetAnimation().size());
		}

		//目的の向きを設定
		if ((float)JoyStick.lX != 0.0f || (float)JoyStick.lY != 0.0f)
		{
			m_fObjectiveRot = rotCamera.y + atan2f((float)JoyStick.lY, (float)JoyStick.lX);
		}
		else if (pInputKeyboard->GetPress(DIK_W) == true)
		{
			//SHIFTを押したら早くする
			if (pInputKeyboard->GetPress(DIK_LSHIFT) == true)
			{
				m_fMoveSpeed = PLAYER_RUN_SPEED;

				//走りモーションにする
				m_fAnimSpeed = (20.0f * 5.0f) / 4800.0f;
				m_pAnimModel->ChangeSpeed(m_fAnimSpeed);
			}
			else
			{
				//歩きモーションにする
				m_fAnimSpeed = (20.0f * 3.0f) / 4800.0f;
				m_pAnimModel->ChangeSpeed(m_fAnimSpeed);
			}

			if (pInputKeyboard->GetPress(DIK_A) == true)
			{
				//目的の向きを設定
				m_fObjectiveRot = rotCamera.y - D3DX_PI / 4.0f;
			}
			else if (pInputKeyboard->GetPress(DIK_D) == true)
			{
				//目的の向きを設定
				m_fObjectiveRot = rotCamera.y + D3DX_PI / 4.0f;
			}
			else
			{
				//目的の向きを設定
				m_fObjectiveRot = rotCamera.y;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{
			if (pInputKeyboard->GetPress(DIK_A) == true)
			{
				//目的の向きを設定
				m_fObjectiveRot = rotCamera.y - D3DX_PI / 4.0f * 3.0f;
			}
			else if (pInputKeyboard->GetPress(DIK_D) == true)
			{
				//目的の向きを設定
				m_fObjectiveRot = rotCamera.y + D3DX_PI / 4.0f * 3.0f;
			}
			else
			{
				//目的の向きを設定
				m_fObjectiveRot = rotCamera.y + D3DX_PI;
			}
		}
		else if (pInputKeyboard->GetPress(DIK_A) == true)
		{
			//目的の向きを設定
			m_fObjectiveRot = rotCamera.y - D3DX_PI / 2.0f;
		}
		else if (pInputKeyboard->GetPress(DIK_D) == true)
		{
			//目的の向きを設定
			m_fObjectiveRot = rotCamera.y + D3DX_PI / 2.0f;
		}

		if (m_bAds)
		{
			m_fMoveSpeed = PLAYER_ADS_WALK_SPEED;
		}

		//移動量加算
		m_move.x = -sinf(m_fObjectiveRot + D3DX_PI) * m_fMoveSpeed;
		m_move.z = -cosf(m_fObjectiveRot + D3DX_PI) * m_fMoveSpeed;
		//回転をさせる
		m_bRotate = true;
	}
	else
	{//スティックに傾きがなかったら

		//移動量をゼロにする
		m_move.x = 0.0f;
		m_move.z = 0.0f;

		//ニュートラルモーションでなかったら且つ撃つモーションじゃなかったら
		if (m_pAnimModel->GetAnimation() != "neutral" && m_pAnimModel->GetAnimation() != "fireneutral")
		{
			//ニュートラルモーションにする
			m_pAnimModel->ChangeAnimation("neutral", 60.0f / 4800.0f);
			m_fAnimSpeed = 60.0f / 4800.0f;
			memcpy(pData->Player.aMotion[0], m_pAnimModel->GetAnimation().c_str(), m_pAnimModel->GetAnimation().size());
		}
	}
}

//================================================
//回転処理
//================================================
void CPlayer::Rotate(void)
{
	//cameraのポインタ配列1番目のアドレス取得
	CCamera **pCameraAddress = CManager::GetInstance()->GetCamera();

	for (int nCntCamera = 0; nCntCamera < MAX_MAIN_CAMERA; nCntCamera++, pCameraAddress++)
	{
		//cameraの取得
		CCamera *pCamera = &**pCameraAddress;
		if (pCamera != nullptr)
		{
			//マウス取得処理
			CInputMouse *pInputMouse;
			pInputMouse = CManager::GetInstance()->GetInputMouse();

			//マウスの移動量取得
			D3DXVECTOR2 mouseVelocity = pInputMouse->GetMouseVelocity();

			//カメラの向き取得
			D3DXVECTOR3 cameraRot = pCamera->GetRotV();

			//================================================
			//マウスによる視点移動処理
			//================================================
			if (mouseVelocity.x != 0.0f)
			{
				cameraRot.y += mouseVelocity.x * CAMERA_V__MOUSE_SPEED_Y;
			}
			if (mouseVelocity.y != 0.0f)
			{
				cameraRot.x -= mouseVelocity.y * CAMERA_V__MOUSE_SPEED_XZ;
			}

			pCamera->SetRotV(cameraRot);


			//cameraの向き取得
			m_rot.y = pCamera->GetRotV().y + D3DX_PI;
		}
	}

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

//================================================
//ジャンプ処理
//================================================
void CPlayer::Jump(void)
{
	//キーボード取得処理
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	if (pInputKeyboard->GetTrigger(DIK_SPACE) == true)	//SPACEボタンを押したときの処理
	{
		//移動量をジャンプ分加算
		m_move.y = PLAYER_JUMP;
		m_bJump = true;
	}
}

//================================================
//射撃処理
//================================================
void CPlayer::Shot(void)
{
	//マウス取得処理
	CInputMouse *pInputMouse;
	pInputMouse = CManager::GetInstance()->GetInputMouse();

	// 通信データ取得処理
	CCommunicationData::COMMUNICATION_DATA *pData = CManager::GetInstance()->GetNetWorkmanager()->GetPlayerData()->GetCmmuData();

	if (pInputMouse->GetPress(CInputMouse::MOUSE_TYPE::MOUSE_TYPE_LEFT) == true)
	{
		//撃つアニメーションでなかったら
		if (m_pAnimModel->GetAnimation() != "fireneutral")
		{
			//撃つモーションにする
			m_fAnimSpeed = (20.0f * 3.0f) / 4800.0f;
			m_pAnimModel->ChangeAnimation("fireneutral", m_fAnimSpeed);
			memcpy(pData->Player.aMotion[0], m_pAnimModel->GetAnimation().c_str(), m_pAnimModel->GetAnimation().size());
		}

		//カウンターを減算
		m_nCounter--;

		//撃っている状態なら
		if (m_bShot == true)
		{
			m_pGunModel->GetModel()->GetModel()->SetMtx();
			//オフセット位置設定
			D3DXVECTOR3 pos = { m_pGunModel->GetMuzzleMtx()._41, m_pGunModel->GetMuzzleMtx()._42, m_pGunModel->GetMuzzleMtx()._43 };

			//マズルフラッシュエフェクトの生成
			CPresetEffect::SetEffect3D(0, pos, {}, {});
			CPresetEffect::SetEffect3D(1, pos, {}, {});

			CBullet *pBullet;	// 弾のポインタ
			//弾の生成
			pBullet = CBullet::Create();

			//撃っていない状態にする
			m_bShot = false;
		}

		//0より小さくなったら
		if (m_nCounter < 0)
		{
			//既定の値にする
			m_nCounter = PLAYER_SHOT_COUNTER;

			//撃ってる状態にする
			m_bShot = true;
		}
	}
	else
	{
		//弾を使ってない
		pData->Bullet.bUse = false;
		pData->Bullet.fDiffer = 0.0f;
		pData->Bullet.hitPos = { 0.0f, 0.0f, 0.0f };
		pData->Bullet.nDamage = 0;
		pData->Bullet.type = CCommunicationData::HIT_TYPE::NONE;
		pData->Ballistic.BigenPos = { 0.0f, 0.0f, 0.0f };
		pData->Ballistic.EndPos = { 0.0f, 0.0f, 0.0f };
		pData->Ballistic.fSpeed = 0.0f;
		pData->Ballistic.Rot = { 0.0f, 0.0f, 0.0f };
		pData->Ballistic.Size = { 0.0f, 0.0f, 0.0f };

		//撃つアニメーションだったら
		if (m_pAnimModel->GetAnimation() == "fireneutral")
		{
			//ニュートラルモーションにする
			m_fAnimSpeed = (20.0f * 3.0f) / 4800.0f;
			m_pAnimModel->ChangeAnimation("neutral", m_fAnimSpeed);
			memcpy(pData->Player.aMotion[0], m_pAnimModel->GetAnimation().c_str(), m_pAnimModel->GetAnimation().size());
		}

		//既定の値より小さかったら
		if (m_nCounter > 0 && m_nCounter <= PLAYER_SHOT_COUNTER)
		{
			//カウンターを加算
			m_nCounter++;

			//既定の値より大きくなったら
			if (m_nCounter > PLAYER_SHOT_COUNTER)
			{
				//0にする
				m_nCounter = 0;
			}
		}
	}
}

//================================================
//ADSの処理
//================================================
void CPlayer::ADS(void)
{
	//マウス取得処理
	CInputMouse *pInputMouse;
	pInputMouse = CManager::GetInstance()->GetInputMouse();

	//右クリックをしたら
	if (pInputMouse->GetPress(CInputMouse::MOUSE_TYPE::MOUSE_TYPE_RIGHT) == true)
	{
		//ADS状態でないなら
		if (!m_bAds)
		{
			//ADS状態にする
			m_bAds = true;
		}
		//cameraのポインタ配列1番目のアドレス取得
		CCamera **pCameraAddress = CManager::GetInstance()->GetCamera();

		for (int nCntCamera = 0; nCntCamera < MAX_MAIN_CAMERA; nCntCamera++, pCameraAddress++)
		{
			//cameraの取得
			CCamera *pCamera = &**pCameraAddress;
			if (pCamera != nullptr)
			{
				//cameraの画角取得
				float fRadius = pCamera->GetRadius();
				//既定より大きいとき
				if (fRadius > PLAYER_ADS_CAMERA_RADIUS)
				{
					//減らす
					fRadius -= PLAYER_ADS_CAMERA_ADD_RADIUS;

					//既定より小さくなったら
					if (fRadius < PLAYER_ADS_CAMERA_RADIUS)
					{
						//既定の値にする
						fRadius = PLAYER_ADS_CAMERA_RADIUS;
					}
					//画角を設定
					pCamera->SetRadius(fRadius);
				}
			}
		}
	}
	else
	{
		//ADS状態なら
		if (m_bAds)
		{
			//ADS状態をやめる
			m_bAds = false;

			//cameraのポインタ配列1番目のアドレス取得
			CCamera **pCameraAddress = CManager::GetInstance()->GetCamera();

			for (int nCntCamera = 0; nCntCamera < MAX_MAIN_CAMERA; nCntCamera++, pCameraAddress++)
			{
				//cameraの取得
				CCamera *pCamera = &**pCameraAddress;
				if (pCamera != nullptr)
				{
					//cameraの画角取得
					float fRadius = pCamera->GetRadius();
					//既定より小さいとき
					if (fRadius < CAMERA_RADIUS)
					{
						//既定の値にする
						fRadius = CAMERA_RADIUS;
						//画角を設定
						pCamera->SetRadius(fRadius);
					}
				}
			}
		}
	}
}

//================================================
//腰の処理
//================================================
void CPlayer::Chest(void)
{
	
}

//================================================
//被弾処理
//================================================
void CPlayer::HitBullet(void)
{
	//通信したデータ取得
	CCommunicationData::COMMUNICATION_DATA *pData = CManager::GetInstance()->GetNetWorkmanager()->GetPlayerData()->GetCmmuData();
	//プレイヤーに送られていたら
	if (pData->SendType == CCommunicationData::COMMUNICATION_TYPE::SEND_TO_ENEMY_AND_PLAYER)
	{
		//弾が自分に当たっていたら
		if (pData->Player.bHit == true)
		{
			//カメラの位置を取得
			D3DXVECTOR3 cameraRot = { 0.0f, 0.0f, 0.0f };
			//cameraのポインタ配列1番目のアドレス取得
			CCamera **pCameraAddress = CManager::GetInstance()->GetCamera();

			for (int nCntCamera = 0; nCntCamera < MAX_MAIN_CAMERA; nCntCamera++, pCameraAddress++)
			{
				//cameraの取得
				CCamera *pCamera = &**pCameraAddress;
				if (pCamera != nullptr)
				{
					cameraRot = pCamera->GetRotV();
				}
			}
			//当たった位置を示すエフェクトを出す
			CPresetEffect::SetEffect2D(0, { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, m_pos, { 0.0f, cameraRot.y, 0.0f });

			//ライフを減らす
			m_nLife -= pData->Player.nHitDamage;
			//ライフが0になったら
			if (m_nLife <= 0)
			{
				//リスポーン
				m_pos = { 0.0f, 1000.0f, 0.0f };
				SetPos(m_pos);
				m_nLife = PLAYER_LIFE;
			}
			//自分に与えられるダメージを0にリセット
			pData->Player.nHitDamage = 0;
			//当たっていない状態にする
			pData->Player.bHit = false;
			//敵に送る状態にする
			pData->SendType = CCommunicationData::COMMUNICATION_TYPE::SEND_TO_ENEMY_AND_PLAYER;
		}

		//弾の数分まわす
		for (int nCntBullet = 0; nCntBullet < pData->Player.nNumShot; nCntBullet++)
		{
			//当たった場所を取得
			D3DXVECTOR3 hitPos = pData->Player.HitPos[nCntBullet];

			//当たったオブジェクトによって処理分け
			if (pData->Player.type[nCntBullet] == CCommunicationData::HIT_TYPE::ENEMY)
			{
				//当たった位置にエフェクトを出す
				CPresetEffect::SetEffect3D(5, hitPos, m_pos, {});
			}
		}
		//弾の撃った数を0にする
		pData->Player.nNumShot = 0;
	}
	
	pData->Player.bHit = false;
	pData->Player.nHitDamage = 0;
}

//================================================
//プレイヤーとの当たり判定
//================================================
void CPlayer::Collision(CObject *pSubjectObject, const float &fObjRadius)
{
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

			//オブジェクトの位置を取得
			D3DXVECTOR3 posObj = pSubjectObject->GetPos();

			//プレイヤーから対象のオブジェクトまでの距離を求める
			float fRadius = sqrtf((posObj.x - pPlayerObj->m_pos.x) * (posObj.x - pPlayerObj->m_pos.x) +
				(posObj.z - pPlayerObj->m_pos.z) * (posObj.z - pPlayerObj->m_pos.z));
			//プレイヤーと対象のオブジェクトの角度を求める
			float fRot = atan2((posObj.x - pPlayerObj->m_pos.x), (posObj.z - pPlayerObj->m_pos.z)) - D3DX_PI;

			//求めた距離がプレイヤーの半径と対象のオブジェクトの半径を足した数値以下だったら
			if (fRadius <= pPlayerObj->m_size.x / 2.0f + fObjRadius)
			{
				//プレイヤーの位置を押し出す
				pPlayerObj->m_pos.x = posObj.x + (sinf(fRot) * (pPlayerObj->m_size.x / 2.0f + fObjRadius));
				pPlayerObj->m_pos.z = posObj.z + (cosf(fRot) * (pPlayerObj->m_size.x / 2.0f + fObjRadius));

				//位置設定
				pPlayerObj->SetPos(pPlayerObj->m_pos);

				//モデルとの当たり判定
				int nHit = CModelSingle::Collision(pPlayerObj, pPlayerObj->m_size.x / 2.0f, 150.0f);
				//上からあたったとき
				if (nHit == 1)
				{
					//重力を0にする
					pPlayerObj->m_move.y = 0.0f;

					//ジャンプをしていない状態にする
					pPlayerObj->m_bJump = false;

					//ジャンプ処理
					pPlayerObj->Jump();
				}
				else if (nHit == 2)
				{//下からあたったとき
				 //重力を0にする
					pPlayerObj->m_move.y = 0.0f;
				}
			}
		}
	}
}

//================================================
//プレイヤーとの衝突判定のみの処理
//================================================
bool CPlayer::CollisionOnly(CObject *&pSubjectObject, const float &fObjRadius)
{
	//オブジェクトの位置を取得
	D3DXVECTOR3 posObj = pSubjectObject->GetPos();

	//プレイヤーから対象のオブジェクトまでの距離を求める
	float fRadius = sqrtf((posObj.x - m_pos.x) * (posObj.x - m_pos.x) +
		(posObj.z - m_pos.z) * (posObj.z - m_pos.z));

	//求めた距離がプレイヤーの半径と対象のオブジェクトの半径を足した数値以下だったら
	if (fRadius <= m_size.x / 2.0f + fObjRadius)
	{
		//当たっている
		return true;
	}

	return false;
}