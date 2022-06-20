//=============================================================================
//
// モデルテスト処理 [modeltester.cpp]
// Author : 羽鳥太一
//
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include "gunmodel.h"
#include "manager.h"
#include "renderer.h"
#include "model_single.h"
#include "model.h"
#include "model.h"

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CGunModel::CGunModel(CObject::PRIORITY Priority) : CObject(Priority)
{

}

//=============================================================================
// デフォルトデストラクタ
//=============================================================================
CGunModel::~CGunModel()
{

}

//=============================================================================
// モデルの初期化処理
//=============================================================================
HRESULT CGunModel::Init(void)
{
	m_model = CModelSingle::Create(m_pos, {0.0f, 2.0f, 0.0f}, m_type, NULL, false);

	return S_OK;
}

//=============================================================================
// モデルの終了処理
//=============================================================================
void CGunModel::Uninit(void)
{
	Release();
}

//=============================================================================
// モデルの更新処理
//=============================================================================
void CGunModel::Update(void)
{

}

//=============================================================================
// モデルの描画処理
//=============================================================================
void CGunModel::Draw(void)
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
	pDevice->SetTransform(	D3DTS_WORLD,
							&m_mtx_muzzle_world);

	mtx_parent = *m_mtxParent;

	D3DXMatrixMultiply(	&m_mtx_muzzle_world,
						&m_mtx_muzzle_world,
						&mtx_parent);
}

//=============================================================================
// モデルの生成
//=============================================================================
CGunModel *CGunModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 muzzle_pos, string type)
{
	CGunModel *gun = NULL;
	if (gun == NULL)
	{
		gun = new CGunModel;
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
