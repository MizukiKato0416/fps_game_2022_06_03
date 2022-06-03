//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : 羽鳥太一
//
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "mouseinput.h"
#include "keyinput.h"

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CCamera::CCamera()
{
	m_fDifferVR = 0.0f;
}

//=============================================================================
// デフォルトデストラクタ
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CCamera::Init(D3DXVECTOR3 PosV, D3DXVECTOR3 PosR, D3DXVECTOR3 Rot)
{
	m_PosV = PosV;
	m_PosR = PosR;
	m_Rot = Rot;
	m_VecU = (D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	m_fDifferVR = 1000.0f;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CCamera::Uninit(void)
{
	
}

//=============================================================================
// 更新処理
//=============================================================================
void CCamera::Update(void)
{
	CInputKeyboard *pKeyboard;
	CInputMouse *pMouse;
	POINT Mouse;
	pMouse = CManager::GetMouse();
	pKeyboard = CManager::GetInputKeyboard();
	GetCursorPos(&Mouse);

	D3DXVECTOR2 mouseVelocity = D3DXVECTOR2(pMouse->GetMousePos().lX, pMouse->GetMousePos().lY);

	if (pKeyboard->GetPress(pKeyboard->SPACE) == true)
	{
		m_PosV.y += 10.0f;
	}
	else if (pKeyboard->GetPress(pKeyboard->LSHIFT) == true)
	{
		m_PosV.y -= 10.0f;
	}

	//================================================
	//マウスによる視点移動処理
	//================================================
	if (pMouse->GetPress(CInputMouse::RIGHT_BOTTON))
	{


		if (mouseVelocity.x != 0.0f)
		{
			m_Rot.y += mouseVelocity.x * 0.003f;
		}
		if (mouseVelocity.y != 0.0f)
		{
			m_Rot.x += mouseVelocity.y * -0.001;
		}

		//移動処理
		Move();
	}



	//ホイールによる注視点から支店までの距離を伸ばす処理
	if (pMouse->GetMousePos().lZ >= 120)
	{
		m_fDifferVR -= 50.0f;
	}
	else if (pMouse->GetMousePos().lZ <= -120)
	{
		m_fDifferVR += 50.0f;
	}

	if (m_Rot.y >= D3DX_PI)
	{
		m_Rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_Rot.y <= -D3DX_PI)
	{
		m_Rot.y += D3DX_PI * 2.0f;
	}
	if (m_Rot.x >= D3DX_PI / 8.0f * 7.0f)
	{
		m_Rot.x = D3DX_PI / 8.0f * 7.0f;
	}
	else if (m_Rot.x <= D3DX_PI / 8.0f)
	{
		m_Rot.x = D3DX_PI / 8.0f;
	}

	//視点の場所を注視点を元に移動
	m_PosV.x = m_PosR.x + m_fDifferVR * sinf(m_Rot.x) * sinf(m_Rot.y);
	m_PosV.z = m_PosR.z + m_fDifferVR * sinf(m_Rot.x) * cosf(m_Rot.y);
	m_PosV.y = m_PosR.y + m_fDifferVR * cosf(m_Rot.x);

	CalcScreenToWorld(&m_MouseWorldPos, Mouse.x, Mouse.y, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, &m_mtxView, &m_mtxProjection);
}

//=============================================================================
// セット処理
//=============================================================================
void CCamera::SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスのポインタ
	pDevice = CManager::GetRenderer()->GetDevice();		//デバイスを取得する
	//---------------------------
	//マトリックスの設定
	//---------------------------
	D3DXMatrixIdentity(&m_mtxProjection);	//マトリックス初期化

	//マトリックスの作成
	D3DXMatrixPerspectiveFovLH(	&m_mtxProjection,
								D3DXToRadian(90.0f),	//視野角
								(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
								10.0f,		//カメラの最小描画距離
								30000.0f);	//カメラの最大描画距離

	//プロジェクションマトリックス設定
	pDevice->SetTransform(	D3DTS_PROJECTION,
							&m_mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(	&m_mtxView,
						&m_PosV,
						&m_PosR,
						&m_VecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(	D3DTS_VIEW,
							&m_mtxView);
}

D3DXVECTOR3 *CCamera::CalcScreenToWorld(D3DXVECTOR3* pout, int Sx, int Sy, float fZ, int Screen_w, int Screen_h, D3DXMATRIX* View, D3DXMATRIX* Prj)
{
	// 各行列の逆行列を算出
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse(&InvView, NULL, View);
	D3DXMatrixInverse(&InvPrj, NULL, Prj);
	D3DXMatrixIdentity(&VP);
	VP._11 = Screen_w / 2.0f; VP._22 = -Screen_h / 2.0f;
	VP._41 = Screen_w / 2.0f; VP._42 = Screen_h / 2.0f;
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// 逆変換
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVec3TransformCoord(pout, &D3DXVECTOR3(Sx, Sy, fZ), &tmp);

	return pout;
}

//=============================================================================
// 生成処理
//=============================================================================
CCamera *CCamera::Create(D3DXVECTOR3 PosV, D3DXVECTOR3 PosR, D3DXVECTOR3 Rot)
{
	CCamera *pCamera = NULL;
	if (pCamera == NULL)
	{
		pCamera = new CCamera;
	}

	if (pCamera)
	{
		pCamera->Init(PosV, PosR, Rot);
	}
	return pCamera;
}


//================================================
//動き処理
//================================================
void CCamera::Move(void)
{
	//キーボード取得処理
	CInputKeyboard *pInputKeyboard;
	pInputKeyboard = CManager::GetInputKeyboard();

	//WASDを押したら
	if (pInputKeyboard->GetPress(pInputKeyboard->W) == true || pInputKeyboard->GetPress(pInputKeyboard->A) == true ||
		pInputKeyboard->GetPress(pInputKeyboard->S) == true || pInputKeyboard->GetPress(pInputKeyboard->D) == true)
	{
		float fObjectiveRot = 0.0f;

		if (pInputKeyboard->GetPress(pInputKeyboard->W) == true)
		{
			if (pInputKeyboard->GetPress(pInputKeyboard->A) == true)
			{
				//目的の向きを設定
				fObjectiveRot = m_Rot.y - D3DX_PI / 4.0f;
			}
			else if (pInputKeyboard->GetPress(pInputKeyboard->D) == true)
			{
				//目的の向きを設定
				fObjectiveRot = m_Rot.y + D3DX_PI / 4.0f;
			}
			else
			{
				//目的の向きを設定
				fObjectiveRot = m_Rot.y;
			}
		}
		else if (pInputKeyboard->GetPress(pInputKeyboard->S) == true)
		{
			if (pInputKeyboard->GetPress(pInputKeyboard->A) == true)
			{
				//目的の向きを設定
				fObjectiveRot = m_Rot.y - D3DX_PI / 4.0f * 3.0f;
			}
			else if (pInputKeyboard->GetPress(pInputKeyboard->D) == true)
			{
				//目的の向きを設定
				fObjectiveRot = m_Rot.y + D3DX_PI / 4.0f * 3.0f;
			}
			else
			{
				//目的の向きを設定
				fObjectiveRot = m_Rot.y + D3DX_PI;
			}
		}
		else if (pInputKeyboard->GetPress(pInputKeyboard->A) == true)
		{
			//目的の向きを設定
			fObjectiveRot = m_Rot.y - D3DX_PI / 2.0f;
		}
		else if (pInputKeyboard->GetPress(pInputKeyboard->D) == true)
		{
			//目的の向きを設定
			fObjectiveRot = m_Rot.y + D3DX_PI / 2.0f;
		}

		//移動量加算
		m_PosR.x += sinf(fObjectiveRot + D3DX_PI) * 20.0f;
		m_PosR.z += cosf(fObjectiveRot + D3DX_PI) * 20.0f;
	}
}
