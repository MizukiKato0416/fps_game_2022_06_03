//================================================
//パーティクル処理
//Author:加藤瑞葵
//================================================
#include "effect2D.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//================================================
//マクロ定義
//================================================


//================================================
//静的メンバ変数宣言
//================================================

//================================================
//デフォルトコンストラクタ
//================================================
CEffect2D::CEffect2D(CObject::PRIORITY Priority) : CObject2D(Priority)
{
	m_type = EFFECT2D_TYPE::NONE;
	m_bAlphaBlendSubtraction = false;
}

//================================================
//オーバーロードされたコンストラクタ
//================================================

//================================================
//デストラクタ
//================================================
CEffect2D::~CEffect2D()
{

}

//================================================
//初期化処理
//================================================
HRESULT CEffect2D::Init(void)
{
	//変数初期化
	m_bAlphaBlendSubtraction = false;

	CObject2D::Init();

	//オブジェクトの種類を設定
	SetObjType(CObject::OBJTYPE::EFFECT_2D);

	return S_OK;
}

//================================================
//終了処理
//================================================
void CEffect2D::Uninit(void)
{
	CObject2D::Uninit();
}

//================================================
//更新処理
//================================================
void CEffect2D::Update(void)
{
	
}

//================================================
//描画処理
//================================================
void CEffect2D::Draw(void)
{
	//デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	if (m_bAlphaBlendSubtraction == false)
	{
		//αブレンディングを加算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}
	else
	{
		//減算合成の設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	CObject2D::Draw();

	if (m_bAlphaBlendSubtraction == false)
	{
		//aブレンディングを通常に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
	else
	{
		//減算合成から通常の合成に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

//================================================
//生成処理
//================================================
CEffect2D* CEffect2D::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &size, const D3DXCOLOR &col, const EFFECT2D_TYPE &type)
{
	//インスタンスの生成
	CEffect2D *pEffect2D = nullptr;
	if (pEffect2D == nullptr)
	{
		pEffect2D = new CEffect2D();
		if (pEffect2D != nullptr)
		{
			pEffect2D->m_type = type;
			pEffect2D->Init();
			pEffect2D->SetCol(col);
			pEffect2D->SetPos(pos, size);
		}
	}
	return pEffect2D;
}
