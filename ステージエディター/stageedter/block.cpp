//=============================================================================
//
// ブロック処理 [block.cpp]
// Author : 羽鳥太一
//
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include "block.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CBlock::CBlock(LAYER_TYPE Layer) : CScene(Layer)
{
	m_bUse = false;
}

//=============================================================================
// デフォルトデストラクタ
//=============================================================================
CBlock::~CBlock()
{

}

//=============================================================================
// ポリゴンの初期化
//=============================================================================
HRESULT CBlock::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int Type)
{
	m_Pos = pos;
	m_Rot = rot;
	m_Scale = scale;
	m_pModel = CModel::Create(Type);
	m_pModel->SetScale(scale);

	return S_OK;
}

HRESULT CBlock::Init(void)
{
	return S_OK;
}

//=============================================================================
// ポリゴンの終了
//=============================================================================
void CBlock::Uninit(void)
{
	m_pModel->Uninit();
	Release();
}

//=============================================================================
// ポリゴンの更新
//=============================================================================
void CBlock::Update(void)
{
	m_pModel->Update();
}

//=============================================================================
// ポリゴンの描画
//=============================================================================
void CBlock::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = NULL;	//デバイスへのポインタ
	pDevice = CManager::GetRenderer()->GetDevice();	// デバイスを取得する

	//-----------------------------------
	//プレイヤーの設定
	//-----------------------------------
	D3DXMATRIX mtxRot, mtxTrans, m_mtxView;	//マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_MtxWold);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 
									m_Rot.y,
									m_Rot.x, 
									m_Rot.z);
	D3DXMatrixMultiply( &m_MtxWold, 
						&m_MtxWold, 
						&mtxRot);
	//位置を反映
	D3DXMatrixTranslation(	&mtxTrans, 
							m_Pos.x, 
							m_Pos.y, 
							m_Pos.z);
	D3DXMatrixMultiply( &m_MtxWold, 
						&m_MtxWold, 
						&mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWold);

	m_pModel->Draw(m_bUse);
}

//=============================================================================
// 生成
//=============================================================================
CBlock *CBlock::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int type)
{
	CBlock *pBlock = NULL;
	if (pBlock == NULL)
	{
		pBlock = new CBlock;
	}
	
	if (pBlock)
	{
		pBlock->Init(pos, rot, scale, type);
	}
	return pBlock;
}