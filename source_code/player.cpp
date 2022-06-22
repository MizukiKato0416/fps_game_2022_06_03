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
#include <thread>
#include "PresetSetEffect.h"
#include "bullet.h"
#include "gunmodel.h"
#include "object2D.h"

//================================================
//マクロ定義
//================================================
#define PLAYER_JUMP							(16.0f)									//ジャンプ力
#define PLAYER_GRAVITY						(1.2f)									//重力の大きさ
#define PLAYER_WALK_SPEED					(3.5f)									//歩き移動の移動量
#define PLAYER_RUN_SPEED					(6.0f)									//走り移動の移動量
#define PLAYER_ADS_WALK_SPEED				(2.0f)									//ADS中の移動速度
#define PLAYER_SIZE							(75.0f)									//プレイヤーのサイズ調整値
#define PLAYER_SHOT_COUNTER					(5)										//次の弾が出るまでのカウンター
#define PLAYER_ADS_GUN_OFFSET				(D3DXVECTOR3(0.0f, -3.0f, 2.0f))		//ADSしたときの銃のオフセット
#define PLAYER_ADS_CAMERA_ADD_RADIUS		(10.0f)									//ADSしたときの画角加算量
#define PLAYER_ADS_CAMERA_RADIUS			(65.0f)									//ADSしたときの画角

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

	//銃モデルの生成
	m_pGunModel = CGunModel::Create({0.0f, 0.0f, 0.0f}, { 0.0f, 0.0f, 0.0f}, { 0.0f, 1.6f, 12.0f }, "asult_gun_inv.x");

	//位置の設定
	SetPos(m_pos);
	SetPosOld(m_posOld);

	//オブジェクトの種類の設定
	SetObjType(CObject::OBJTYPE::PLAYER);

	// アニメーション付きXファイルの生成
	m_pAnimModel = CXanimModel::Create("data/armmotion.x");
	//ニュートラルモーションにする
	m_pAnimModel->ChangeAnimation("neutral", 60.0f / 4800.0f);

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
	CShadow::Create(D3DXVECTOR3(m_pos.x, 0.0f, m_pos.z), D3DXVECTOR3(m_size.x, 0.0f, m_size.z), this);

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
	CSound *sound;
	sound = CManager::GetInstance()->GetSound();
	CTcpClient *pTcp = CManager::GetInstance()->GetCommunication();
	CCommunicationData::COMMUNICATION_DATA *pData = m_commu_data.GetCmmuData();

	//位置取得
	D3DXVECTOR3 pos = GetPos();

	m_pos = pos;
	m_posOld = pos;

	//1フレーム前の位置設定
	SetPosOld(m_posOld);

	//腰の処理
	Chest();

	//移動処理
	Move();

	//重力
	m_move.y -= PLAYER_GRAVITY;

	m_pos += m_move;		//移動量反映

	//回転の慣性
	Rotate();

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

	m_pAnimModel->Update();

	pData->Player.Pos = m_pos;
	pData->Player.Rot = m_rot;
	pData->Player.fMotionSpeed = m_fAnimSpeed;
	pTcp->Send((char*)pData, sizeof(CCommunicationData::COMMUNICATION_DATA));

	//射撃処理
	Shot();

	//ADS処理
	ADS();
}

//================================================
//描画処理
//================================================
void CPlayer::Draw(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;			//計算用マトリックス

	D3DXMatrixIdentity(&m_mtxWorld);		//プレイヤーのワールドマトリックスの初期化

	D3DXVECTOR3 pos, rot;

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

	D3DXMATRIX *cameraMtx = nullptr;
	//cameraのポインタ配列1番目のアドレス取得
	CCamera **pCameraAddress = CManager::GetInstance()->GetCamera();

	for (int nCntCamera = 0; nCntCamera < MAX_MAIN_CAMERA; nCntCamera++, pCameraAddress++)
	{
		//cameraの取得
		CCamera *pCamera = &**pCameraAddress;
		if (pCamera != nullptr)
		{
			D3DXVECTOR3 rotCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			//cameraの向き取得
			rotCamera = pCamera->GetRotV();

			rotCamera.x -= D3DX_PI / 2.0f;

			//プレイヤーの向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

			cameraMtx = pCamera->GetMtxPoint();
		}
	}

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

	//m_pAnimModel->Draw();

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
	}
	else
	{
		//銃と親子関係をつける
		m_pGunModel->GetModel()->GetModel()->SetMtxParent(cameraMtx);
		m_pGunModel->GetModel()->GetModel()->SetObjParent(true);
		m_pGunModel->GetModel()->GetModel()->SetRot({ 0.0f, 0.0f, 0.0f });
		m_pGunModel->GetModel()->GetModel()->SetPos(PLAYER_ADS_GUN_OFFSET);
	}
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

	if (pInputMouse->GetPress(CInputMouse::MOUSE_TYPE::MOUSE_TYPE_LEFT) == true)
	{
		//撃つアニメーションでなかったら
		if (m_pAnimModel->GetAnimation() != "fireneutral")
		{
			//撃つモーションにする
			m_fAnimSpeed = (20.0f * 3.0f) / 4800.0f;
			m_pAnimModel->ChangeAnimation("fireneutral", m_fAnimSpeed);
		}

		//カウンターを減算
		m_nCounter--;

		//0より小さくなったら
		if (m_nCounter < 0)
		{
			//既定の値にする
			m_nCounter = PLAYER_SHOT_COUNTER;

			//オフセット位置設定
			D3DXVECTOR3 pos = { m_pGunModel->GetMuzzleMtx()._41, m_pGunModel->GetMuzzleMtx()._42, m_pGunModel->GetMuzzleMtx()._43};
			pos += m_posOld - m_pos;

			//マズルフラッシュエフェクトの生成
			CPresetEffect::SetEffect3D(0, pos, {}, {});
			CPresetEffect::SetEffect3D(1, pos, {}, {});

			//弾の生成
			CBullet::Create();
		}
	}
	else
	{
		//撃つアニメーションだったら
		if (m_pAnimModel->GetAnimation() == "fireneutral")
		{
			//ニュートラルモーションにする
			m_fAnimSpeed = (20.0f * 3.0f) / 4800.0f;
			m_pAnimModel->ChangeAnimation("neutral", m_fAnimSpeed);
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
	//cameraのポインタ配列1番目のアドレス取得
	CCamera **pCameraAddress = CManager::GetInstance()->GetCamera();

	for (int nCntCamera = 0; nCntCamera < MAX_MAIN_CAMERA; nCntCamera++, pCameraAddress++)
	{
		//cameraの取得
		CCamera *pCamera = &**pCameraAddress;
		if (pCamera != nullptr)
		{
			D3DXVECTOR3 rotCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			//cameraの向き取得
			rotCamera = pCamera->GetRotV();

			D3DXMATRIX cameraMtx;
			D3DXMatrixIdentity(&cameraMtx);
			D3DXMatrixRotationYawPitchRoll(&cameraMtx, 0.0f, rotCamera.x, 0.0f);

			//マトリックスを取得
			D3DXMATRIX *handR = nullptr;
			handR = m_pAnimModel->GetMatrix("handR");

			handR->_11 = 10050.0f;

			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &cameraMtx);

			//m_pAnimModel->SetMatrix("handR", handR);
		}
	}
}

//================================================
//プレイヤーとの当たり判定
//================================================
void CPlayer::Collision(CObject *&pSubjectObject, const float &fObjRadius)
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
				CModelSingle::Collision(pPlayerObj);
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